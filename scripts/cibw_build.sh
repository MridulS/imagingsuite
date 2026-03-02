#!/bin/bash
set -euo pipefail

# Build script for cibuildwheel's before-build step.
# Called once per Python version to build C++ extensions and copy them into the package.
#
# Usage: cibw_build.sh <platform> <repo_root>
#   platform: linux | macos | windows
#   repo_root: path to imagingsuite repo root

PLATFORM="$1"
REPO_ROOT="$(cd "$2" && pwd)"
PACKAGE_DIR="${REPO_ROOT}/package"
BUILD_DIR="${REPO_ROOT}/../build-imagingsuite"

echo "=== cibw_build.sh ==="
echo "Platform: ${PLATFORM}"
echo "Repo root: ${REPO_ROOT}"
echo "Python: $(python --version)"
echo "Python path: $(which python)"

cd "${REPO_ROOT}"

# Select Conan profile based on platform and architecture
case "${PLATFORM}" in
    linux)
        ARCH=$(uname -m)
        if [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ]; then
            PROFILE="profiles/linux_gcc_11_aarch64_release"
        else
            PROFILE="profiles/linux_gcc_11_release"
        fi
        ;;
    macos)
        ARCH=$(uname -m)
        if [ "$ARCH" = "arm64" ]; then
            PROFILE="profiles/macos_arm_clang_15_release"
        else
            PROFILE="profiles/macos_x64_clang_14_release"
        fi
        ;;
    windows)
        PROFILE="profiles/windows_msvc_17_release"
        ;;
    *)
        echo "Unknown platform: ${PLATFORM}"
        exit 1
        ;;
esac

echo "Using Conan profile: ${PROFILE}"

# Clean stale build artifacts to force re-detection of the current Python version.
# Remove CMakeCache.txt so CMake re-runs find_package(Python3) with the correct hint.
# Remove the Conan generators directory so the toolchain file is regenerated with the
# correct Python3_EXECUTABLE / Python_EXECUTABLE for this Python version.
# Remove old cpython .so modules that may carry wrong Python version tags.
rm -f "${BUILD_DIR}/Release/CMakeCache.txt" 2>/dev/null || true
rm -f "${BUILD_DIR}/CMakeCache.txt" 2>/dev/null || true
rm -rf "${BUILD_DIR}/Release/generators" 2>/dev/null || true
rm -rf "${BUILD_DIR}/generators" 2>/dev/null || true
rm -f "${BUILD_DIR}/Release/lib/"*cpython*.so 2>/dev/null || true
rm -f "${BUILD_DIR}/lib/"*cpython*.so 2>/dev/null || true
# Also clean the package directory to avoid mixing versions
rm -f "${PACKAGE_DIR}/pymuhrec/"*cpython*.so 2>/dev/null || true

# Run Conan build (installs deps from cache + runs CMake configure + build)
# BUILD_GUI=OFF is set via CIBW_ENVIRONMENT, so CMakeToolchain picks it up
conan build . --profile:all "${PROFILE}" --build=missing

# Copy shared libraries and pybind11 modules to the package directory
echo "Copying built artifacts to ${PACKAGE_DIR}/pymuhrec/"

case "${PLATFORM}" in
    linux)
        cp "${BUILD_DIR}/Release/lib/"*.so* "${PACKAGE_DIR}/pymuhrec/" 2>/dev/null || true
        # Fix rpaths so shared libraries find each other in the package directory
        cd "${PACKAGE_DIR}/pymuhrec"
        for f in *.so*; do
            if [ -f "$f" ] && [ ! -L "$f" ]; then
                patchelf --set-rpath '$ORIGIN' "$f" 2>/dev/null || true
            fi
        done
        ;;
    macos)
        cp "${BUILD_DIR}/Release/lib/"*.dylib "${PACKAGE_DIR}/pymuhrec/" 2>/dev/null || true
        cp "${BUILD_DIR}/Release/lib/"*.so "${PACKAGE_DIR}/pymuhrec/" 2>/dev/null || true
        ;;
    windows)
        cp "${BUILD_DIR}/bin/Release/"*.dll "${PACKAGE_DIR}/pymuhrec/" 2>/dev/null || true
        cp "${BUILD_DIR}/bin/Release/"*.pyd "${PACKAGE_DIR}/pymuhrec/" 2>/dev/null || true
        ;;
esac

echo "=== cibw_build.sh complete ==="
