#include "stdafx.h"
#include "ImageProcHandle.h"


CImageProcHandle::CImageProcHandle()
{
}


CImageProcHandle::~CImageProcHandle()
{

}

//======================================================//
//=��������		ȥ��ͼ����Ӱ
//=��  �ܣ�	���ڶ�ɫ���ķ�����ͱ����涼�ܷ��䣬����ͼ����
//=			����һ����Ӱ���ӣ�Ӱ��ͼ��������
//=��  �룺
//======================================================//
cv::Mat RemoveFringeFrame(cv::Mat matImageIn, U16 wOffsetX, U16 wOffsetY, float fOffsetAlpha)
{
	cv::Mat matIndex = cv::Mat::zeros(10,10,CV_16U);

	return matIndex;
}

