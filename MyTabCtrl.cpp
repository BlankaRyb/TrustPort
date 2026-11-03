#include "StdAfx.h"
#include "MyTabCtrl.h"

CMyTabCtrl::CMyTabCtrl(void)
{
}

CMyTabCtrl::~CMyTabCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CMyTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CTabCtrl::OnPaint() for painting messages

	int iTab; //pocet zalozek
	int i;
	CRect rcPage, rcItem, rc;
	CFont fontText, *pOldFont;

	//Nastaveni fondu
	fontText.CreateFont(
			15,                        // nHeight
			0,                         // nWidth
			m_iWayText,                // angle of escapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			(LPCTSTR)"Arial CE");      // lpszFacename
	if (fontText.m_hObject)
	{
		pOldFont = dc.SelectObject(&fontText);
	}
	dc.SetBkColor(GetSysColor(COLOR_BTNFACE));

	iTab = GetItemCount();
	if(!iTab) return;

	GetClientRect(&rcPage);
/////////////////////////////////////////////////
/*	dc.SetBkColor(RGB(0xff, 0xff, 0));
	CBrush pBrush;
	CBrush *pOldBrush;

	//vypln pozadi
//	pBrush.CreateSolidBrush(GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	pBrush.CreateHatchBrush(HS_DIAGCROSS, RGB(255,0,0));

	pOldBrush = dc.SelectObject(&pBrush);
	dc.Rectangle(0,0,500,500);
	dc.SetDCBrushColor(RGB(0, 0, 0xff));
	pBrush.DeleteObject();
*/
/////////////////////////////////////////////////

	for( i = 0; i < iTab; i++)
	{
		if (!GetItemRect(i, &rcItem))
			return;
		if ((m_iTyp == 0) || (m_iTyp == 2))
		{	//vodorovne
			rc = GetRectNoneDialog(rcPage, m_iTyp, rcItem.Height(), 2);
		}
		else
		{	//svisle
			rc = GetRectNoneDialog(rcPage, m_iTyp, rcItem.Width(), 2);
		}
		DrawMyTabItem(&dc, i, rc, rcItem);
	}

	if (fontText.m_hObject)
	{
		dc.SelectObject(pOldFont);
		fontText.DeleteObject();
	}
//	dc.SelectObject(pOldBrush);
//	CTabCtrl::OnPaint();
}
void CMyTabCtrl::DrawMyTabItem(CDC* pDC, int ixItem, const CRect& rcPage, const CRect& rcItem)
{
	CPen pen, *pOldPen;
	COLORREF col;
	int iAktiv; // aktivni zalozka
	HICON hIcon;

	col = GetSysColor(COLOR_BTNFACE);
	pen.CreatePen (PS_SOLID, 1, col);

	if(pen.m_hObject)
	{
		pOldPen = pDC->SelectObject(&pen);
	}

	iAktiv=GetCurSel();

	switch(m_iTyp)
	{
		case 0:
			PaintItemTyp0(pDC, rcPage, rcItem, ixItem, iAktiv==ixItem);
		break;
		case 1:
			PaintItemTyp1(pDC, rcPage, rcItem, ixItem, iAktiv==ixItem);
		break;
		case 2:
			PaintItemTyp2(pDC, rcPage, rcItem, ixItem, iAktiv==ixItem);
		break;
		case 3:
			PaintItemTyp3(pDC, rcPage, rcItem, ixItem, iAktiv==ixItem);
		break;
		default:
			PaintItemTyp0(pDC, rcPage, rcItem, ixItem, iAktiv==ixItem);
		break;
	}
	
	hIcon = m_imageList.ExtractIcon(ixItem);
	if (hIcon)
	{
		pDC->DrawIcon(rcItem.left+(rcItem.Width()-32)/2, rcItem.top+10, hIcon);
		::DestroyIcon(hIcon);
	}
	if(pen.m_hObject)
	{
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	pen.DeleteObject();
	
}

//Kreslani zalozek 0. typu
//pDC - grafika
//rcPage - odelnik pro child dialog
//rcItem - odelnik Zalozky
//ixItem - poradova cislo zalozky, pocitano od 0
//bAktiv - true vybrana zalozka. false - nevybrana
void CMyTabCtrl::PaintItemTyp0(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv)
{
	CPen pen, pen1;

	if (bAktiv)
	{
		//vymazani cary pod neaktivnimi zalozkami
		pDC->MoveTo(rcPage.left,rcPage.top);
		pDC->LineTo(rcPage.right,rcPage.top);
	}
	else
	{
		//vymazani orazoveho zvyrazneni
		pDC->MoveTo(rcItem.left+m_arc-1,rcItem.top+1);
		pDC->LineTo(rcItem.right-m_arc+1,rcItem.top+1);
		pDC->MoveTo(rcItem.left+m_arc-2,rcItem.top+2);
		pDC->LineTo(rcItem.right-m_arc+2,rcItem.top+2);
	}

	pen.CreatePen (PS_SOLID, 1, SetColRect());
	if (pen.m_hObject)
	{
		pDC->SelectObject(&pen);
	}

	pDC->MoveTo(rcItem.right,rcItem.bottom);
	pDC->LineTo(rcItem.right,rcItem.top+m_arc);
	pDC->AngleArc(rcItem.right-m_arc, rcItem.top+m_arc, m_arc, 0, 90);
	pDC->LineTo(rcItem.left+m_arc,rcItem.top);
	pDC->AngleArc(rcItem.left+m_arc, rcItem.top+m_arc, m_arc, 90, 90);
	pDC->LineTo(rcItem.left,rcItem.bottom);
	if (bAktiv)
	{
		pDC->LineTo(rcPage.left,rcPage.top);
		pDC->LineTo(rcPage.left,rcPage.bottom);
		pDC->LineTo(rcPage.right,rcPage.bottom);
		pDC->LineTo(rcPage.right,rcPage.top);
		pDC->LineTo(rcItem.right,rcItem.bottom);
		//zvyrazneni aktivniho listu
		pen1.CreatePen (PS_SOLID, 1, SetColIsert());
		if (pen1.m_hObject)
		{
			pDC->SelectObject(&pen1);
		}
		pDC->MoveTo(rcItem.left+m_arc,rcItem.top);
		pDC->LineTo(rcItem.right-m_arc,rcItem.top);
		pDC->MoveTo(rcItem.left+m_arc-1,rcItem.top+1);
		pDC->LineTo(rcItem.right-m_arc+1,rcItem.top+1);
		pDC->MoveTo(rcItem.left+m_arc-2,rcItem.top+2);
		pDC->LineTo(rcItem.right-m_arc+2,rcItem.top+2);
	}
	PaintItemText(pDC, rcItem, ixItem);

	if (pen.m_hObject)
	{
		pen.DeleteObject();
	}
	if (pen1.m_hObject)
	{
		pen1.DeleteObject();
	}
}

//Kreslani zalozek 1. typu
//pDC - grafika
//rcPage - odelnik pro child dialog
//rcItem - odelnik Zalozky
//ixItem - poradova cislo zalozky, pocitano od 0
//bAktiv - true vybrana zalozka. false - nevybrana
void CMyTabCtrl::PaintItemTyp1(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv)
{
	CPen pen, pen1;

	if (bAktiv)
	{
		//vymazani cary pod neaktivnimi zalozkami
		pDC->MoveTo(rcPage.left,rcPage.top);
		pDC->LineTo(rcPage.left,rcPage.bottom);
	}
	else
	{
		//vymazani orazoveho zvyrazneni
		pDC->MoveTo(rcItem.left+1,rcItem.top+m_arc-1);
		pDC->LineTo(rcItem.left+1,rcItem.bottom-m_arc+1);
		pDC->MoveTo(rcItem.left+2,rcItem.top+m_arc-2);
		pDC->LineTo(rcItem.left+2,rcItem.bottom-m_arc+2);
	}

	pen.CreatePen (PS_SOLID, 1, SetColRect());
	if (pen.m_hObject)
	{
		pDC->SelectObject(&pen);
	}
	
	pDC->MoveTo(rcItem.right,rcItem.top);
	pDC->LineTo(rcItem.left+m_arc,rcItem.top);
	pDC->AngleArc(rcItem.left+m_arc, rcItem.top+m_arc, m_arc, 90, 90);
	pDC->LineTo(rcItem.left,rcItem.bottom-m_arc);
	pDC->AngleArc(rcItem.left+m_arc, rcItem.bottom-m_arc, m_arc, 180, 90);
	pDC->LineTo(rcItem.right,rcItem.bottom);
	if (bAktiv)
	{
		pDC->LineTo(rcPage.left,rcPage.bottom);
		pDC->LineTo(rcPage.right,rcPage.bottom);
		pDC->LineTo(rcPage.right,rcPage.top);
		pDC->LineTo(rcPage.left,rcPage.top);
		pDC->LineTo(rcItem.right,rcItem.top);
		//zvyrazneni aktivniho listu
		pen1.CreatePen (PS_SOLID, 1, SetColIsert());
		if (pen1.m_hObject)
		{
			pDC->SelectObject(&pen1);
		}
		pDC->MoveTo(rcItem.left,rcItem.top+m_arc);
		pDC->LineTo(rcItem.left,rcItem.bottom-m_arc);
		pDC->MoveTo(rcItem.left+1,rcItem.top+m_arc-1);
		pDC->LineTo(rcItem.left+1,rcItem.bottom-m_arc+1);
		pDC->MoveTo(rcItem.left+2,rcItem.top+m_arc-2);
		pDC->LineTo(rcItem.left+2,rcItem.bottom-m_arc+2);
	}
	PaintItemText(pDC, rcItem, ixItem);

	if (pen.m_hObject)
	{
		pen.DeleteObject();
	}
	if (pen1.m_hObject)
	{
		pen1.DeleteObject();
	}
}

//Kreslani zalozek 2. typu
//pDC - grafika
//rcPage - odelnik pro child dialog
//rcItem - odelnik Zalozky
//ixItem - poradova cislo zalozky, pocitano od 0
//bAktiv - true vybrana zalozka. false - nevybrana
void CMyTabCtrl::PaintItemTyp2(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv)
{
	CPen pen, pen1;

	if (bAktiv)
	{
		//vymazani cary pod neaktivnimi zalozkami
		pDC->MoveTo(rcPage.left,rcPage.bottom);
		pDC->LineTo(rcPage.right,rcPage.bottom);
	}
	else
	{
		//vymazani orazoveho zvyrazneni
		pDC->MoveTo(rcItem.left+m_arc-1,rcItem.bottom-1);
		pDC->LineTo(rcItem.right-m_arc+1,rcItem.bottom-1);
		pDC->MoveTo(rcItem.left+m_arc-2,rcItem.bottom-2);
		pDC->LineTo(rcItem.right-m_arc+2,rcItem.bottom-2);
	}

	pen.CreatePen (PS_SOLID, 1, SetColRect());
	if (pen.m_hObject)
	{
		pDC->SelectObject(&pen);
	}
	
	pDC->MoveTo(rcItem.left,rcItem.top);
	pDC->LineTo(rcItem.left,rcItem.bottom-m_arc);
	pDC->AngleArc(rcItem.left+m_arc, rcItem.bottom-m_arc, m_arc, 180, 90);
	pDC->LineTo(rcItem.right-m_arc,rcItem.bottom);
	pDC->AngleArc(rcItem.right-m_arc, rcItem.bottom-m_arc, m_arc, 270, 90);
	pDC->LineTo(rcItem.right,rcItem.top);
	if (bAktiv)
	{
		pDC->LineTo(rcPage.right,rcPage.bottom);
		pDC->LineTo(rcPage.right,rcPage.top);
		pDC->LineTo(rcPage.left,rcPage.top);
		pDC->LineTo(rcPage.left,rcPage.bottom);
		pDC->LineTo(rcItem.left,rcItem.top);
		//zvyrazneni aktivniho listu
		pen1.CreatePen (PS_SOLID, 1, SetColIsert());
		if (pen1.m_hObject)
		{
			pDC->SelectObject(&pen1);
		}
		pDC->MoveTo(rcItem.left+m_arc,rcItem.bottom);
		pDC->LineTo(rcItem.right-m_arc,rcItem.bottom);
		pDC->MoveTo(rcItem.left+m_arc-1,rcItem.bottom-1);
		pDC->LineTo(rcItem.right-m_arc+1,rcItem.bottom-1);
		pDC->MoveTo(rcItem.left+m_arc-2,rcItem.bottom-2);
		pDC->LineTo(rcItem.right-m_arc+2,rcItem.bottom-2);
	}
	PaintItemText(pDC, rcItem, ixItem);

	if (pen.m_hObject)
	{
		pen.DeleteObject();
	}
	if (pen1.m_hObject)
	{
		pen1.DeleteObject();
	}
}

//Kreslani zalozek 3. typu
//pDC - grafika
//rcPage - odelnik pro child dialog
//rcItem - odelnik Zalozky
//ixItem - poradova cislo zalozky, pocitano od 0
//bAktiv - true vybrana zalozka. false - nevybrana
void CMyTabCtrl::PaintItemTyp3(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv)
{
	CPen pen, pen1;

	if (bAktiv)
	{
		//vymazani cary pod neaktivnimi zalozkami
		pDC->MoveTo(rcPage.right,rcPage.top);
		pDC->LineTo(rcPage.right,rcPage.bottom);
	}
	else
	{
		//vymazani orazoveho zvyrazneni
		pDC->MoveTo(rcItem.right-1,rcItem.top+m_arc-1);
		pDC->LineTo(rcItem.right-1,rcItem.bottom-m_arc+1);
		pDC->MoveTo(rcItem.right-2,rcItem.top+m_arc-2);
		pDC->LineTo(rcItem.right-2,rcItem.bottom-m_arc+2);
	}

	pen.CreatePen (PS_SOLID, 1, SetColRect());
	if (pen.m_hObject)
	{
		pDC->SelectObject(&pen);
	}
	
	pDC->MoveTo(rcItem.left,rcItem.top);
	pDC->LineTo(rcItem.right-m_arc,rcItem.top);
	pDC->AngleArc(rcItem.right-m_arc, rcItem.top+m_arc, m_arc, 90, -90);
	pDC->LineTo(rcItem.right,rcItem.bottom-m_arc);
	pDC->AngleArc(rcItem.right-m_arc, rcItem.bottom-m_arc, m_arc, 0, -90);
	pDC->LineTo(rcItem.left,rcItem.bottom);
	if (bAktiv)
	{
		pDC->LineTo(rcPage.right,rcPage.bottom);
		pDC->LineTo(rcPage.left,rcPage.bottom);
		pDC->LineTo(rcPage.left,rcPage.top);
		pDC->LineTo(rcPage.right,rcPage.top);
		pDC->LineTo(rcItem.left,rcItem.top);
		//zvyrazneni aktivniho listu
		pen1.CreatePen (PS_SOLID, 1, SetColIsert());
		if (pen1.m_hObject)
		{
			pDC->SelectObject(&pen1);
		}
		pDC->MoveTo(rcItem.right,rcItem.top+m_arc);
		pDC->LineTo(rcItem.right,rcItem.bottom-m_arc);
		pDC->MoveTo(rcItem.right-1,rcItem.top+m_arc-1);
		pDC->LineTo(rcItem.right-1,rcItem.bottom-m_arc+1);
		pDC->MoveTo(rcItem.right-2,rcItem.top+m_arc-2);
		pDC->LineTo(rcItem.right-2,rcItem.bottom-m_arc+2);
	}
	PaintItemText(pDC, rcItem, ixItem);

	if (pen.m_hObject)
	{
		pen.DeleteObject();
	}
	if (pen1.m_hObject)
	{
		pen1.DeleteObject();
	}
}

//text v zalozce
//pDC - grafika
//rcItem - odelnik Zalozky
//ixItem - poradova cislo zalozky, pocitano od 0
void CMyTabCtrl::PaintItemText(CDC* pDC, const CRect& rcItem, int ixItem)
{
	//text v zalozce
	TC_ITEM tci;
	CString sText;
	CSize sizeText;

	tci.mask       = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText    = sText.GetBuffer(128);
	tci.cchTextMax = 127;
	GetItem(ixItem, &tci);

	sizeText = pDC->GetTextExtent(tci.pszText);

	if (m_iWayText == 900)
	{
		pDC->TextOut(rcItem.right-sizeText.cy-3, rcItem.bottom-3, tci.pszText);
	}
	else
	{
		pDC->TextOut(rcItem.left+3, rcItem.bottom-sizeText.cy-3, tci.pszText);
	}
}

//vypocet obedlniku pro vlozeni child dialogu
//rc - obdelnik dialogu
//iTyp - typ zalozek
//height - sirka zalozky
//i - konstanta pro zmenseni vsech rozmeru
CRect CMyTabCtrl::GetRectNoneDialog(CRect rc, int iTyp, int height, int i)
{
	switch(iTyp)
	{
		case 0:
			rc.left += i; 
			rc.top += height+i;
			rc.right -= i;
			rc.bottom -= i;
		break;
		case 1:
			rc.left += height+i;
			rc.top += i;
			rc.right -= i; 
			rc.bottom -= i;
		break;
		case 2:
			rc.left += i;
			rc.top += i;
			rc.right -= i; 
			rc.bottom -= height+i;
		break;
		case 3:
			rc.left += i;
			rc.top += i;
			rc.right -= height+i; 
			rc.bottom -= i;
		break;
		default:
			rc.left += i; 
			rc.top += height+i;
			rc.right -= i;
			rc.bottom -= i;
		break;
	}

	return rc;
}

void CMyTabCtrl::SetTypTable(int iTyp)
{
	m_iTyp = iTyp;

	if ((m_iTyp < 0) || (4 < m_iTyp))
	{
		m_iTyp = 0;
	}
}

void CMyTabCtrl::SetWayText(int iWayText)
{
	m_iWayText = iWayText;
}

//barva pro ramecek
COLORREF CMyTabCtrl::SetColRect()
{
	return RGB(0x91, 0x9b, 0x9c);
}

//barva pro vybrany list (oranzova)
COLORREF CMyTabCtrl::SetColIsert()
{
	return RGB(0xff, 0xc8, 0x3c);
}
