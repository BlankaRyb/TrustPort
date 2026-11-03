#include "StdAfx.h"
#include "ButtonMy.h"

using namespace std;

CMap<int,int,DataText, DataText> MapData;

CButtonMy::CButtonMy(void)
{
	m_iPocet = -1;
	m_iWidth = 100;
	m_iInterval = 0;
	m_iSizeIco = 32;
	m_rc = CRect(0,0,0,0);
	m_pPixel = m_pPixelIco = m_pPos = CPoint(0,0);
	m_bClickSelect = FALSE;
}

CButtonMy::~CButtonMy(void)
{
	MapData.RemoveAll();
}

//Prida rozbalvaci button
//nStr1 - text v nadpisu
//nStr2 - text pod nadpisem
//nID1 - ikona pro zbaleny text
//nID2 - ikona	pro rozbaleny text
//nIDD - dialog, ktery patri k rozbalovacimu textu
void CButtonMy::Add(UINT nStr1, UINT nStr2, UINT nID1, UINT nID2, UINT nIDD)
{
	DataText data;

	m_iPocet++;

	data.str1.LoadString(nStr1);
	data.str2.LoadString(nStr2);
	data.hIco1 = AfxGetApp()->LoadIcon(nID1);
	data.hIco2 = AfxGetApp()->LoadIcon(nID2);
	data.nIDD = nIDD;
	if (nIDD)
	{
		data.pDialog = new CDialog();
		data.pDialog->Create(nIDD, this);
		data.pDialog->GetClientRect(data.rc);
	}
	else //NULL
	{
		data.pDialog = NULL;
		data.rc = CRect(0,0,0,0);
	}
	data.bRozbaleno = FALSE;
	data.iHeight = 0;
	data.iPosY = 0;
	data.recText = CRect(0,0,0,0);
	data.recIco = CRect(0,0,32,32);

	MapData.SetAt(m_iPocet, data);
}

//nastaveni sirky objektu - sirka mista pro text
void CButtonMy::SetWidth(int iWidth)
{
	m_iWidth = iWidth;
}

//nastavi mezeru v pixelech mezi texty
void CButtonMy::SetInterval(int iInterval)
{
	m_iInterval = iInterval;
}

//rozmer Ikony
void CButtonMy::SetSizeIco(int iSizeIco)
{
	m_iSizeIco = iSizeIco;
//	m_recIco = CRect(m_recIco.TopLeft(),CSize(iSizeIco, iSizeIco));
}

//pozice textu
void CButtonMy::SetPosText(CPoint pPixel)
{
	m_pPixel = pPixel;
}

//pozice Icony
void CButtonMy::SetPosIco(CPoint pPixelIco)
{
	m_pPixelIco = pPixelIco;
//	m_recIco.MoveToXY(m_pPixelIco);
}

//Vypocet vsech textu
void CButtonMy::Calculation()
{
	int i, iHeigh;
	CSize size;
	DataText data;
	CFont *font1, *font2;

	iHeigh = 0;
	font1 = GetFont1();
	font2 = GetFont2();

	for (i=0; i<=m_iPocet; i++)
	{
		data = MapData[i];
		//Pozice Y pro text
		data.iPosY = iHeigh;

		//vypocet vysky buttonu
		size = GetSizeText(font1, data.str1);
		data.recText = CRect(CPoint(m_pPixel.x,m_pPixel.y+iHeigh), size);
		data.iHeight = size.cy;
		if (data.bRozbaleno)
		{
			size = GetSizeText(font2, data.str2);
			data.iHeight += size.cy;

			//Dialog
			if (data.nIDD)
			{
/*				kvuli rychlosti presunuto do Add
				CDialog* pDialog;
				pDialog = new CDialog();
				pDialog->Create(data.nIDD, this);
				data.pDialog->GetClientRect(rc);*/
				data.pDialog->SetWindowPos(NULL,data.rc.left,data.rc.top+data.iHeight+iHeigh,data.rc.Width(),data.rc.Height(),SWP_NOZORDER);
				data.pDialog->ShowWindow(SW_SHOW);
				data.iHeight += data.rc.Height();
			}
		}
		else //je kvuli zavreni dialogu pod textem
		{
			if (data.nIDD)
			{
				data.pDialog->ShowWindow(SW_HIDE);
			}
		}

		iHeigh += data.iHeight+m_iInterval;
		MapData.SetAt(i, data);
	}

	if (font1)
	{
		font1->DeleteObject();
	}
	if (font2)
	{
		font2->DeleteObject();
	}
	m_rc = CRect(m_rc.TopLeft(),CSize(m_iWidth, iHeigh));
}

BEGIN_MESSAGE_MAP(CButtonMy, CStatic)
ON_WM_PAINT()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CButtonMy::OnPaint()
{
	int i;
	CPaintDC dc(this); // device context for painting

	//Text
	dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	dc.SetBkColor(GetSysColor(COLOR_BTNFACE));

	for(i=0; i<=m_iPocet; i++)
	{
		DrawText(dc.m_hDC, i);

		if (MapData[i].bRozbaleno)
		{
			DrawIconEx((HDC)dc, m_pPixelIco.x, m_pPixelIco.y+MapData[i].iPosY, MapData[i].hIco2, m_iSizeIco, m_iSizeIco, 0, NULL, DI_NORMAL);
		}
		else
		{
			//Ikona
			DrawIconEx((HDC)dc, m_pPixelIco.x, m_pPixelIco.y+MapData[i].iPosY, MapData[i].hIco1, m_iSizeIco, m_iSizeIco, 0, NULL, DI_NORMAL);
		}
		MapData[i].recIco = CRect(CPoint(m_pPixelIco.x, m_pPixelIco.y+MapData[i].iPosY), CSize(m_iSizeIco, m_iSizeIco));
	}

	// Do not call CStatic::OnPaint() for painting messages
}

CFont *CButtonMy::GetFont1()
{
    int     fontsize = 8;
    CDC     dc;    
    CFont   *fontText = new(CFont);

    dc.CreateCompatibleDC(NULL);
	fontText->CreateFont(
			-13,
			0,                         // nWidth
			0,                         // angle of escapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			TRUE,                      // bUnderline
			FALSE,                     // cStrikeOut
			DEFAULT_CHARSET,           // nCharSet
			OUT_TT_PRECIS,             // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			PROOF_QUALITY,             // nQuality
			VARIABLE_PITCH | FF_SWISS, // nPitchAndFamily
			(LPCTSTR)"Arial CE");      // lpszFacename

	return fontText;
}

CFont *CButtonMy::GetFont2()
{
    int     fontsize = 10;//20;
    CFont   *fontText = new(CFont);

	fontText->CreateFont(
			-13,                       // nHeight
			0,                         // nWidth
			0,                         // angle of escapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			DEFAULT_CHARSET,           // nCharSet
			OUT_TT_PRECIS,             // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			PROOF_QUALITY,             // nQuality
			VARIABLE_PITCH | FF_SWISS, // nPitchAndFamily
			(LPCTSTR)"Arial CE");      // lpszFacename

	return fontText;
}

//kresli dva texty patrici k sobe
//hDC - kreslici zarizeni
//i - poradi ve strukture MapData
void CButtonMy::DrawText(HDC hDC, int i)
{
	CSize size;
	CFont *font1, *font2;

	//vypocet vysky buttonu
	font1 = GetFont1();
	size = DrawText(hDC, m_pPixel.x, m_pPixel.y+MapData[i].iPosY, font1, MapData[i].str1);
//	MapData[i].recText = CRect(CPoint(m_pPixel.x,m_pPixel.y+size.cy, size);
	if (MapData[i].bRozbaleno)
	{
		font2 = GetFont2();
		DrawText(hDC, m_pPixel.x, m_pPixel.y+MapData[i].iPosY+size.cy, font2, MapData[i].str2);
		if (font2)
		{
			font2->DeleteObject();
		}
	}
	if (font1)
	{
		font1->DeleteObject();
	}
}

//Nakresli test a vrati jeho rozmery
//hDC - kreslici zarizeni
//x,y - pozice textu
//font - font textu
//sText - zobrazovany text
CSize CButtonMy::DrawText(HDC hDC, int x, int y, CFont *font, CString sText)
{
	HFONT pOldFont;
	SIZE size;
	int cxMax; //maximalni rozmer cx
	CString s1, s2, s21, s22; //pomocne stringy
	int i;
	int iHeight;

	iHeight = cxMax = 0;

	if (font)
	{
		pOldFont = (HFONT)SelectObject(hDC, (HFONT)font->m_hObject);
	}
	if (m_pPixel.x>=m_iWidth)
	{
		m_iWidth = m_pPixel.x+1;
	}
	s2 = sText;
	//string se rozdeli podle '\n'
	do
	{
		i = s2.Find('\n');
		if (i>=0)
		{
			s1 = s2.Left(i);
			s2 = s2.Right(s2.GetLength()-i-1);
			s1 = DelMezery(s1);
			s2 = DelMezery(s2);
		}
		else
		{
			s1 = s2;
		}
		if (s1.GetLength()>0)
		{
			GetTextExtentPoint32(hDC, s1, s1.GetLength(), &size);
			//string je DLOUHY, rozdeli se podle ' '
			if (size.cx > (m_iWidth - m_pPixel.x))
			{
				while (s1 != "")
				{
					s21 = RozdelString(hDC, font, s1, &s22, &size);
					TextOut(hDC,x,y+iHeight,s21,s21.GetLength());
					s1 = s22;
					iHeight += size.cy;
					if (size.cx > cxMax)
					{
						cxMax = size.cx;
					}
				} 
			}
			else
			{
				TextOut(hDC,x,y+iHeight,s1,s1.GetLength());
				iHeight += size.cy;
				if (size.cx > cxMax)
				{
					cxMax = size.cx;
				}
			}
		}
	} while(i>=0);

	if (font)
	{
		SelectObject(hDC, pOldFont);
	}

	return CSize(cxMax,iHeight);
}

//Odstrani ze stringu na zacatku a na konci 32 (' ')
CString CButtonMy::DelMezery(CString s)
{
	int iLen;

	while (s[0] == ' ')
	{
		s.Delete(0,1);
	}

	iLen = s.GetLength();
	while (s[iLen-1] == ' ')
	{
		s.Delete(iLen-1,1);
		iLen--;
	}

	return s;
}

//Rozdeli string tak, aby navratova hodnota byla pri zobrazeni mela maximalni delku
//nebo byla jen jedno slovo

//vraci string povolene delky nebo minylani mozne
//hDC - graficke zarizeni
//font - font textu
//s - string, ktery se upravuje
//s1 - string po odsrtaneni stringu v navratove hodnete
//size - rozmery zorbrazovaneho stringu v navratove hodnte
CString CButtonMy::RozdelString(HDC hDC, CFont *font, CString s, CString *s1, SIZE *size)
{
	int i;
	CString str;
	HFONT pOldFont;

	if (font)
	{
		pOldFont = (HFONT)SelectObject(hDC, (HFONT)font->m_hObject);
	}

	GetTextExtentPoint32(hDC, s, s.GetLength(), size);

	i = 0;
	str = s;
	*s1 = "";
	while ((size->cx > (m_iWidth-m_pPixel.x)) && (i >= 0))
	{
		i = str.ReverseFind(' ');
		if (i >= 0)//pokud neni slnena podminka je slovo vetsi nez pozadovana delka
		{
			str = s.Left(i);
			*s1 = s.Right(s.GetLength()-i-1);
			str = DelMezery(str);
			*s1 = DelMezery(*s1);
			GetTextExtentPoint32(hDC, str, str.GetLength(), size);
		}
	}

	if (font)
	{
		SelectObject(hDC, pOldFont);
	}

	return str;
}

void CButtonMy::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
}

//vrati rozmery textu s ohledem na nutny pocet radku
//font - font textu
//sText - text
CSize CButtonMy::GetSizeText(CFont *font, CString sText)
{
	CFont *pOldFont;
	CSize size;
	int cxMax; //maximalni rozmer cx
	CPaintDC dc(this); // device context for painting
	CString s1, s2, s21, s22; //pomocne stringy
	int i;
	int iHeight;

	iHeight = cxMax = 0;

	if (font)
	{
		pOldFont = dc.SelectObject(font);
	}
	if (m_pPixel.x>=m_iWidth)
	{
		m_iWidth = m_pPixel.x+1;
	}
/*	if ((m_iWidth == 0)||(m_pPixel.x>=m_iWidth))
	{
		size = dc.GetTextExtent(sText);
		iHeight += size.cy;
	}*/
	s2 = sText;
	//string se rozdeli podle '\n'
	do
	{
		i = s2.Find('\n');
		if (i>=0)
		{
			s1 = s2.Left(i);
			s2 = s2.Right(s2.GetLength()-i-1);
			s1 = DelMezery(s1);
			s2 = DelMezery(s2);
		}
		else
		{
			s1 = s2;
		}
		if (s1.GetLength()>0)
		{
			size = dc.GetTextExtent(s1);
			//string je DLOUHY, rozdeli se podle ' '
			if (size.cx > (m_iWidth - m_pPixel.x))
			{
				while (s1 != "")
				{
					s21 = RozdelString(dc.m_hDC, font, s1, &s22, &size);
					s1 = s22;
					iHeight += size.cy;
					if (size.cx > cxMax)
					{
						cxMax = size.cx;
					}
				} 
			}
			else
			{
				iHeight += size.cy;
				if (size.cx > cxMax)
				{
					cxMax = size.cx;
				}
			}
		}
	} while(i>=0);

	if (font)
	{
		dc.SelectObject(pOldFont);
	}

	return CSize(cxMax,iHeight);
}

//nastaveni pozice buttonu; levy horni roh
//PREPOCTE rozmer buttonu
void CButtonMy::SetPos(CPoint pPos)
{
	m_pPos = pPos;
	m_rc.MoveToXY(m_pPos);

	this->SetWindowPos(NULL,m_rc.left,m_rc.top,m_rc.Width(),m_rc.Height(), SWP_NOZORDER);
}

//Vrati pozici buttonu; levy horni roh
CPoint CButtonMy::GetPos()
{
	return m_pPos;
}

void CButtonMy::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	Click (point);

	CStatic::OnLButtonDblClk(nFlags, point);
}

void CButtonMy::OnLButtonUp(UINT nFlags, CPoint point)
{
	Click (point);

	CStatic::OnLButtonUp(nFlags, point);
}

//Spolecna funkce pro Click dubleClick
//point - misto cliku
void CButtonMy::Click(CPoint point)
{
	int i;
	int iNalezeno;

	m_bClickSelect = FALSE;
	iNalezeno = -1;

	for (i=0; i<=m_iPocet; i++)
	{
		if (MapData[i].recText.PtInRect(point))
		{
			m_bClickSelect = TRUE;
			MapData[i].bRozbaleno = !MapData[i].bRozbaleno;
		}
		else
		{
			if (MapData[i].recIco.PtInRect(point))
			{
				m_bClickSelect = TRUE;
				MapData[i].bRozbaleno = !MapData[i].bRozbaleno;
			}
		}
		if (m_bClickSelect)
		{
			iNalezeno = i;
			i = m_iPocet+1;
		}
	}

	//novy vypocet rozmeru buttonu
	if (m_bClickSelect)
	{
		int iHeightPuv;

		iHeightPuv = MapData[iNalezeno].iHeight;
		Calculation();
		m_iChangeHeight = MapData[iNalezeno].iHeight - iHeightPuv;

//		m_rc = CRect(m_rc.TopLeft(),CSize(m_iWidth, m_iHeight)); //je provedeno v Calculation();
		this->SetWindowPos(NULL,m_rc.left,m_rc.top,m_rc.Width(),m_rc.Height(), SWP_NOZORDER);
	}
}

//Vrati vysku radku pro text pod nadpisem
int CButtonMy::GetSizeLine()
{
	CFont *font;
	CFont *pOldFont;
	CSize size;
	CPaintDC dc(this); // device context for painting

	font = GetFont2();
	if (font)
	{
		pOldFont = dc.SelectObject(font);
	}
	size = dc.GetTextExtent(MapData[0].str2);
	if (font)
	{
		dc.SelectObject(pOldFont);
		font->DeleteObject();
	}

	return size.cy;
}

//Vrati odelnik buttonu
CRect CButtonMy::GetRectButt()
{
	return m_rc;
}

//Vraci TRUE jestli bylo kliknuto na ikonu nebo text, jinak FALSE
BOOL CButtonMy::GetClick()
{
	return m_bClickSelect;
}

//Vrati zmenu velikosti buttonu po poslednim kliku
int CButtonMy::GetChangeHeight()
{
	return m_iChangeHeight;
}

//predani parametru
//i - poradi (pocitano pri pouziti Add) poradi pocitano od 0
//nIDD - idd prvku v dialogu
//str - text ktery se vyplni do prvku
void CButtonMy::SetParam(int i, UINT nIDD, CString str)
{
	if (MapData[i].pDialog)
	{
		::SetWindowText(MapData[i].pDialog->GetDlgItem(nIDD)->m_hWnd, str);
	}
}

//Vrati string, ktery je vyplnen v polozce
//i - poradi (pocitano pri pouziti Add) poradi pocitano od 0
//nIDD - idd prvku v dialogu
CString CButtonMy::GetParam(int i, UINT nIDD)
{
	CString     s;
	HWND        hWnd;
	int         iLen;

	DataText data;
	data = MapData[i];

	if (MapData[i].pDialog)
	{
		hWnd = MapData[i].pDialog->GetDlgItem(nIDD)->m_hWnd;
		iLen = ::GetWindowTextLength(hWnd);

		::GetWindowText(hWnd, s.GetBufferSetLength(iLen), iLen+1);
	}

	return s;
}
