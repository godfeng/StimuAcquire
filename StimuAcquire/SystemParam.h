/************************************************************/
/*			������
/*
/************************************************************/
#ifndef _SYSTEM_PARAM_H_
#define _SYSTEM_PARAM_H_

//================�궨��=========================//
#define _WithCcdDeivce_

#define Api_Sucess	0
#define Api_Failed  1

//-----��������------
#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned int

//-----�������------
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
	//---- Ԥ��ͼ���С ----//
	unsigned int	uiPrevImageWidth;
	unsigned int	uiPrevImageHeight;

	//---- �ɼ�ͼ���С ----//
	unsigned int	uiAcquImageWidth;
	unsigned int	uiAcquImageHeight;

	//---- CCD�������� ----//
	unsigned int	uiExposeTime;
	unsigned int	uiGain;

	//---- ͼ���Ż� ------//
	float			fContrast;
	float			fBrightness;
	int				ifOffsetX;
	int				ifOffsetY;
};

#endif