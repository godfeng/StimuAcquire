
// StimuAcquireDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StimuAcquire.h"
#include "StimuAcquireDlg.h"
#include "afxdialogex.h"
#include <InitGuid.h>
#include <wincodec.h>
#include <wmsdkidl.h>
#include <stdlib.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MSG_CAMEVENT			(WM_APP + 1)

static BOOL SaveImageByWIC(const wchar_t* strFilename, const void* pData, const BITMAPINFOHEADER* pHeader)
{
	GUID guidContainerFormat;
	if (PathMatchSpec(strFilename, L"*.bmp"))
		guidContainerFormat = GUID_ContainerFormatBmp;
	else if (PathMatchSpec(strFilename, L"*.jpg"))
		guidContainerFormat = GUID_ContainerFormatJpeg;
	else if (PathMatchSpec(strFilename, L"*.png"))
		guidContainerFormat = GUID_ContainerFormatPng;
	else
		return FALSE;

	CComPtr<IWICImagingFactory> spIWICImagingFactory;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (LPVOID*)&spIWICImagingFactory);
	if (FAILED(hr))
		return FALSE;

	CComPtr<IWICBitmapEncoder> spIWICBitmapEncoder;
	hr = spIWICImagingFactory->CreateEncoder(guidContainerFormat, NULL, &spIWICBitmapEncoder);
	if (FAILED(hr))
		return FALSE;

	CComPtr<IWICStream> spIWICStream;
	spIWICImagingFactory->CreateStream(&spIWICStream);
	if (FAILED(hr))
		return FALSE;

	hr = spIWICStream->InitializeFromFilename(strFilename, GENERIC_WRITE);
	if (FAILED(hr))
		return FALSE;

	hr = spIWICBitmapEncoder->Initialize(spIWICStream, WICBitmapEncoderNoCache);
	if (FAILED(hr))
		return FALSE;

	CComPtr<IWICBitmapFrameEncode> spIWICBitmapFrameEncode;
	CComPtr<IPropertyBag2> spIPropertyBag2;
	hr = spIWICBitmapEncoder->CreateNewFrame(&spIWICBitmapFrameEncode, &spIPropertyBag2);
	if (FAILED(hr))
		return FALSE;

	if (GUID_ContainerFormatJpeg == guidContainerFormat)
	{
		PROPBAG2 option = { 0 };
		option.pstrName = L"ImageQuality"; /* jpg quality, you can change this setting */
		CComVariant varValue(0.75f);
		spIPropertyBag2->Write(1, &option, &varValue);
	}
	hr = spIWICBitmapFrameEncode->Initialize(spIPropertyBag2);
	if (FAILED(hr))
		return FALSE;

	hr = spIWICBitmapFrameEncode->SetSize(pHeader->biWidth, pHeader->biHeight);
	if (FAILED(hr))
		return FALSE;

	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat24bppBGR;
	hr = spIWICBitmapFrameEncode->SetPixelFormat(&formatGUID);
	if (FAILED(hr))
		return FALSE;

	hr = spIWICBitmapFrameEncode->WritePixels(pHeader->biHeight, TDIBWIDTHBYTES(pHeader->biWidth * pHeader->biBitCount), TDIBWIDTHBYTES(pHeader->biWidth * pHeader->biBitCount) * pHeader->biHeight, (BYTE*)pData);
	if (FAILED(hr))
		return FALSE;

	hr = spIWICBitmapFrameEncode->Commit();
	if (FAILED(hr))
		return FALSE;
	hr = spIWICBitmapEncoder->Commit();
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

// CStimuAcquireDlg 对话框

CStimuAcquireDlg::CStimuAcquireDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStimuAcquireDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStimuAcquireDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStimuAcquireDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Bn_Start, &CStimuAcquireDlg::OnBnStart)
	ON_BN_CLICKED(IDC_Bn_Snap, &CStimuAcquireDlg::OnBnSnap)
	ON_BN_CLICKED(IDC_Bn_Record, &CStimuAcquireDlg::OnBnRecord)
	ON_BN_CLICKED(IDC_Bn_Calibrat, &CStimuAcquireDlg::OnBnCalibrat)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_AutoExpose, &CStimuAcquireDlg::OnCheckAutoExposeTime)
	ON_BN_CLICKED(IDC_CHECK_HorizFlip, &CStimuAcquireDlg::OnCheckHorizFlip)
	ON_BN_CLICKED(IDC_CHECK_VertFlip, &CStimuAcquireDlg::OnCheckVertFlip)
	ON_BN_CLICKED(IDC_RADIO_COLOR, &CStimuAcquireDlg::OnBnRadioColor)
	ON_BN_CLICKED(IDC_RADIO_GREY, &CStimuAcquireDlg::OnBnRadioColor)
	ON_CBN_SELCHANGE(IDC_COMBO_Preview, &CStimuAcquireDlg::OnCbnSelchangeComboPreview)
	ON_MESSAGE(MSG_CAMEVENT, &CStimuAcquireDlg::OnMsgCamevent)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_WHITEBALNC, &CStimuAcquireDlg::OnBnWhiteBalance)
END_MESSAGE_MAP()


// CStimuAcquireDlg 消息处理程序

BOOL CStimuAcquireDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);
	  
	SkinH_Attach();
	//-------- 控件初始化 ---------//
	CComboBox * pComboSelector = NULL;
	pComboSelector = (CComboBox *)GetDlgItem(IDC_COMBO_Preview);
	pComboSelector->InsertString(0, _T("640x480"));
	pComboSelector->InsertString(1, _T("1280x960"));
	pComboSelector->InsertString(2, _T("2592x1944"));
	pComboSelector->SetCurSel(0);

	pComboSelector = (CComboBox *)GetDlgItem(IDC_COMBO_Acquire);
	pComboSelector->InsertString(0, _T("640x480"));
	pComboSelector->InsertString(1, _T("1280x960"));
	pComboSelector->InsertString(2, _T("2592x1944"));
	pComboSelector->SetCurSel(2);

	((CButton*)GetDlgItem(IDC_RADIO_COLOR))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_AutoExpose))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_SLD_ExposeTime))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_SLD_ValueGain))->EnableWindow(FALSE);
	//-------- 重载类初始化 ---------//
	pSystemParam = new CSystemParam;
	pImageProcHandle = new CImageProcHandle;
	//-------- 变量初始化 ---------//
	memset(&m_BmpInfo, 0, sizeof(m_BmpInfo));
	m_BmpInfo.biSize = sizeof(m_BmpInfo);
	m_BmpInfo.biPlanes = 1;
	m_BmpInfo.biBitCount = 24;
	//-------- 变量初始化 ---------//
	if (OpenDevice())
	{
		AfxMessageBox(_T("当前系统没找到相机设备"));
	}
	InitialSystem();

	return TRUE; 
}

void CStimuAcquireDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CStimuAcquireDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CStimuAcquireDlg::OnMsgCamevent(WPARAM wp, LPARAM /*lp*/)
{
	switch (wp)
	{
	case TOUPCAM_EVENT_ERROR:
		OnEventError();
		break;
	case TOUPCAM_EVENT_DISCONNECTED:
		OnEventDisconnected();
		break;
	case TOUPCAM_EVENT_IMAGE:
		OnEventImage();
		break;
	case TOUPCAM_EVENT_EXPOSURE:
		OnEventExpo();
		break;
	case TOUPCAM_EVENT_TEMPTINT:
		OnEventTempTint();
		break;
	case TOUPCAM_EVENT_STILLIMAGE:
		OnEventStillImage();
		break;
	default:
		break;
	}
	return 0;
}

//======================================================//
//=函数功能：打开摄像头设备
//=输入/出参数：无
//======================================================//
bool CStimuAcquireDlg::OpenDevice()
{
#ifdef _WithCcdDeivce_
	if (hCameraHandle)
		return Api_Sucess;

	hCameraHandle = Toupcam_Open(NULL);
	if (hCameraHandle == NULL)
	{
		m_bDeviceIsOpen = false;
		return Api_Failed;
	}
#endif

	m_bDeviceIsOpen = true;
	return Api_Sucess;
}

//======================================================//
//=函数功能：关闭摄像头设备
//=输入/出参数：无
//======================================================//
void CStimuAcquireDlg::CloseDevice()
{
#ifdef _WithCcdDeivce_
	if (hCameraHandle)
		Toupcam_Close(hCameraHandle);
#endif

	m_bDeviceIsOpen = false;
}

//======================================================//
//=函数功能：初始化相机并关联界面控件
//=输入/出参数：无
//======================================================//
void CStimuAcquireDlg::InitialSystem()
{
#ifdef _WithCcdDeivce_
	//--获取当前图像大小--
	int nWidth = 0, nHeight = 0;
	HRESULT hr = Toupcam_get_Size(hCameraHandle, &nWidth, &nHeight);
	if (FAILED(hr))
		return;
	//--初始化保存文件头--
	m_BmpInfo.biWidth = nWidth;
	m_BmpInfo.biHeight = nHeight;
	m_BmpInfo.biSizeImage = TDIBWIDTHBYTES(nWidth * 24) * nHeight;
	if (m_pImageData){
		free(m_pImageData);
		m_pImageData = NULL;
	}
	m_pImageData = malloc(m_BmpInfo.biSizeImage);

	//--获取曝光时间范围--
	unsigned nMinExpTime, nMaxExpTime, nDefExpTime;
	Toupcam_get_ExpTimeRange(hCameraHandle, &nMinExpTime, &nMaxExpTime, &nDefExpTime);
	((CSliderCtrl*)GetDlgItem(IDC_SLD_ExposeTime))->SetRange(nMinExpTime, nMaxExpTime);

	//--是否自动曝光--
	BOOL bEnableAutoExpo = ((CButton*)GetDlgItem(IDC_CHECK_AutoExpose))->GetCheck();
	Toupcam_get_AutoExpoEnable(hCameraHandle, &bEnableAutoExpo);

	//----------------------------------//
	((CSliderCtrl*)GetDlgItem(IDC_SLD_Temp))->SetRange(2000, 15000);
	((CSliderCtrl*)GetDlgItem(IDC_SLD_Tint))->SetRange(200, 2500);
#endif
}

//======================================================//	
//=函数功能：启动或者停止采集
//=输入/出参数：无
//======================================================//
void CStimuAcquireDlg::OnBnStart()
{
	if (!bSystemIsRunning) 
	{
		bSystemIsRunning = true;
		SetDlgItemText(IDC_Bn_Start, _T("停止"));
		//----------------------------------//
		Toupcam_StartPullModeWithWndMsg(hCameraHandle, m_hWnd, MSG_CAMEVENT);
		OnEventExpo();
		OnEventTempTint();
	}
	else
	{
		bSystemIsRunning = false;
		Sleep(30);
		CloseDevice();
		SetDlgItemText(IDC_Bn_Start, _T("启动"));
	}
}

//======================================================//
//=		捕捉当前帧图像，并按时间点保存图像
//=
//======================================================//
void CStimuAcquireDlg::OnBnSnap()
{
	int n = Toupcam_get_StillResolutionNumber(hCameraHandle);
	if (n <= 0)
	{
		unsigned e = 0;
		Toupcam_get_eSize(hCameraHandle, &e);
		Toupcam_Snap(hCameraHandle, e);
	}
	else
	{
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.CreatePopupMenu();
		TCHAR text[64];
		int w, h;
		for (int i = 0; i < n; ++i)
		{
			Toupcam_get_StillResolution(hCameraHandle, i, &w, &h);
			swprintf_s(text, _T("%d * %d"), w, h);
			menu.AppendMenu(MF_STRING, IDM_SNAP_RESOLUTION + i, text);
		}
		menu.TrackPopupMenu(TPM_RIGHTALIGN, pt.x, pt.y, this);
	}
}

//======================================================//
//=		开始或者停止录制视频
//=
//======================================================//
void CStimuAcquireDlg::OnBnRecord()
{
	
}

//======================================================//
//=		开始或者停止录制视频
//=
//======================================================//
void CStimuAcquireDlg::OnBnCalibrat()
{
	//SetCalibrateMat();
}

//======================================================//
//=		水平滚动条
//======================================================//
void CStimuAcquireDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString strT;
	int nCtrlID = pScrollBar->GetDlgCtrlID();
	switch (nCtrlID)
	{
	case IDC_SLD_ExposeTime:{
								int nCurPos = ((CSliderCtrl*)GetDlgItem(IDC_SLD_ExposeTime))->GetPos();
								strT.Format(_T("%d"), nCurPos);
								GetDlgItem(IDC_STC_ExposeTime)->SetWindowText(strT);
								break;
	}
	case IDC_SLD_ValueGain:{
							   int nCurPos = ((CSliderCtrl*)GetDlgItem(IDC_SLD_ValueGain))->GetPos();
							   strT.Format(_T("%d"), nCurPos);
							   GetDlgItem(IDC_STC_ValueGain)->SetWindowText(strT);
							   break;
	}
	case IDC_SLD_OffsetX:{
							 int nCurPos = ((CSliderCtrl*)GetDlgItem(IDC_SLD_OffsetX))->GetPos();
							 strT.Format(_T("%d"), nCurPos);
							 GetDlgItem(IDC_STC_OffsetX)->SetWindowText(strT);
							 break;
	}
	case IDC_SLD_OffsetY:{
							 int nCurPos = ((CSliderCtrl*)GetDlgItem(IDC_SLD_OffsetY))->GetPos();
							 strT.Format(_T("%d"), nCurPos);
							 GetDlgItem(IDC_STC_OffsetY)->SetWindowText(strT);
							 break;
	}
	case IDC_SLD_OffsetAlpha:{
								 int nCurPos = ((CSliderCtrl*)GetDlgItem(IDC_SLD_OffsetAlpha))->GetPos();
								 strT.Format(_T("%d"), nCurPos);
								 GetDlgItem(IDC_STC_OffsetAlpha)->SetWindowText(strT);
								 break;
	}
	default:
		break;
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

//======================================================//
//=		自动曝光切换
//======================================================//
void CStimuAcquireDlg::OnCheckAutoExposeTime()
{
	int bAutoExposeIsCheck = ((CButton*)GetDlgItem(IDC_CHECK_AutoExpose))->GetCheck();
	if (bAutoExposeIsCheck)
	{
		GetDlgItem(IDC_SLD_ExposeTime)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLD_ValueGain)->EnableWindow(FALSE);
		if (hCameraHandle)
			Toupcam_put_AutoExpoEnable(hCameraHandle, TRUE);
	}
	else
	{
		GetDlgItem(IDC_SLD_ExposeTime)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLD_ValueGain)->EnableWindow(TRUE);
		if (hCameraHandle)
			Toupcam_put_AutoExpoEnable(hCameraHandle, FALSE);
	}
}

//======================================================//
//=		白平衡切换
//======================================================//
void CStimuAcquireDlg::OnBnWhiteBalance()
{
	if (hCameraHandle)
		Toupcam_AwbOnePush(hCameraHandle, NULL, NULL);
}

//======================================================//
//=		是否水平翻转
//======================================================//
void CStimuAcquireDlg::OnCheckHorizFlip()
{
	// TODO:  在此添加控件通知处理程序代码
}

//======================================================//
//=		是否垂直翻转
//======================================================//
void CStimuAcquireDlg::OnCheckVertFlip()
{
	// TODO:  在此添加控件通知处理程序代码
}

void CStimuAcquireDlg::OnBnRadioColor()
{
	int bIsShowColor = ((CButton*)GetDlgItem(IDC_RADIO_COLOR))->GetCheck();
	if (bIsShowColor)
	{

	}
	else
	{

	}
}

//======================================================//
//=		预览图像尺寸切换
//======================================================//
void CStimuAcquireDlg::OnCbnSelchangeComboPreview()
{
	if (NULL == hCameraHandle)
		return;

	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_Preview);
	int nSel = pCombox->GetCurSel();
	if (nSel < 0)
		return;
	unsigned nResolutionIndex = 0;
	HRESULT hr = Toupcam_get_eSize(hCameraHandle, &nResolutionIndex);
	if (FAILED(hr))
		return;

	if (nResolutionIndex != nSel)
	{
		hr = Toupcam_Stop(hCameraHandle);
		if (FAILED(hr))
			return;

		Toupcam_put_eSize(hCameraHandle, nSel);

		InitialSystem();
	}
}

//======================================================//
//=		预览图像尺寸切换
//======================================================//
void CStimuAcquireDlg::OnEventDisconnected()
{
	if (hCameraHandle)
	{
		Toupcam_Close(hCameraHandle);
		hCameraHandle = NULL;
	}
	AfxMessageBox(_T("The camera is disconnected, mybe has been pulled out."));
}

//======================================================//
//=相机事件消息：	相机运行错误
//======================================================//
void CStimuAcquireDlg::OnEventError()
{
	if (hCameraHandle)
	{
		Toupcam_Close(hCameraHandle);
		hCameraHandle = NULL;
	}
	AfxMessageBox(_T("Error"));
}

//======================================================//
//=相机事件消息：	相机曝光时间自动修改
//======================================================//
void CStimuAcquireDlg::OnEventExpo()
{
	unsigned nTime = 0;
	Toupcam_get_ExpoTime(hCameraHandle, &nTime);
	SetDlgItemInt(IDC_STC_ExposeTime, nTime);
	((CSliderCtrl*)GetDlgItem(IDC_SLD_ExposeTime))->SetPos(nTime);
}

//======================================================//
//=相机事件消息：	相机白平衡参数自动修改
//======================================================//
void CStimuAcquireDlg::OnEventTempTint()
{
	int nTemp = TOUPCAM_TEMP_DEF, nTint = TOUPCAM_TINT_DEF;
	Toupcam_get_TempTint(hCameraHandle, &nTemp, &nTint);
	SetDlgItemInt(IDC_STC_Temp, nTemp, TRUE);
	SetDlgItemInt(IDC_STC_Tint, nTint, TRUE);
	((CSliderCtrl*)GetDlgItem(IDC_SLD_Temp))->SetPos(nTemp);
	((CSliderCtrl*)GetDlgItem(IDC_SLD_Tint))->SetPos(nTint);
}

//======================================================//
//=相机事件消息：	单帧图像采集完成响应
//======================================================//
void CStimuAcquireDlg::OnEventImage()
{
	HRESULT hr = Toupcam_PullImage(hCameraHandle, m_pImageData, 24, NULL, NULL);
	if (SUCCEEDED(hr))
	{
		CWnd *pWnd = GetDlgItem(IDC_STATIC_SHOW);
		CDC *pDC = pWnd->GetDC();
		CRect rc;
		pWnd->GetClientRect(&rc);
		rc.left += 4;
		rc.top += 4;
		rc.bottom -= 4;
		rc.right -= 4;

		int m = pDC->SetStretchBltMode(COLORONCOLOR);
		StretchDIBits(pDC->GetSafeHdc()
			, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top
			, 0, 0, m_BmpInfo.biWidth, m_BmpInfo.biHeight
			, m_pImageData, (BITMAPINFO*)&m_BmpInfo
			, DIB_RGB_COLORS, SRCCOPY);
		pDC->SetStretchBltMode(m);
	}
}

//======================================================//
//=相机事件消息：	单帧图像采集完成响应
//======================================================//
void CStimuAcquireDlg::OnEventStillImage()
{
	unsigned w = 0, h = 0;
	HRESULT hr = Toupcam_PullStillImage(hCameraHandle, NULL, 24, &w, &h);
	if (SUCCEEDED(hr))
	{
		void* pData = malloc(TDIBWIDTHBYTES(w * 24) * h);
		hr = Toupcam_PullStillImage(hCameraHandle, pData, 24, NULL, NULL);
		if (SUCCEEDED(hr))
		{
			BITMAPINFOHEADER header = { 0 };
			header.biSize = sizeof(header);
			header.biPlanes = 1;
			header.biBitCount = 24;
			header.biWidth = w;
			header.biHeight = h;
			header.biSizeImage = TDIBWIDTHBYTES(header.biWidth * header.biBitCount) * header.biHeight;
			SaveImageByWIC(L"toupcamdemomfc.jpg", pData, &header);
		}
	}
}


void CStimuAcquireDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (hCameraHandle)
	{
		Toupcam_Close(hCameraHandle);
		hCameraHandle = NULL;
	}
	if (m_pImageData)
	{
		free(m_pImageData);
		m_pImageData = NULL;
	}
}



