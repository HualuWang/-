#pragma once


// CCrack �Ի���

class CCrack : public CDialogEx
{
	DECLARE_DYNAMIC(CCrack)

public:
	CCrack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCrack();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CRACK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_Small;
	int m_Big;
	virtual BOOL OnInitDialog();
};
