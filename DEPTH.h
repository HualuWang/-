#pragma once


// CDEPTH �Ի���

class CDepth : public CDialogEx
{
	DECLARE_DYNAMIC(CDepth)

public:
	CDepth(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDepth();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DEPTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_depth;
	virtual BOOL OnInitDialog();
};
