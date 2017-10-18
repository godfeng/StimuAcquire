
// StimuAcquireDlg.h : 头文件
//

#pragma once

#include "afxcmn.h"
#include "opencv2/opencv.hpp"
#include "SystemParam.h"
#include "ImageProcHandle.h"

// CStimuAcquireDlg 对话框
class CStimuAcquireDlg : public CDialogEx
{
// 构造
public:
	CStimuAcquireDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_STIMUACQUIRE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnStart();
	afx_msg void OnBnSnap();
	afx_msg void OnBnRecord();
	afx_msg void OnBnCalibrat();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckAutoExposeTime();
	afx_msg void OnCheckHorizFlip();
	afx_msg void OnCheckVertFlip();
	afx_msg void OnBnRadioColor();
	afx_msg LRESULT OnMsgCamevent(WPARAM wp, LPARAM lp);
	afx_msg void OnCbnSelchangeComboPreview();
	afx_msg void OnDestroy();
	afx_msg void OnBnWhiteBalance();
	DECLARE_MESSAGE_MAP()
public:
	CImageProcHandle	*pImageProcHandle;
	CSystemParam		*pSystemParam;
	//--------------//
	HToupCam			hCameraHandle;
	cv::VideoWriter		*pCvVideoWriter;
	void*				m_pImageData;
	BITMAPINFOHEADER	m_BmpInfo;
	//--------------//
	bool				bSystemIsRunning;
	bool				m_bDeviceIsOpen;
	//--------------//
	unsigned int uiWidth;
	unsigned int uiHeight;

	bool OpenDevice();
	void CloseDevice();
	void InitialSystem();

	void OnEventError();
	void OnEventDisconnected();
	void OnEventImage();
	void OnEventExpo();
	void OnEventTempTint();
	void OnEventStillImage();

};
