#pragma once


// COther �Ի���

class COther : public CDialogEx
{
	DECLARE_DYNAMIC(COther)

public:
	COther(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COther();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OTHER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	double m_perimeter;
	int m_no;
	double m_distance;
	double m_angle;
	double m_peak;
	CString m_coordinate;
	CString m_endpoint;
	double m_angle1;
	double m_distance1;
	CString m_endpoint1;
};
