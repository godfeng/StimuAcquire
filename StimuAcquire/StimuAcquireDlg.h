
// StimuAcquireDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"
#include "opencv2/opencv.hpp"
#include "SystemParam.h"
#include "ImageProcHandle.h"

// CStimuAcquireDlg �Ի���
class CStimuAcquireDlg : public CDialogEx
{
// ����
public:
	CStimuAcquireDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STIMUACQUIRE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
