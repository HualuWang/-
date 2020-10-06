
// documentView.cpp : CdocumentView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "document.h"
#endif

#include"DEPTH.h"
#include"Other.h"
#include "Replace.h"
#include "TwoDimension.h"
#include "Crack.h"
#include "Threshold.h"
#include "documentDoc.h"
#include "documentView.h"

#define Pi (double) 3.1415926
#define thred 100

#include "mclmcr.h"
#include "afxdialogex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"		// add header file

// import necessary lib??
#pragma comment( lib, "libeng.lib")
#pragma comment( lib, "libmx.lib")
#pragma comment( lib,"libmat.lib")



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int Max[1200][1600] = { 0 };			//���ڴ��������˹�任�����ֵ
int high[1200][1600] = { 0 };			//���ڴ��ϵ��ͼ������к�
int Lap[1200][1600] = { 0 };			//���ڱ���Ҷ�ֵ
int newLap[1200][1600] = { 0 };		//���ڱ���������˹�任��ľ۽�ֵ
float newLap_sum[69] = { 0 };			//���ڱ���ÿ��ͼ��۽�ֵ֮��

// CdocumentView

IMPLEMENT_DYNCREATE(CdocumentView, CView)

BEGIN_MESSAGE_MAP(CdocumentView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CdocumentView::OnFileOpen)
	ON_COMMAND(IDM_EDGE, &CdocumentView::OnEdge)
	ON_COMMAND(IDM_CRACK, &CdocumentView::OnCrack)
	ON_COMMAND(IDM_PARAMETERS, &CdocumentView::OnParameters)
	ON_COMMAND(IDM_REPLACE, &CdocumentView::OnReplace)
	ON_COMMAND(IDM_DEPTH, &CdocumentView::OnDepth)
	ON_COMMAND(IDM_3DREST, &CdocumentView::On3drest)
	ON_COMMAND(IDM_FUSE, &CdocumentView::OnFuse)
	ON_COMMAND(IDM_OTHER, &CdocumentView::OnOther)
END_MESSAGE_MAP()

// CdocumentView ����/����

CdocumentView::CdocumentView()
	: m_threshold(300)
	, Flag(0)
	, m_open(2)
	, m_number(0)
	, m_Filepath(_T(""))
	, m_Point_x(0)
	, m_Point_y(0)
{
	// TODO: �ڴ˴���ӹ������
	m_small = 0;
	m_big = 0;
	m_SrcImg = NULL;
}

CdocumentView::~CdocumentView()
{
}

BOOL CdocumentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CdocumentView ����

void CdocumentView::OnDraw(CDC* pDC)
{
	CdocumentDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (m_EntName.Compare(_T("bmp")) == 0)      //bmp��ʽ
	{
		ShowBitmap(pDC, m_BmpName);               //��ʾͼƬ
	}
}


// CdocumentView ��ӡ

BOOL CdocumentView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CdocumentView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CdocumentView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CdocumentView ���

#ifdef _DEBUG
void CdocumentView::AssertValid() const
{
	CView::AssertValid();
}

void CdocumentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdocumentDoc* CdocumentView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdocumentDoc)));
	return (CdocumentDoc*)m_pDocument;
}
#endif //_DEBUG


// CdocumentView ��Ϣ�������


void CdocumentView::ShowBitmap(CDC* pDC, CString BmpName)
{
	//����bitmapָ�� ���ú���LoadImageװ��λͼ
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

	if (m_Cbitmap.m_hObject)
	{
		m_Cbitmap.Detach();				 //�ж�CWnd�ʹ�����ϵ
	}
	m_Cbitmap.Attach(m_hBitmap);		 //�����HBITMAP m_hBitmap��CBitmap m_bitmap����

	//�߽�
	CRect rect;
	GetClientRect(&rect); 

	//���岢����һ���ڴ��豸����DC
	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))   //���������Ե�DC
		return;

	BITMAP bmp;                           //��ʱbmpͼƬ����
	m_Cbitmap.GetBitmap(&bmp);            //��ͼƬ����λͼ��

	CBitmap *pbmpOld = NULL;
	dcBmp.SelectObject(&m_Cbitmap);        //��λͼѡ����ʱ�ڴ��豸����

	//ͼƬ��ʾ���ú���stretchBlt
	pDC->StretchBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &dcBmp, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	dcBmp.SelectObject(pbmpOld);           //�ָ���ʱDC��λͼ
	DeleteObject(&m_Cbitmap);              //ɾ���ڴ��е�λͼ
	dcBmp.DeleteDC();                      //ɾ��CreateCompatibleDC�õ���ͼƬDC
}

void CdocumentView::OnFileOpen()
{
	// TODO: �ڴ���������������
	//���ָ�ʽ���ļ���bmp gif jpg tiff
	CString filter;
	filter = "�����ļ�(*.bmp,*.jpg,*.gif,*tiff)|*.bmp;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//����ȷ����ť dlg.DoModal() ������ʾ�Ի���
	if (dlg.DoModal() == IDOK)
	{
		m_BmpName = dlg.GetPathName();     //��ȡ�ļ�·����   ��D:\pic\abc.bmp
		m_EntName = dlg.GetFileExt();      //��ȡ�ļ���չ��
		CString FileName = dlg.GetFileName();	 //��ȡ�ļ���
		m_FilePath=dlg.GetFolderPath();
		m_EntName.MakeLower();             //���ļ���չ��ת��Ϊһ��Сд�ַ�
		Invalidate();                    //���øú����ͻ����OnDraw�ػ滭ͼ
		USES_CONVERSION;				 //���ȫ�ֱ���m_srcimage
		m_SrcImg = cv::imread(W2A(m_BmpName), 1);
		AfxGetMainWnd()->SetWindowText(L"�ĵױ�����������ϵͳ-" + FileName);
	}
}

void CdocumentView::OnEdge()
{
	// TODO: �ڴ���������������
	//�ж��Ƿ��Ѿ���ͼƬ
	if (m_SrcImg.empty())
	{
		MessageBox(TEXT("���ȴ�һ��Ҫ�����ͼ��"));
		return;
	}
	double d[8] = { 0 };			//���ڴ��sobel���ӵ��м����
	CThreshold Dlg;					//����Ի��������
	cv::String FilePath = m_FilePath+"/Threshold.bmp";
	Dlg.m_Threshold = m_threshold;
	Dlg.m_open = m_open;
	if (IDOK == Dlg.DoModal())
	{
		m_threshold = Dlg.m_Threshold;
		m_open = Dlg.m_open;
		m_imgcols = m_SrcImg.cols;
		m_imgrows = m_SrcImg.rows;
		cv::cvtColor(m_SrcImg, m_GrayImg, CV_BGR2GRAY);

		//��ȡÿһ�����ص�ĻҶ�ֵ
		for (int i = 0; i<m_imgcols; i++)
			for (int j = 0; j < m_imgrows; j++)
			{
				original[i][j] = (int)m_GrayImg.at<uchar>(j, i);
			}

		//sobel��������
		for (int i = 0; i<m_imgcols; i++)
			for (int j = 0; j < m_imgrows; j++)
			{
				d[0] = pow((5 * original[i - 1][j - 1] + 5 * original[i - 1][j] + 5 * original[i - 1][j + 1] - 3 * original[i][j - 1] 
					- 3 * original[i][j + 1] - 3 * original[i + 1][j - 1] - 3 * original[i + 1][j] - 3 * original[i + 1][j + 1]), 2);		//Sobel���ӵ�dx[��ֱ�ݶ�]
				d[1] = pow(((-3) * original[i - 1][j - 1] + 5 * original[i - 1][j] + 5 * original[i - 1][j + 1] - 3 * original[i][j - 1] 
					+ 5 * original[i][j + 1] - 3 * original[i + 1][j - 1] - 3 * original[i + 1][j] - 3 * original[i + 1][j + 1]), 2);		//Sobel���ӵ�dy[ˮƽ�ݶ�]
				d[2] = pow(((-3) * original[i - 1][j - 1] - 3 * original[i - 1][j] + 5 * original[i - 1][j + 1] - 3 * original[i][j - 1] 
					+ 5 * original[i][j + 1] - 3 * original[i + 1][j - 1] - 3 * original[i + 1][j] + 5 * original[i + 1][j + 1]), 2);
				d[3] = pow(((-3) * original[i - 1][j - 1] - 3 * original[i - 1][j] - 3 * original[i - 1][j + 1] - 3 * original[i][j - 1] 
					+ 5 * original[i][j + 1] - 3 * original[i + 1][j - 1] + 5 * original[i + 1][j] + 5 * original[i + 1][j + 1]), 2);
				d[4] = pow(((-3) * original[i - 1][j - 1] - 3 * original[i - 1][j] - 3 * original[i - 1][j + 1] - 3 * original[i][j - 1] 
					- 3 * original[i][j + 1] + 5 * original[i + 1][j - 1] + 5 * original[i + 1][j] + 5 * original[i + 1][j + 1]), 2);
				d[5] = pow(((-3) * original[i - 1][j - 1] - 3 * original[i - 1][j] - 3 * original[i - 1][j + 1] + 5 * original[i][j - 1] 
					- 3 * original[i][j + 1] + 5 * original[i + 1][j - 1] + 5 * original[i + 1][j] - 3 * original[i + 1][j + 1]), 2);
				d[6] = pow((5 * original[i - 1][j - 1] - 3 * original[i - 1][j] - 3 * original[i - 1][j + 1] + 5 * original[i][j - 1] 
					- 3 * original[i][j + 1] + 5 * original[i + 1][j - 1] - 3 * original[i + 1][j] - 3 * original[i + 1][j + 1]), 2);
				d[7] = pow((5 * original[i - 1][j - 1] + 5 * original[i - 1][j] - 3 * original[i - 1][j + 1] + 5 * original[i][j - 1] 
					- 3 * original[i][j + 1] - 3 * original[i + 1][j - 1] - 3 * original[i + 1][j] - 3 * original[i + 1][j + 1]), 2);
				result[i][j] = sqrt(d[0] + d[1] + d[2] + d[3] + d[4] + d[5] + d[6] + d[7]);
				if (result[i][j] > m_threshold)
					m_GrayImg.at<uchar>(j, i) = 255;
				else
					m_GrayImg.at<uchar>(j, i) = 0;
			}

		//�ȸ�ʴ�����ʹ����������
		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(m_open, m_open));
		erode(m_GrayImg, m_GrayImg, element);
		dilate(m_GrayImg, m_GrayImg, element);

		/*cv::Mat element1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
		dilate(imgGray, imgGray, element1);
		erode(imgGray, imgGray, element1);*/

		//ͼ����ʾ��Ϊ�˷��������ʾ����
		Mat OutImage = ~m_GrayImg;
		cv::imwrite(FilePath, OutImage);
		USES_CONVERSION;
		std::string filepath = FilePath;
		m_BmpName = A2W(filepath.c_str());
		Invalidate();
	}
}


void CdocumentView::OnCrack()
{
	// TODO: �ڴ���������������
	if (m_GrayImg.empty())
	{
		MessageBox(TEXT("�������ͼ��ı�Ե���"));
		return;
	}
	cv::String FilePath =m_FilePath+ "/Crack.bmp";
	CCrack Dlg;
	Dlg.m_Small = m_small;
	Dlg.m_Big = m_big;
	if (IDOK==Dlg.DoModal())
	{
		vector<vector<Point>>contours(10000);
		vector<Vec4i>hierarchy(10000);
		m_small = Dlg.m_Small;
		m_big = Dlg.m_Big;
		m_CrackImg = Mat(m_imgrows, m_imgcols, CV_8UC3, Scalar(0, 0, 0));
		Mat image1 = Mat(m_imgrows, m_imgcols, CV_8UC3, Scalar(0, 0, 0));
		Mat image2 = Mat(m_imgrows, m_imgcols, CV_8UC3, Scalar(0, 0, 0));
		Mat image3 = Mat(m_imgrows, m_imgcols, CV_8UC3, Scalar(0, 0, 0));
		Mat image4 = Mat(m_imgrows, m_imgcols, CV_8UC1, Scalar(0, 0, 0));
		Mat out = m_GrayImg;
		findContours(out, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
		Scalar color = Scalar(255, 255, 0);
		
		//��þ�Ƭ�ڵ���������
		int MaxArea = 0,MaxID=0;
		for (int i = 0; i < hierarchy.size(); i++)
		{
			int area = (int)contourArea(contours[i], true);
			if (area > 0)
			{
				drawContours(image1, contours, i, color, 2, 8, hierarchy, 1);
				drawContours(image2, contours, i, color, 2, 8, hierarchy, 0);
				absdiff(image1, image2, image3);
			}
			if (MaxArea < area)
			{
				MaxID = i;
				MaxArea = area;
			}
		}
		Rect boundRect;
		boundRect = boundingRect(Mat(contours[MaxID]));
		m_Point_x = boundRect.x + boundRect.width / 2;
		m_Point_y = boundRect.y + boundRect.height / 2;
		//CString STemp;
		//STemp.Format(_T("%d  %d"), boundRect.x+ boundRect.width/2, boundRect.y + boundRect.height / 2);
		//MessageBox(STemp);
		//���contours��hierarchy�ڵ�����
		vector<vector<Point>>().swap(contours);
		vector<Vec4i>().swap(hierarchy);
		cv::cvtColor(image3, image3, CV_RGB2GRAY);
		threshold(image3, image4, 20, 255, THRESH_BINARY);
		findContours(image4, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		//ȥ����Ƭ�ڵ����
		if (m_big == 0)
		{
			for (int i = 0; i < hierarchy.size(); i++)
			{
				int area = (int)contourArea(contours[i]);
				if (area > m_small)
				{
					drawContours(m_CrackImg, contours, i, color, -1, 8, hierarchy);
				}
			}
		}
		else if (m_small > m_big)
		{
			MessageBox(TEXT("С��ͨ��ֵ��С�ڴ���ͨ��ֵ"));
			return;
		}
		else
		{
			for (int i = 0; i < hierarchy.size(); i++)
			{
				int area = (int)contourArea(contours[i]);
				if (area > m_small && area < m_big)
				{
					drawContours(m_CrackImg, contours, i, color, -1, 8, hierarchy);
				}
			}
		}
		//Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		//dilate(m_CrackImg, m_CrackImg, element);
		//erode(image5, image5, element);

		//���contours��hierarchy�ڵ�����
		vector<vector<Point>>().swap(contours);
		vector<Vec4i>().swap(hierarchy);
		Mat OutImage = ~m_CrackImg;
		cv::imwrite(FilePath, OutImage);
		USES_CONVERSION;
		std::string filepath = FilePath;
		m_BmpName = A2W(filepath.c_str());
		Invalidate();
		//���contours��hierarchy�ڵ����ݣ�Ч������
		//contours.clear();
		//hierarchy.clear();
	}
}


void CdocumentView::OnParameters()
{
	// TODO: �ڴ���������������
	if (m_CrackImg.empty())
	{
		MessageBox(TEXT("�������ͼ�����ȡ����"));
		return;
	}
	CTwoDimension Dlg;
	vector<vector<Point>>contours(10000);
	vector<Vec4i>hierarchy(10000);
	Rect boundRect;			//������Ӿ���
	RotatedRect box;		//������С��Ӿ���
	Point2f rect[4];
	cv::String FilePath = m_FilePath + "/Parameter.bmp";
	Mat image5 = Mat(m_imgrows, m_imgcols, CV_8UC1, Scalar(0, 0, 0));
	m_ParaImg = Mat(m_imgrows, m_imgcols, CV_8UC3, Scalar(0, 0, 0));
	Scalar color = Scalar(255, 255, 0);
	cv::cvtColor(m_CrackImg, m_CrackImg, CV_RGB2GRAY);
	threshold(m_CrackImg, image5, 20, 255, THRESH_BINARY);
	findContours(image5, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < hierarchy.size(); i++)
	{
		int area = (int)contourArea(contours[i]);
		drawContours(m_ParaImg, contours, i, color, -1, 8, hierarchy);
		box = minAreaRect(Mat(contours[i]));				//����������С��Ӿ���
		boundRect = boundingRect(Mat(contours[i]));
		circle(m_ParaImg, Point((int)(box.center.x), (int)(box.center.y)), 5, Scalar(0, 255, 0), -1, 8);
		rectangle(m_ParaImg, Point(boundRect.x, boundRect.y), Point(boundRect.x + boundRect.width, boundRect.y + boundRect.height), Scalar(0, 255, 0), 2, 8);
		//cout << "��" << i << "����������Ӿ��ζ������꣨" << boundRect.x << "��" << boundRect.y << "��" << endl;
		//cout << "��" << i << "����������Ӿ��γ���" << boundRect.width << " " << boundRect.height << endl;
		if (boundRect.width < boundRect.height)
			Dlg.m_lwr = (double)(boundRect.width)/ boundRect.height;
		else
			Dlg.m_lwr = (double)(boundRect.height) / boundRect.width;
		box.points(rect);  //����С��Ӿ����ĸ��˵㸴�Ƹ�rect����
		for (int j = 0; j<4; j++)
		{
			//cout << "��" << i << "�������������С���ζ������꣨" << rect[j].x << "��" << rect[j].y << "��" << endl;
			line(m_ParaImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);	//������С��Ӿ���ÿ����
		}
		double a = sqrt((rect[0].x- rect[1].x)*(rect[0].x - rect[1].x)+ (rect[0].y - rect[1].y)*(rect[0].y - rect[1].y));
		double b = sqrt((rect[1].x - rect[2].x)*(rect[1].x - rect[2].x) + (rect[1].y - rect[2].y)*(rect[1].y - rect[2].y));
		double S = a*b;
		if (a > b)
		{
			if (rect[0].x < rect[1].x)
				Dlg.m_dirrecttion=(double)atan((double)(rect[1].y - rect[0].y) / (rect[1].x - rect[0].x));
			else
				Dlg.m_dirrecttion = (double)atan((double)(rect[0].y - rect[1].y) / (rect[0].x - rect[1].x));
		}
		else
		{
			if (rect[1].x < rect[2].x)
				Dlg.m_dirrecttion = (double)atan((rect[2].y - rect[1].y) / (rect[2].x - rect[1].x));
			else
				Dlg.m_dirrecttion = (double)atan((rect[1].y - rect[2].y) / (rect[1].x - rect[2].x));
		}
		Dlg.m_rect = area / S;
		int length = (int)arcLength(contours[i], true);
		Dlg.m_length = length/2;
		Dlg.m_circle = 4 * PI*area / length / length;
		Dlg.m_position.Format(_T("(%d,%d)"), boundRect.x - m_Point_x,- boundRect.y + m_Point_y);
		//cout << "��" << i << "�����������Ϊ" << area << endl;
		//cout << "��" << i << "���������ܳ�Ϊ" << length << endl;
			
		/*�����ƻ������ֱ�� �������㳤�Ȳ�׼ȷ*/
		/*cv::Vec4f line_para;
		cv::fitLine(contours[i], line_para, cv::DIST_L2, 0, 1e-2, 1e-2);
		//��ȡ��бʽ�ĵ��б��  
		cv::Point point0;
		point0.x = line_para[2];
		point0.y = line_para[3];
		double k = line_para[1] / line_para[0];
		//����ֱ�ߵĶ˵�(y = k(x - x0) + y0)  
		cv::Point point1, point2;
		point1.x = 0;
		point1.y = k * (0 - point0.x) + point0.y;
		point2.x = 1292;
		point2.y = k * (1292 - point0.x) + point0.y;
		cv::line(image7, point1, point2, cv::Scalar(0, 255, 0), 2, 8, 0);*/
	}
	Dlg.m_image = m_ParaImg;

	//���contours��hierarchy�ڵ�����
	vector<vector<Point>>().swap(contours);
	vector<Vec4i>().swap(hierarchy);

	//����ͼ����ʾ
	Mat OutImage = ~m_ParaImg;
	cv::imwrite(FilePath, OutImage);
	USES_CONVERSION;
	std::string filepath = FilePath;
	m_BmpName = A2W(filepath.c_str());
	Invalidate();
	Dlg.DoModal();
}


void CdocumentView::OnReplace()
{
	// TODO: �ڴ���������������
	CReplace Dlg;
	Dlg.m_Number = m_number;
	Dlg.m_FilePath = m_Filepath;
	if (IDOK==Dlg.DoModal())
	{
		m_number = Dlg.m_Number;
		m_Filepath = Dlg.m_FilePath;
		int s = 3;
		string filepath;
		USES_CONVERSION;
		filepath = W2A(Dlg.m_FilePath);
		for (UINT number= 0; number <= m_number; number++)		 //ͼ����������Ϊ00*.bmp
		{
			ostringstream oss;
			oss << filepath << "\\" << setw(4) << setfill('0') << number << ".bmp";//�ļ�����·��
			Mat src = imread(oss.str(), 0);
			for (int i = 0; i < 1200; i++)					//��ȡÿһ���ص�ĻҶ�ֵ
				for (int j = 0; j < 1600; j++)
				{
					Lap[i][j] = (int)src.at<uchar>(i, j);
				}

			for (int i = 5; i < 1200 - 5; i++)				//��ȡÿһ���ص�������˹�任��ĻҶ�ֵ
				for (int j = 5; j < 1600 - 5; j++)
				{
					newLap[i][j] = abs(2 * Lap[i][j] - Lap[i - s][j] - Lap[i + s][j]) + abs(2 * Lap[i][j] - Lap[i][j - s] - Lap[i][j + s]);
					newLap_sum[number] += newLap[i][j];		//��������
				}
			for (int i = 0; i < 1200; i++)					//�Ƚ�ÿһ���ص�ĻҶ�ֵ���ڷ�ֵ��֮ǰͼ��ĻҶ�ֵ�����滻
				for (int j = 0; j < 1600; j++)
				{
					if (Max[i][j] < newLap[i][j] && newLap[i][j]>10)		//�趨��ֵ
					{
						Max[i][j] = newLap[i][j];
						high[i][j] = number;
					}
				}
			src.release();
		}
	}
}


void CdocumentView::OnDepth()
{
	 //TODO: �ڴ���������������
	if (m_Filepath.IsEmpty())
	{
		MessageBox(TEXT("����ѡ���ļ���"));
		return;
	}
	CDepth Dlg;
	string filepath;
	USES_CONVERSION;
	filepath = W2A(m_Filepath);
	char buf[200];
	strcpy(buf, filepath.c_str());
	strcat(buf, "/y1.txt");
	for (int i = (int)strlen(buf); i > 0; i--)
		if (buf[i] == ':')
			buf[i + 1] = '/';
		else if (buf[i] == '\\')
			buf[i + 1] = '/';
		else
			buf[i + 1] = buf[i];
	ofstream outfile1(buf);//����ļ�·��
	strcpy(buf, filepath.c_str());
	strcat(buf, "/y2.txt");
	for (int i = (int)strlen(buf); i > 0; i--)
		if (buf[i] == ':')
			buf[i + 1] = '/';
		else if (buf[i] == '\\')
			buf[i + 1] = '/';
		else
			buf[i + 1] = buf[i];
	ofstream outfile2(buf);//����ļ�·��
	int max_layer = 0;
	int min_layer = 0;
	for (uint i = 0; i<m_number; i++)
	{
		if (newLap_sum[max_layer] < newLap_sum[i])
		{
			max_layer = i;
		}
		outfile2 << newLap_sum[i] << '\t' << (newLap_sum[i + 3] - newLap_sum[i]) / 3 << '\n';
	}
	outfile2 << "������˹�任���ֵ���ڲ���" << max_layer << '\n';
	int sum;
	for (int i = 6; i<1200 - 6; i++)
		for (int j = 6; j<1600 - 6; j++)
			if (high[i][j] == 0)
			{
				int c = 0;
				sum = 0;
				for (int a = i - 1; a <= i + 1; a++)
					for (int b = j - 1; b <= j + 1; b++)
						if (high[a][b] != 0)
						{
							sum = sum + high[a][b];
							c++;
						}
				if (c != 0)
					high[i][j] = (int)sum / c;
			}
	int temp = 0;
	//�����ѷ�ĵ�������λ��
	for (UINT t = max_layer; t <= m_number; t++)
		for (int i = 6; i<1200 - 6; i++)
		{
			for (int j = 6; j<1600 - 6; j++)
			{
				sum = 0;
				if (high[i][j] == t)
				{
					for (int a = i - 2; a <= i + 2; a++)
						for (int b = j - 2; b <= j + 2; b++)
							sum = sum + abs(high[a][b] - high[i][j]);
						if (sum <= 80)
						{
							outfile2 << t << "\t" << i << "\t" << j << '\t' << high[i][j] << '\n';
							min_layer = t;
							temp = 1;
						}
						break;
				}
			}
			if (temp == 1)
			{
				temp = 0;
				break;
			}
		}
	outfile2 << buf << '\n';
	outfile2 << endl;
	for (int i = 5; i<1200 - 5; i++)
	{
		for (int j = 5; j<1600 - 5; j++)
			outfile1 << 250 - high[i][j] << '\t';   //Ԫ�ؼ����ˮƽ�Ʊ�λ
		outfile1 << endl;                    //ÿ�м�һ�س�
	}
	float highacc[256];
	int layer=0;
	strcpy(buf, filepath.c_str());
	strcat(buf, "/highacc.txt");
	for (int i = (int)strlen(buf); i > 0; i--)
		if (buf[i] == ':')
			buf[i + 1] = '/';
		else if (buf[i] == '\\')
			buf[i + 1] = '/';
		else
			buf[i + 1] = buf[i];
	ifstream infile(buf);//�����ļ�·��
	if (infile.is_open()) {
		std::string tmp;
		while (getline(infile, tmp)) {
			std::string::const_iterator p, q = tmp.begin(), end = tmp.end();
			while ((p = find_if(q, end, isdigit)) != end) {
				q = find_if_not(p, end, isdigit);
				highacc[layer++] = stoul(tmp.substr(distance(tmp.cbegin(), p), distance(p, q)));
			}
		}
	}
	Dlg.m_depth = (highacc[min_layer]-highacc[max_layer])/10.0;
	Dlg.DoModal();
}


void CdocumentView::On3drest()
{
	// TODO: �ڴ���������������
	if (m_Filepath.IsEmpty())
	{
		MessageBox(TEXT("����ѡ���ļ���"));
		return;
	}
	char buf[200];
	string filepath;
	USES_CONVERSION;
	filepath = W2A(m_Filepath);
	strcpy(buf, filepath.c_str());
	strcat(buf, "/y1.txt");
	for (int i = (int)strlen(buf); i > 0; i--)
		if (buf[i] == ':')
			buf[i + 1] = '/';
		else if (buf[i] == '\\')
			buf[i + 1] = '/';
		else
			buf[i + 1] = buf[i];
	Engine* ep;
	mxArray *x1 = NULL;
	mxArray *y1 = NULL;
	if ((ep = engOpen("")) == NULL)
	{
		return;
	}
	//double *x = new double[m_number + 1];
	//double *y = new double[m_number + 1];
	double x[250], y[250];
	for (UINT i = 0; i <= m_number; i++)
	{
		x[i] = i;
		y[i] = newLap_sum[i];
	}
	x1 = mxCreateDoubleMatrix(1, m_number, mxREAL);
	y1 = mxCreateDoubleMatrix(1, m_number, mxREAL);

	memcpy((void *)mxGetPr(x1), (void *)x, 8 * (m_number + 1));
	memcpy((void *)mxGetPr(y1), (void *)y, 8 * (m_number + 1));
	engPutVariable(ep, "x", x1);
	engPutVariable(ep, "y", y1);
	engEvalString(ep, "figure(2)");
	engEvalString(ep, "plot(x,y)");
	engEvalString(ep, "title('������˹֮��')");
	for (UINT i = 0; i <= m_number; i++)
	{
		x[i] = i;
		if (i <= m_number - 3)
			y[i] = (newLap_sum[i + 3] - newLap_sum[i]) / 3;
		else
			y[i] = 0;
	}
	memcpy((void *)mxGetPr(x1), (void *)x, 8 * (m_number + 1));
	memcpy((void *)mxGetPr(y1), (void *)y, 8 * (m_number + 1));
	engPutVariable(ep, "x", x1);
	engPutVariable(ep, "y", y1);
	engEvalString(ep, "figure(3)");
	engEvalString(ep, "plot(x,y)");
	engEvalString(ep, "title('��')");
	//CString filepath = _T("C:\\Users\\Administrator.XJB-01810111549\\Desktop\\�ĵ�ȱ�ݼ�����\\04\\y1.txt"); 
	//engEvalString(ep, "z=textread('F://������ά���/02/y1.txt')");
	engEvalString(ep, "z=textread(buf)");
	engEvalString(ep, "h=fspecial('average',[9 9])");
	engEvalString(ep, "z1=filter2(h,z)");
	engEvalString(ep, "figure(4)");
	engEvalString(ep, "mesh(z1)");
	engEvalString(ep, "title('��ά�ع�')");
	engEvalString(ep, "axis tight");
	//engClose(ep);
}

char buf[200];
void CdocumentView::OnFuse()
{
	// TODO: �ڴ���������������
	string filepath;
	USES_CONVERSION;
	filepath = W2A(m_FilePath);
	strcpy(buf, filepath.c_str());
	strcat(buf, "/850X-3.bmp");
	for (int i = (int)strlen(buf); i > 0; i--)
		if (buf[i] == ':')
			buf[i + 1] = '/';
		else if (buf[i] == '\\')
			buf[i + 1] = '/';
		else
			buf[i + 1] = buf[i];
	m_BmpName = buf;
	Invalidate();
}

Vec4i GetRect(vector<Point> &contours)
{
	int xmin, xmax, ymin, ymax;
	xmin = xmax = contours[0].x;//xΪ��ȷ���yΪ�߶ȷ���
	ymin = ymax = contours[0].y;
	for (int i = 0; i < contours.size(); i++)
	{
		xmin = (xmin < contours[i].x ? xmin : contours[i].x);
		xmax = (xmax > contours[i].x ? xmax : contours[i].x);
		ymin = (ymin < contours[i].y ? ymin : contours[i].y);
		ymax = (ymax > contours[i].y ? ymax : contours[i].y);
	}
	return Vec4i(xmin, xmax, ymin, ymax);
}

double Dis_point2Line(Point2f center, Point p1, Point p2)
{
	double dis = 0;
	if (p1.x == p2.x)
	{
		return abs((double)(center.x - p1.x));
	}
	if (p1.y == p2.y)
	{
		return abs((double)(center.y - p1.y));
	}
	double  k = (double)((p1.y - p2.y) / (double)(p1.x - p2.x));
	double  b = p1.y - k*p1.x; //ֱ�߹�ʽΪ kx-y+b=0;
	dis = abs((double)(k*center.x - center.y + b)) / sqrt(k*k + 1);
	return dis;
}

void CdocumentView::OnOther()
{
	// TODO: �ڴ���������������
	COther Dlg;
	cv::String FilePath = m_FilePath + "/Hough.bmp";
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat srcImage, grayImage, thredImage, midImage;
	//srcImage = imread("C://Users//Administrator.XJB-01810111549//Desktop//850X-3.bmp");
	srcImage = imread(buf);
	if (srcImage.empty())
	{
		/*cout << "error" << endl;*/
		/*break;*/
		MessageBox(TEXT("����ͼƬʧ��"));
		return;
	}
	cvtColor(srcImage, grayImage, COLOR_RGB2GRAY);
	threshold(grayImage, thredImage, thred, 255, THRESH_BINARY_INV);
	Mat drawImage = thredImage.clone();
	findContours(thredImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
	Mat drawing = Mat::zeros(srcImage.size(), CV_8UC1);
	Mat outImage(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(255, 255, 255));
	double maxlen = 0;
	int index = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(drawing, contours, i, 255, 1, 8, hierarchy, 1, Point(0, 0));
		drawContours(outImage, contours, i, Scalar(0,0,0), 5, 8, hierarchy, 1, Point(0, 0));
		double len = arcLength(contours[i], true);
		if (maxlen<len)
		{
			maxlen = len;
			index = i;
		}
	}
	/*cout << "����ܳ���" << maxlen << endl;
	cout << "������ţ�" << index << endl;*/
	Dlg.m_perimeter = maxlen;
	Dlg.m_no = index;

	vector<Vec2f> lines;
	HoughLines(drawing, lines, 1, CV_PI / 15, 40, 0, 0);
	cvtColor(drawing, drawing, COLOR_GRAY2BGR);
	Mat dstImage;
	cvtColor(drawImage, dstImage, COLOR_GRAY2BGR);
	/*for(size_t i = 0; i < lines.size(); i++)*/
	
	float rho = lines[5][0];
	/*cout << "ԭ����ֱ�ߵľ��룺" << rho << endl;*/
	Dlg.m_distance = rho;
	float theta = lines[5][1];
	Point pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a * rho, y0 = b * rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));
	double a1 = ((double)(pt1.y - pt2.y) / (pt1.x - pt2.x));
	double b1 = atan(-a1);
	double angle = b1 * 180 / Pi;
	line(outImage, pt1, pt2, Scalar(0, 0, 255), 4, CV_AA);
	/*cout << "�Ƕ�:" << angle << endl;
	cout << "ֱ�ߵ������˵�;" << endl;*/
	/*cout << pt1 << endl;
	cout << pt2 << endl;*/
	Dlg.m_angle = angle;
	Dlg.m_endpoint.Format(_T("(%d,%d)��(%d,%d)"), pt1.x , pt1.y, pt2.x, pt2.y);
	/*imshow("�����߱任", drawing);
	cout << i<< endl;
	waitKey(0);*/

	/*�ظ����룬��modelʹ��*/
	rho = lines[8][0];
	Dlg.m_distance1 = rho;
	theta = lines[8][1];
	a = cos(theta), b = sin(theta);
	x0 = a * rho, y0 = b * rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));
	a1 = ((double)(pt1.y - pt2.y) / (pt1.x - pt2.x));
	b1 = atan(-a1);
	angle = b1 * 180 / Pi;
	line(outImage, pt1, pt2, Scalar(0, 0, 255), 4, CV_AA);
	Dlg.m_angle1 = angle;
	Dlg.m_endpoint1.Format(_T("(%d,%d)��(%d,%d)"), pt1.x, pt1.y, pt2.x, pt2.y);
	/*�ظ����룬��modelʹ��*/

	double maxc1 = 0;
	for (int i = 0; i < contours[99].size(); i++)
	{
		double c1 = Dis_point2Line((Point2f)contours[99][i], Point(-168, 1308), Point(1319, -30));
		if (maxc1<c1)
		{
			maxc1 = c1;
			index = i;
		}
	}
	/*cout << "��ֵ�����:" << maxc1 << endl;
	cout << "��ֵ������:" << (Point)contours[99][index] << endl;*/
	Dlg.m_peak = maxc1;
	Point p = (Point)contours[99][index];
	Dlg.m_coordinate.Format(_T("(%d,%d)"), p.x, p.y);
	circle(outImage, p, 5, Scalar(0, 0, 255), -1);
	cv::imwrite(FilePath, ~outImage);
	USES_CONVERSION;
	std::string filepath = FilePath;
	m_BmpName = A2W(filepath.c_str());
	Invalidate();
	Dlg.DoModal();
}