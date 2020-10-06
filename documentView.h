
// documentView.h : CdocumentView ��Ľӿ�
//

#pragma once
#include "gdiplusheaders.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>

#define PI 3.1415926

using namespace std;
using namespace cv;


class CdocumentView : public CView
{
protected: // �������л�����
	CdocumentView();
	DECLARE_DYNCREATE(CdocumentView)

// ����
public:
	CdocumentDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CdocumentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnEdge();
	afx_msg void OnCrack();
private:
	UINT m_threshold;
	int m_small, m_big;
	int Flag;
public:
	//��Ա����
	CString m_BmpName;                               //����ͼ���ļ��ļ���
	CString m_EntName;                               //����ͼ���ļ���չ��
	CBitmap m_Cbitmap;                               //����λͼ����
	CString m_FilePath;
	Mat m_SrcImg;
	Mat m_GrayImg;
	Mat m_CrackImg;
	Mat m_ParaImg;
	int m_imgcols, m_imgrows;
	int original[1292][964];
	double result[1292][964];
	//��Ա����
	void ShowBitmap(CDC* pDC, CString BmpName);
private:
	UINT m_open;
	UINT m_number;
public:
	afx_msg void OnParameters();
	afx_msg void OnReplace();
	afx_msg void OnDepth();
	CString m_Filepath;
	afx_msg void On3drest();
private:
	int m_Point_x;
	int m_Point_y;
public:
	afx_msg void OnFuse();
	afx_msg void OnOther();
};

#ifndef _DEBUG  // documentView.cpp �еĵ��԰汾
inline CdocumentDoc* CdocumentView::GetDocument() const
   { return reinterpret_cast<CdocumentDoc*>(m_pDocument); }
#endif

