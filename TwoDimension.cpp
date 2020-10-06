// TwoDimension.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "document.h"
#include "TwoDimension.h"
#include "afxdialogex.h"


// CTwoDimension �Ի���

IMPLEMENT_DYNAMIC(CTwoDimension, CDialogEx)

CTwoDimension::CTwoDimension(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_PARA, pParent)
	, m_length(0)
	, m_dirrecttion(0)
	, m_lwr(0)
	, m_circle(0)
	, m_rect(0)
	, m_position(_T(""))
{

}

CTwoDimension::~CTwoDimension()
{
}

void CTwoDimension::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_length);
	DDX_Text(pDX, IDC_EDIT_DIRECTION, m_dirrecttion);
	DDX_Text(pDX, IDC_EDIT_LWR, m_lwr);
	DDX_Text(pDX, IDC_EDIT_CIRCLE, m_circle);
	DDX_Text(pDX, IDC_EDIT_RECT, m_rect);
	DDX_Text(pDX, IDC_EDIT_POSITION, m_position);
}


BEGIN_MESSAGE_MAP(CTwoDimension, CDialogEx)
END_MESSAGE_MAP()


// CTwoDimension ��Ϣ�������


BOOL CTwoDimension::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(L"��ά������ȡ");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
