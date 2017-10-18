#include "stdafx.h"
#include "ImageProcHandle.h"


CImageProcHandle::CImageProcHandle()
{
}


CImageProcHandle::~CImageProcHandle()
{

}

//======================================================//
//=函数名：		去除图像重影
//=功  能：	由于二色镜的反射面和背向面都能反射，导致图像上
//=			产生一个重影叠加，影响图像质量。
//=输  入：
//======================================================//
cv::Mat RemoveFringeFrame(cv::Mat matImageIn, U16 wOffsetX, U16 wOffsetY, float fOffsetAlpha)
{
	cv::Mat matIndex = cv::Mat::zeros(10,10,CV_16U);

	return matIndex;
}

