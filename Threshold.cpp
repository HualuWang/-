// Threshold.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "document.h"
#include "Threshold.h"
#include "afxdialogex.h"


// CThreshold �Ի���

IMPLEMENT_DYNAMIC(CThreshold, CDialogEx)

CThreshold::CThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_EDGE, pParent)
	, m_Threshold(0)
	, m_open(0)
{

}

CThreshold::~CThreshold()
{
}

void CThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EDGE, m_Threshold);
	DDX_Text(pDX, IDC_EDIT_OPEN, m_open);
}


BEGIN_MESSAGE_MAP(CThreshold, CDialogEx)
END_MESSAGE_MAP()


// CThreshold ��Ϣ�������


BOOL CThreshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(L"��Ե���");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
