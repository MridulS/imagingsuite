from setuptools import setup, Distribution

class BinaryDistribution(Distribution):
    """Mark the distribution as containing extension modules (platform-specific)."""
    def has_ext_modules(self):
        return True

setup(
    distclass=BinaryDistribution,
)
