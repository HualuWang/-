// Crack.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "document.h"
#include "Crack.h"
#include "afxdialogex.h"


// CCrack �Ի���

IMPLEMENT_DYNAMIC(CCrack, CDialogEx)

CCrack::CCrack(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_CRACK, pParent)
	, m_Small(0)
	, m_Big(0)
{

}

CCrack::~CCrack()
{
}

void CCrack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SMALL, m_Small);
	DDX_Text(pDX, IDC_EDIT_BIG, m_Big);
}


BEGIN_MESSAGE_MAP(CCrack, CDialogEx)
END_MESSAGE_MAP()


// CCrack ��Ϣ�������


BOOL CCrack::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(L"��ȡ����");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
