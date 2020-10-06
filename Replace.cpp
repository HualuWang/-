// Replace.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "document.h"
#include "Replace.h"

using namespace std;
using namespace cv;

// CReplace �Ի���

IMPLEMENT_DYNAMIC(CReplace, CDialogEx)

CReplace::CReplace(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIG_REPLACE, pParent)
	, m_FilePath(_T(""))
	, m_Number(0)
{

}

CReplace::~CReplace()
{
}

void CReplace::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FilePath, m_FilePath);
	DDX_Text(pDX, IDC_Number, m_Number);
}


BEGIN_MESSAGE_MAP(CReplace, CDialogEx)
	ON_BN_CLICKED(IDC_Choose, &CReplace::OnBnClickedChoose)
END_MESSAGE_MAP()


// CReplace ��Ϣ�������


BOOL CReplace::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(L"��ֵ�滻");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CReplace::OnBnClickedChoose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	BOOL bRet = FALSE;
	TCHAR szFolder[MAX_PATH * 2];
	szFolder[0] = _T('/0');
	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
			bRet = TRUE;
		IMalloc *pMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
	}
	m_FilePath = szFolder;//ѡ����ļ���·�� 
	//MessageBox(m_FilePath);
	UpdateData(FALSE);
}
