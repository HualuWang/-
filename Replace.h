#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include<math.h>
#include<stdlib.h>
#include <windows.h>
#include <opencv2/opencv.hpp>


// CReplace �Ի���


class CReplace : public CDialogEx
{
	DECLARE_DYNAMIC(CReplace)

public:
	CReplace(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReplace();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIG_REPLACE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_FilePath;
	UINT m_Number;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChoose();
};
