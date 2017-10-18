/************************************************************/
/*			ÕºœÒ¥¶¿Ì¿‡
/*
/************************************************************/
#ifndef _IMAGE_PROC_HANDLE_H_
#define _IMAGE_PROC_HANDLE_H_

#include "SystemParam.h"
#include "opencv2/opencv.hpp"

class CImageProcHandle
{
public:
	CImageProcHandle();
	~CImageProcHandle();

public:
	cv::Mat RemoveFringeFrame(cv::Mat matImageIn,U16 wOffsetX,U16 wOffsetY,float fOffsetAlpha);
};

#endif