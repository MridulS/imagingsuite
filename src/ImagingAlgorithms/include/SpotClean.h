/*
 * SpotClean.h
 *
 *  Created on: Jan 17, 2013
 *      Author: anders
 */

#ifndef IMGALG_SPOTCLEAN_H_
#define IMGALG_SPOTCLEAN_H_

#include "stdafx.h"

#include <base/timage.h>
#include <strings/miscstring.h>
#include <filters/filter.h>
#include <containers/ArrayBuffer.h>
#include <math/LUTCollection.h>
#include <logging/logger.h>

#include <sstream>
#include <algorithm>
#include <string>
#include <map>
#include <list>

namespace ImagingAlgorithms {

class DLL_EXPORT PixelInfo {
public:
	PixelInfo() :pos(0), value(0.0f), weight(0.0f) {}
	PixelInfo(int nPos,float fValue, float fWeight) : pos(nPos), value(fValue), weight(fWeight) {}
	PixelInfo(const PixelInfo &info) : pos(info.pos), value(info.value), weight(info.weight) {}
	PixelInfo & operator=(const PixelInfo & info) {
		pos   = info.pos;
		value = info.value;
		weight = info.weight;

		return *this;
	}
	int pos;
	float value;
	float weight;
};

enum DetectionMethod {
	Detection_StdDev = 0,
	Detection_Ring,
	Detection_Median,
	Detection_MinMax
};

class DLL_EXPORT SpotClean
{
protected:
	kipl::logging::Logger logger;
public:
	const float mark;
	SpotClean();
	virtual ~SpotClean(void);

	int Setup(size_t iterations,
			float threshold, float width,
			float minlevel, float maxlevel,
			int maxarea,
			ImagingAlgorithms::DetectionMethod method);

	kipl::base::TImage<float,2> DetectionImage(kipl::base::TImage<float,2> img, DetectionMethod method, size_t dims);
	kipl::base::TImage<float,2> DetectionImage(kipl::base::TImage<float,2> img);
	double ChangeStatistics(kipl::base::TImage<float,2> img);
	virtual std::string Version() {
		ostringstream s;
		//s<<"ImagingAlgorithms::SpotClean ("<<std::max(kipl::strings::VersionNumber("$Rev: 1384 $"), SourceVersion())<<")";
		s<<"ImagingAlgorithms::SpotClean ("<<std::max(kipl::strings::VersionNumber("$Rev: 1384 $"), 0)<<")";

		return s.str();
	}
	int Process(kipl::base::TImage<float,2> & img);
	int Process(kipl::base::TImage<float,3> & img);

protected:


	kipl::base::TImage<float,2> DetectSpots(kipl::base::TImage<float,2> img, kipl::containers::ArrayBuffer<PixelInfo> *pixels);
	void ExcludeLargeRegions(kipl::base::TImage<float,2> &img);

	kipl::base::TImage<float,2> CleanByList(kipl::base::TImage<float,2> img);
	kipl::base::TImage<float,2> CleanByArray(kipl::base::TImage<float,2> img,kipl::containers::ArrayBuffer<PixelInfo> *pixels);

	kipl::base::TImage<float,2> BoxFilter(kipl::base::TImage<float,2> img, size_t dim);
	kipl::base::TImage<float,2> StdDevDetection(kipl::base::TImage<float,2> img, size_t dim);
	kipl::base::TImage<float,2> MedianDetection(kipl::base::TImage<float,2> img);
	kipl::base::TImage<float,2> MinMaxDetection(kipl::base::TImage<float,2> img);
	kipl::base::TImage<float,2> RingDetection(kipl::base::TImage<float,2> img);



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


	float m_fGamma;
	float m_fSigma;
	int m_nIterations;
	float m_fMaxLevel;
	float m_fMinLevel;
	size_t m_nMaxArea;
	DetectionMethod m_eDetectionMethod;

	kipl::base::TImage<float,2> mask;
	kipl::base::TImage<float,2> medianImage;
	size_t nFilterDims[2];
	float kernel[64];
	int mNeighborhood;
	size_t nKernelSize;
	size_t nCurrentIteration;
	int sx;
	int ng[8];
	int first_line;
	int last_line;

	std::list<PixelInfo > processList;
	kipl::math::SigmoidLUT mLUT;

	kipl::filters::FilterBase::EdgeProcessingStyle eEdgeProcessingStyle;
};

}

void DLL_EXPORT string2enum(std::string str, ImagingAlgorithms::DetectionMethod & method);
std::string DLL_EXPORT enum2string(ImagingAlgorithms::DetectionMethod method);
std::ostream DLL_EXPORT & operator<<(std::ostream &s, ImagingAlgorithms::DetectionMethod method);

#endif /* SPOTCLEAN_H_ */
