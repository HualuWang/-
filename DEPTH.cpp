// DEPTH.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "document.h"
#include "DEPTH.h"
#include "afxdialogex.h"


// CDEPTH �Ի���

IMPLEMENT_DYNAMIC(CDepth, CDialogEx)

CDepth::CDepth(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_DEPTH, pParent)
	, m_depth(0)
{

}

CDepth::~CDepth()
{
}

void CDepth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEPTH, m_depth);
}


BEGIN_MESSAGE_MAP(CDepth, CDialogEx)
END_MESSAGE_MAP()


// CDEPTH ��Ϣ�������


BOOL CDepth::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(L"�����ȡ");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
