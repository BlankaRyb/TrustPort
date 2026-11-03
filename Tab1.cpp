// Tab1.cpp : implementation file
//

#include "stdafx.h"
#include "tab.h"
#include "Tab1.h"

// CTab1 dialog

IMPLEMENT_DYNAMIC(CTab1, CDialog)

CTab1::CTab1(CWnd* pParent /*=NULL*/)
	: CDialog(CTab1::IDD, pParent)
{
	m_1pozice = 7;
	m_iPage = m_iLine = 0;
}

CTab1::~CTab1()
{
}

void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR3, m_ScrollBarMy);
	DDX_Control(pDX, IDC_BUTTON6, m_butt);
}

BEGIN_MESSAGE_MAP(CTab1, CDialog)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON6, &CTab1::OnBnClickedButton6)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTab1 message handlers

BOOL CTab1::OnInitDialog()
{
	CRect rc;
	CString str;
//	int i;
//	CClickText TextClick1;

	CDialog::OnInitDialog();

//	m_TextClick1.SetWindowPos(NULL,0,0,100,100,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
//	m_TextClick1.SetWindowPos(NULL,0,0,100,100, SWP_NOZORDER);

	//Rozbalovaci Texty
/*	m_TextClick1.SetIcon(IDI_SIPKA1, IDI_SIPKA2);
	m_TextClick2.SetIcon(IDI_SIPKA1, IDI_SIPKA2);
	m_TextClick3.SetIcon(IDI_SIPKA1, IDI_SIPKA2);
	m_TextClick4.SetIcon(IDI_SIPKA1, IDI_SIPKA2);
	m_TextClick5.SetIcon(IDI_SIPKA1, IDI_SIPKA2);
	m_TextClick1.SetText(IDS_STRING31, IDS_STRING32);
	m_TextClick2.SetText(IDS_STRING11, IDS_STRING12);
	m_TextClick3.SetText(IDS_STRING31, IDS_STRING32);
	m_TextClick4.SetText(IDS_STRING21, IDS_STRING22);
	m_TextClick5.SetText(IDS_STRING11, IDS_STRING12);
	m_TextClick1.SetWidth(200);
	m_TextClick2.SetWidth(200);
	m_TextClick3.SetWidth(200);
	m_TextClick4.SetWidth(200);
	m_TextClick5.SetWidth(200);
	m_TextClick1.SetIntervalBut(10);
	m_TextClick2.SetIntervalBut(10);
	m_TextClick3.SetIntervalBut(10);
	m_TextClick4.SetIntervalBut(10);
	m_TextClick5.SetIntervalBut(10);
	m_TextClick1.SetSizeIco(15);
	m_TextClick2.SetSizeIco(15);
	m_TextClick3.SetSizeIco(15);
	m_TextClick4.SetSizeIco(15);
	m_TextClick5.SetSizeIco(15);
	m_TextClick1.SetPosText(CPoint(40,0));
	m_TextClick2.SetPosText(CPoint(40,0));
	m_TextClick3.SetPosText(CPoint(40,0));
	m_TextClick4.SetPosText(CPoint(40,0));
	m_TextClick5.SetPosText(CPoint(40,0));
	//nasledujici az na nakonec
	//1. Calculation - vypocte obelnik buttonu
	//2. SetPos
	m_TextClick1.Calculation();
	m_TextClick2.Calculation();
	m_TextClick3.Calculation();
	m_TextClick4.Calculation();
	m_TextClick5.Calculation();
	m_TextClick1.SetPos(CPoint(220, m_1pozice));
	i = m_TextClick1.GetHeight()+m_TextClick1.GetInterval();
	m_TextClick2.SetPos(CPoint(220, i));
	i += m_TextClick2.GetHeight()+m_TextClick2.GetInterval();
	m_TextClick3.SetPos(CPoint(220, i));
	i += m_TextClick3.GetHeight()+m_TextClick3.GetInterval();
	m_TextClick4.SetPos(CPoint(220, i));
	i += m_TextClick4.GetHeight()+m_TextClick4.GetInterval();
	m_TextClick5.SetPos(CPoint(220, i));
	i += m_TextClick5.GetHeight()+m_TextClick4.GetInterval();*/

	m_butt.Add(IDS_STRING31, IDS_STRING32, IDI_SIPKA1, IDI_SIPKA2, IDD_ADD0);
	m_butt.Add(IDS_STRING11, IDS_STRING12, IDI_SIPKA1, IDI_SIPKA2, NULL);
	m_butt.Add(IDS_STRING31, IDS_STRING32, IDI_SIPKA1, IDI_SIPKA2, IDD_ADD0);
	m_butt.Add(IDS_STRING21, IDS_STRING22, IDI_SIPKA1, IDI_SIPKA2, IDD_ADD3);
	m_butt.Add(IDS_STRING11, IDS_STRING12, IDI_SIPKA1, IDI_SIPKA2, NULL);
	m_butt.SetWidth(440);
	m_butt.SetInterval(10);
	m_butt.SetSizeIco(15);
	m_butt.SetPosText(CPoint(40,0));
	m_butt.SetPosIco(CPoint(0,0));
	m_butt.Calculation();
	m_butt.SetPos(CPoint(10, m_1pozice));
	//nastaveni parametru

	str = _T("Vyplò text");
	m_butt.SetParam(0,IDC_EDIT1,str);
	str = _T("Zde je jiný text");
	m_butt.SetParam(2,IDC_EDIT1,str);

	//ScrollBar
	GetClientRect(&rc);
	m_iPage = rc.bottom;//pocet pixelu na stranku- vyska dialogu
	m_iLine = m_butt.GetSizeLine(); //pocet pixelu na radek pod nadpisem

	rc = m_butt.GetRectButt();
	SetScrollBarMy(rc.Height(), 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//SetPos v nasledujicich funkci musi byt volan i na aktualnim buttonu,
//aby doslo krepoctu jeho velikosti, te velikosti, ktera je pod textem

/*void CTab1::OnBnClickedButton1()
{
	CRect rc;
	int iHeightCh; //zmena vysky buttonu
	int iPozCh; //zmena pozice buttonu (kvuli zmene ScroolBaru)
	int iSuma; //iHeightCh+iPozCh
	int minpos;
	int maxpos;
	int iPozPuv;
	CPoint point;

	if (m_TextClick1.GetClick())
	{
		iHeightCh = m_TextClick1.GetChangeHeight();

		m_ScrollBarMy.GetScrollRange(&minpos, &maxpos); 

		iPozPuv = m_ScrollBarMy.GetScrollPos();
		SetScrollBarMy(maxpos+iHeightCh, iPozPuv);

		//sbaleno
		//prepocet kvuli ScroolBaru
		iPozCh = 0;
		if (iHeightCh<0)
		{
			iPozCh = iPozPuv-m_ScrollBarMy.GetScrollPos();
		}

		iSuma = iPozCh + iHeightCh;

		if (iPozCh)
		{
			point = m_TextClick1.GetPos();
			point.y += iPozCh;
			m_TextClick1.SetPos(point);
		}
		if (iSuma)
		{
			point = m_TextClick2.GetPos();
			point.y += iSuma;
			m_TextClick2.SetPos(point);
			point = m_TextClick3.GetPos();
			point.y += iSuma;
			m_TextClick3.SetPos(point);
			point = m_TextClick4.GetPos();
			point.y += iSuma;
			m_TextClick4.SetPos(point);
			point = m_TextClick5.GetPos();
			point.y += iSuma;
			m_TextClick5.SetPos(point);
		}
	}
	Invalidate(TRUE);
}

void CTab1::OnBnClickedButton2()
{
	CRect rc;
	int iHeightCh; //zmena vysky buttonu
	int iPozCh; //zmena pozice buttonu (kvuli zmene ScroolBaru)
	int iSuma; //iHeightCh+iPozCh
	int minpos;
	int maxpos;
	int iPozPuv;
	CPoint point;

	if (m_TextClick2.GetClick())
	{
		iHeightCh = m_TextClick2.GetChangeHeight();

		m_ScrollBarMy.GetScrollRange(&minpos, &maxpos); 

		iPozPuv = m_ScrollBarMy.GetScrollPos();
		SetScrollBarMy(maxpos+iHeightCh, iPozPuv);

		//sbaleno
		//prepocet kvuli ScroolBaru
		iPozCh = 0;
		if (iHeightCh<0)
		{
			iPozCh = iPozPuv-m_ScrollBarMy.GetScrollPos();
		}

		iSuma = iPozCh + iHeightCh;

		if (iPozCh != 0)
		{
			point = m_TextClick1.GetPos();
			point.y += iPozCh;
			m_TextClick1.SetPos(point);
			point = m_TextClick2.GetPos();
			point.y += iPozCh;
			m_TextClick2.SetPos(point);
		}
		if (iSuma != 0)
		{
			point = m_TextClick3.GetPos();
			point.y += iSuma;
			m_TextClick3.SetPos(point);
			point = m_TextClick4.GetPos();
			point.y += iSuma;
			m_TextClick4.SetPos(point);
			point = m_TextClick5.GetPos();
			point.y += iSuma;
			m_TextClick5.SetPos(point);
		}

	}
	Invalidate(TRUE);
}

void CTab1::OnBnClickedButton3()
{
	CRect rc;
	int iHeightCh; //zmena vysky buttonu
	int iPozCh; //zmena pozice buttonu (kvuli zmene ScroolBaru)
	int iSuma; //iHeightCh+iPozCh
	int minpos;
	int maxpos;
	int iPozPuv;
	CPoint point;

	if (m_TextClick3.GetClick())
	{
		iHeightCh = m_TextClick3.GetChangeHeight();

		m_ScrollBarMy.GetScrollRange(&minpos, &maxpos); 

		iPozPuv = m_ScrollBarMy.GetScrollPos();
		SetScrollBarMy(maxpos+iHeightCh, iPozPuv);

		//sbaleno
		//prepocet kvuli ScroolBaru
		iPozCh = 0;
		if (iHeightCh<0)
		{
			iPozCh = iPozPuv-m_ScrollBarMy.GetScrollPos();
		}

		iSuma = iPozCh + iHeightCh;

		if (iPozCh != 0)
		{
			point = m_TextClick1.GetPos();
			point.y += iPozCh;
			m_TextClick1.SetPos(point);
			point = m_TextClick2.GetPos();
			point.y += iPozCh;
			m_TextClick2.SetPos(point);
			point = m_TextClick3.GetPos();
			point.y += iPozCh;
			m_TextClick3.SetPos(point);
		}
		if (iSuma != 0)
		{
			point = m_TextClick4.GetPos();
			point.y += iSuma;
			m_TextClick4.SetPos(point);
			point = m_TextClick5.GetPos();
			point.y += iSuma;
			m_TextClick5.SetPos(point);
		}
	}
	Invalidate(TRUE);
}

void CTab1::OnBnClickedButton4()
{
	CRect rc;
	int iHeightCh; //zmena vysky buttonu
	int iPozCh; //zmena pozice buttonu (kvuli zmene ScroolBaru)
	int iSuma; //iHeightCh+iPozCh
	int minpos;
	int maxpos;
	int iPozPuv;
	CPoint point;

	if (m_TextClick4.GetClick())
	{
		iHeightCh = m_TextClick4.GetChangeHeight();

		m_ScrollBarMy.GetScrollRange(&minpos, &maxpos); 

		iPozPuv = m_ScrollBarMy.GetScrollPos();
		SetScrollBarMy(maxpos+iHeightCh, iPozPuv);

		//sbaleno
		//prepocet kvuli ScroolBaru
		iPozCh = 0;
		if (iHeightCh<0)
		{
			iPozCh = iPozPuv-m_ScrollBarMy.GetScrollPos();
		}

		iSuma = iPozCh + iHeightCh;

		if (iPozCh != 0)
		{
			point = m_TextClick1.GetPos();
			point.y += iPozCh;
			m_TextClick1.SetPos(point);
			point = m_TextClick2.GetPos();
			point.y += iPozCh;
			m_TextClick2.SetPos(point);
			point = m_TextClick3.GetPos();
			point.y += iPozCh;
			m_TextClick3.SetPos(point);
			point = m_TextClick4.GetPos();
			point.y += iPozCh;
			m_TextClick4.SetPos(point);
		}
		if (iSuma != 0)
		{
			point = m_TextClick5.GetPos();
			point.y += iSuma;
			m_TextClick5.SetPos(point);
		}
	}
	Invalidate(TRUE);
}

void CTab1::OnBnClickedButton5()
{
	CRect rc;
	int iHeightCh; //zmena vysky buttonu
	int iPozCh; //zmena pozice buttonu (kvuli zmene ScroolBaru)
//	int iSuma; //iHeightCh+iPozCh
	int minpos;
	int maxpos;
	int iPozPuv;
	CPoint point;

	if (m_TextClick5.GetClick())
	{
		iHeightCh = m_TextClick5.GetChangeHeight();

		m_ScrollBarMy.GetScrollRange(&minpos, &maxpos); 

		iPozPuv = m_ScrollBarMy.GetScrollPos();
		SetScrollBarMy(maxpos+iHeightCh, iPozPuv);

		//sbaleno
		//prepocet kvuli ScroolBaru
		iPozCh = 0;
		if (iHeightCh<0)
		{
			iPozCh = iPozPuv-m_ScrollBarMy.GetScrollPos();
		}

//		iSuma = iPozCh + iHeightCh;
		if (iPozCh != 0)
		{
			point = m_TextClick1.GetPos();
			point.y += iPozCh;
			m_TextClick1.SetPos(point);
			point = m_TextClick2.GetPos();
			point.y += iPozCh;
			m_TextClick2.SetPos(point);
			point = m_TextClick3.GetPos();
			point.y += iPozCh;
			m_TextClick3.SetPos(point);
			point = m_TextClick4.GetPos();
			point.y += iPozCh;
			m_TextClick4.SetPos(point);
			point = m_TextClick5.GetPos();
			point.y += iPozCh;
			m_TextClick5.SetPos(point);
		}
	}
	Invalidate(TRUE);
}*/

void CTab1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CPoint Point;
	WORD iwCode;
	int y, yPuv, yZmena;
//	int minpos;
//	int maxpos;
	int limit;

	yPuv = y = m_ScrollBarMy.GetScrollPos();
//	m_ScrollBarMy.GetScrollRange(&minpos, &maxpos); 
	limit = m_ScrollBarMy.GetScrollLimit();
	iwCode = MAKEWORD(-1, nSBCode);

	switch (HIBYTE(iwCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = limit;
		break;
	case SB_LINEUP:
		y -= m_iLine;
		break;
	case SB_LINEDOWN:
		y += m_iLine;
		break;
	case SB_PAGEUP:
		y -= m_iPage;
		break;
	case SB_PAGEDOWN:
		y += m_iPage;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	if (y<0)
	{
		y = 0;
	}
	if (y>limit)
	{
		y = limit;
	}

	yZmena = yPuv-y;
	if (yZmena != 0)
	{

		//Texty
		Point = m_butt.GetPos();
		m_butt.SetPos(CPoint(Point.x, Point.y+yZmena));
/*		Point = m_TextClick1.GetPos();
		m_TextClick1.SetPos(CPoint(Point.x, Point.y+yZmena));
		Point = m_TextClick2.GetPos();
		m_TextClick2.SetPos(CPoint(Point.x, Point.y+yZmena));
		Point = m_TextClick3.GetPos();
		m_TextClick3.SetPos(CPoint(Point.x, Point.y+yZmena));
		Point = m_TextClick4.GetPos();
		m_TextClick4.SetPos(CPoint(Point.x, Point.y+yZmena));
		Point = m_TextClick5.GetPos();
		m_TextClick5.SetPos(CPoint(Point.x, Point.y+yZmena));*/

		m_ScrollBarMy.SetScrollPos(y, TRUE);
		SetScrollPos(SB_VERT, y, TRUE);
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

//Nastavi ScrollBar
//iMax - Maximalni pozice ScrollBaru
//iPoz - Pozice ScrollBaru, pokud je prilis velka da tu ktera je nejvyssi mozna
void CTab1::SetScrollBarMy(int iMax, int iPoz)
{
	SCROLLINFO info;

	info.cbSize = sizeof(SCROLLINFO);     
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = iMax;
	info.nPage = m_iPage;
	info.nPos = iPoz;
	info.nTrackPos = 2;
	m_ScrollBarMy.SetScrollInfo(&info);
	if (iMax <= m_iPage)
	{
		m_ScrollBarMy.ShowScrollBar(FALSE);
	}
	else
	{
		m_ScrollBarMy.ShowScrollBar(TRUE);
	}
}
void CTab1::OnBnClickedButton6()
{
	CRect rc;
	int iHeightCh; //zmena vysky buttonu
	int iPozCh; //zmena pozice buttonu (kvuli zmene ScroolBaru)
	int iSuma; //iHeightCh+iPozCh
	int minpos;
	int maxpos;
	int iPozPuv;
	CPoint point;

	if (m_butt.GetClick())
	{
		iHeightCh = m_butt.GetChangeHeight();

		m_ScrollBarMy.GetScrollRange(&minpos, &maxpos); 

		iPozPuv = m_ScrollBarMy.GetScrollPos();
		SetScrollBarMy(maxpos+iHeightCh, iPozPuv);

		//sbaleno
		//prepocet kvuli ScroolBaru
		iPozCh = 0;
		if (iHeightCh<0)
		{
			iPozCh = iPozPuv-m_ScrollBarMy.GetScrollPos();
		}

		iSuma = iPozCh + iHeightCh;

		if (iPozCh)
		{
			point = m_butt.GetPos();
			point.y += iPozCh;
			m_butt.SetPos(point);
		}
	}
}

void CTab1::OnDestroy()
{
	CDialog::OnDestroy();

	CString str;

	str = m_butt.GetParam(0,IDC_EDIT1);
	str = m_butt.GetParam(2,IDC_EDIT1);
}