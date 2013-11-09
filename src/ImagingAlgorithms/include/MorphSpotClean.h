#ifndef MORPHSPOTCLEAN_H
#define MORPHSPOTCLEAN_H

#include <base/timage.h>
#include <morphology/morphology.h>
#include <logging/logger.h>
#include <containers/ArrayBuffer.h>

#include <string>
#include <iostream>

#include "pixelinfo.h"


namespace ImagingAlgorithms {

enum eMorphCleanMethod {
    MorphCleanHoles = 0,
    MorphCleanPeaks,
    MorphCleanBoth
};

class MorphSpotClean
{
    kipl::logging::Logger logger;
    const float mark;
public:
    MorphSpotClean();
    void Process(kipl::base::TImage<float,2> &img, float th);

    void setConnectivity(kipl::morphology::MorphConnect conn = kipl::morphology::conn8);
    void setCleanMethod(eMorphCleanMethod mcm);
    kipl::base::TImage<float,2> DetectionImage(kipl::base::TImage<float,2> img);

protected:
    void ProcessReplace(kipl::base::TImage<float,2> &img);
    void ProcessFill(kipl::base::TImage<float,2> &img);

    void PadEdges(kipl::base::TImage<float,2> &img, kipl::base::TImage<float,2> &padded);
    void UnpadEdges(kipl::base::TImage<float,2> &padded, kipl::base::TImage<float,2> &img);
    /// \brief Prepares neighborhood indexing LUT
    /// \param dimx Length of the x-axis
    /// \param N number of pixels in the image
    int PrepareNeighborhood(int dimx, int N);

    /// \brief Extracts the neighborhood of a pixel. Skips pixels with the value max(float)
    /// \param pImg reference to the image matrix
    /// \param idx Index of the pixel
    /// \param neigborhood preallocated array to carry the extracted neighbors
    /// \returns number of extracted pixels in the array
    int Neighborhood(float * pImg, int idx, float * neigborhood);

    kipl::base::TImage<float,2> DetectSpots(kipl::base::TImage<float,2> img, kipl::containers::ArrayBuffer<PixelInfo> *pixels);
    kipl::base::TImage<float,2> DetectHoles(kipl::base::TImage<float,2> img);
    kipl::base::TImage<float,2> DetectPeaks(kipl::base::TImage<float,2> img);
    kipl::base::TImage<float,2> DetectBoth(kipl::base::TImage<float,2> img);

    void ExcludeLargeRegions(kipl::base::TImage<float,2> &img);

    kipl::base::TImage<float,2> CleanByArray(kipl::base::TImage<float,2> img, kipl::containers::ArrayBuffer<PixelInfo> *pixels);

    kipl::morphology::MorphConnect m_eConnectivity;
    eMorphCleanMethod              m_eMorphClean;
    int m_nEdgeSmoothLength;
    int m_nMaxArea;
    float m_fMinLevel;
    float m_fMaxLevel;
    float m_fThreshold;

    kipl::base::TImage<float,2> mask;
    int sx;
    int ng[8];
    int first_line;
    int last_line;

};

}

std::string enum2string(ImagingAlgorithms::eMorphCleanMethod mc);
std::ostream & operator<<(std::ostream &s, ImagingAlgorithms::eMorphCleanMethod mc);
void string2enum(std::string str, ImagingAlgorithms::eMorphCleanMethod &mc);
#endif // MORPHSPOTCLEAN_H
