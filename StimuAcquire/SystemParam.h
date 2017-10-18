/************************************************************/
/*			属性类
/*
/************************************************************/
#ifndef _SYSTEM_PARAM_H_
#define _SYSTEM_PARAM_H_

//================宏定义=========================//
#define _WithCcdDeivce_

#define Api_Sucess	0
#define Api_Failed  1

//-----数据类型------
#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned int

//-----相机属性------
#define SetImageWidth			0x1001
#define SetImageHeight			0x1002
#define SetExposureTime			0x1003
#define SetValueGain			0x1004
#define SetAutoExposure			0x1005
#define SetColorMode			0x1006
//================================================//

class CSystemParam
{
public:
	CSystemParam();
	~CSystemParam();
public:
	//---- 预览图像大小 ----//
	unsigned int	uiPrevImageWidth;
	unsigned int	uiPrevImageHeight;

	//---- 采集图像大小 ----//
	unsigned int	uiAcquImageWidth;
	unsigned int	uiAcquImageHeight;

	//---- CCD属性设置 ----//
	unsigned int	uiExposeTime;
	unsigned int	uiGain;

	//---- 图像优化 ------//
	float			fContrast;
	float			fBrightness;
	int				ifOffsetX;
	int				ifOffsetY;
};

#endif