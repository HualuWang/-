#pragma once


// CThreshold �Ի���

class CThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CThreshold)

public:
	CThreshold(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThreshold();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EDGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UINT m_Threshold;
	UINT m_open;
	virtual BOOL OnInitDialog();
};
