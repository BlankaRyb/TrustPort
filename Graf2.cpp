#include "StdAfx.h"
#include "Graf2.h"
#include "resource.h"

CGraf2::CGraf2(void)
{
	int i;

	m_x1 = 0;
	m_x2 = 10;
	m_y1 = 0;
	m_y2 = 10;
	m_xs = m_ys = 0;
	m_rc = CRect(0, 0, 0, 0);
	m_sCaption = _T("");
	m_ixCap = m_iyCap = 0;
	m_iTypGraf = 0;
	m_iPocBar = 1;
	timeModified = 0;
	m_sFileName = "";
	m_numFce = 1;
	m_iTypTime = -1;
	for(i=0; i<100; i++)
	{
		m_bFunkce[i] = TRUE;
		m_rgb[i] = RGB(0,0,0);
	}
}

CGraf2::~CGraf2(void)
{
	int i;

	for(i=0; i<100; i++)
	{
		MapBody[i].RemoveAll();
	}
	MapBar.RemoveAll();
	MapInterval.RemoveAll();
	MapIntervalT.RemoveAll();
}
BEGIN_MESSAGE_MAP(CGraf2, CGraf)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CGraf2::OnPaint()
{
	CSize size;
	CFont *font;
	CFont *pOldFont;
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(rc);

//	pOldFont = (HFONT)dc->SelectObject((HFONT)font->m_hObject);
	font = GetFont();
	if (font)
	{
		pOldFont = dc.SelectObject(font);
	}
	dc.SetBkColor(GetSysColor(COLOR_BTNFACE));
//	dc.Rectangle(m_rc);
	//pro pripad neexistujici obdelniku
	size = m_rc.Size();
	if ((size.cx <= 0) || (size.cy <= 0))
	{
		m_rc = rc;
	}
	//pro pripad neexistujicich os
	if (m_x1 >= m_x2)
	{
		m_x2 = m_x1+1;
	}
	if (m_y1 >= m_y2)
	{
		m_y2 = m_y1+1;
	}
//	dc.SetTextColor(RGB(0,0,200));
//	dc.TextOut(20,50,_T("AHOJ"));

	Graf(&dc);
	DrawOsy(&dc);
	if (font)
	{
		dc.SelectObject(pOldFont);
		font->DeleteObject();
	}
}

//nastavi rozpeti os
//x1, x2 - pocatek a konec osy X
//y1, y2 - pocatek a konec osy Y
void CGraf2::SetOsyXY(double x1, double x2, double y1, double y2)
{
	m_x1 = x1;
	m_x2 = x2;
	m_y1 = y1;
	m_y2 = y2;
}

//nastavi pod kde se kreslene osy krizi
void CGraf2::SetCentre(double xs, double ys)
{
	m_xs = xs;
	m_ys = ys;
}

//nastaveni zmenseni obelniku s grafem
void CGraf2::SetSizeRect(int left, int top, int right, int bottom)
{
	GetClientRect(m_rc);
	m_rc.left += left;
	m_rc.top += top;
	m_rc.right -= right;
	m_rc.bottom -= bottom;
}

//kresli osy x,y
void CGraf2::DrawOsy(CDC* pDC)
{
	CPen penTluste, penTenke, *pOldPen;
	int x,y;

	penTluste.CreatePen (PS_SOLID, 3, GetSysColor(COLOR_WINDOWTEXT));
	penTenke.CreatePen (PS_SOLID, 1, GetSysColor(COLOR_WINDOWTEXT));

	if (penTluste.m_hObject)
	{
		pOldPen = pDC->SelectObject(&penTluste);
	}

	//Osa X
	y = (int)(GetPosOsy(m_ys, m_y1, m_y2, m_rc.top, m_rc.bottom, FALSE)+0.5);
	pDC->MoveTo(m_rc.left,y);
	pDC->LineTo(m_rc.right,y);

	//Osa Y
	x = (int)(GetPosOsy(m_xs, m_x1, m_x2, m_rc.left, m_rc.right, TRUE)+0.5);
	pDC->MoveTo(x, m_rc.top);
	pDC->LineTo(x, m_rc.bottom);

	if (penTenke.m_hObject)
	{
		pDC->SelectObject(&penTenke);
	}
	//popis osy X
	if (m_iTypGraf == FUNKCE)
	{
		DescriptionOsyX(pDC, y);
	}
	//popis osy Y
	DescriptionOsyY(pDC, x);
	//nadpis
	DrawCaption(pDC);

	if (penTluste.m_hObject)
	{
		pDC->SelectObject(pOldPen);
		penTluste.DeleteObject();
	}
	if (penTenke.m_hObject)
	{
		penTenke.DeleteObject();
	}
}

//kresli cila a carky na ose X
//y - v pixelech misto kde vede osa X
void CGraf2::DescriptionOsyX(CDC* pDC, int y)
{
	double dilek; //dlouha cara
	double dilek_puv; //puvodni dilek, pro intervali, ktere se meni diky ruznemu postu dni v mesicich
	double dilek_m; //kratka cara
	double d; //dilky na ose po kterych se posouva
	int x;
	int i; //i =0,1,2,3,4 a kdyz i=0 tak dlouha cara
	CSize   sizeText;
	CString str;
	CTime t;
	BOOL bCasVelky;

	bCasVelky = FALSE;
	if (m_iTypTime<0)//standartni cisla
	{
		dilek = Distance(m_x1, m_x2, m_rc.left, m_rc.right);
	}
	else //cas
	{
		dilek = (double)DistanceTimeX(m_x1, m_x2, m_rc.left, m_rc.right);
	}

	//dilky
	if (m_iTypTime<0)//standartni cisla
	{
		d = Get1Dilek(&i, m_x1, dilek);
	}
	else //cas
	{
		if ((60*60*24*30) > dilek)
		{
			d = Get1DilekTime(&i, m_x1, dilek);
		}
		else //casovy udaj je mesic nebo vetsi
		{
			dilek_puv = dilek;
			d = Get1DilekVelkyTime(&i, m_x1, &dilek);
			bCasVelky = TRUE;
		}
	}

	dilek_m = dilek/5;

	for(d=d; d<=m_x2; d+=dilek_m)
	{
		if ((-0.0000000001<d)&&(d<0.0000000001))
		{
			d = 0;
		}
		x = (int)(GetPosOsy(d, m_x1, m_x2, m_rc.left, m_rc.right, TRUE)+0.5);
		if (i!=0)
		{
			pDC->MoveTo(x, y-m_carka);
			pDC->LineTo(x, y+m_carka);
		}
		else
		{
			pDC->MoveTo(x, m_rc.top);
			pDC->LineTo(x, m_rc.bottom);
			if (m_iTypTime<0)//standartni cisla
			{
				str = GetTextNumber(d, dilek);
				sizeText = pDC->GetTextExtent(str);
				pDC->TextOut(x-sizeText.cx/2,y+m_carka, str);
			}
			else //cas
			{
				t = CTime((__int64)d);
				str = GetTimeKomplet1(t);
				sizeText = pDC->GetTextExtent(str);
				pDC->TextOut(x-sizeText.cx/2,y+m_carka, str);
				str = GetTimeKomplet2(t);
				sizeText = pDC->GetTextExtent(str);
				pDC->TextOut(x-sizeText.cx/2,y+sizeText.cy+m_carka, str);
				if (bCasVelky) //cas se pro interval mesic a vetsi pocita podle postu dnu v mesici
				{
					dilek = GetIntervalVelkyTime(d, dilek_puv);
					dilek_m = dilek/5;
				}
			}
			i = 0;
		}
		i++;
		if (i==5)
		{
			i = 0;
		}
	}
	//Text u osy X
	if (m_strOsaX != "")
	{
		sizeText = pDC->GetTextExtent(m_strOsaX);
		if (m_iTypTime<0)//standartni cisla
		{
			pDC->TextOut(m_rc.right-sizeText.cx,y+m_carka+sizeText.cy, m_strOsaX);
		}
		else //cas
		{
			pDC->TextOut(m_rc.right-sizeText.cx,y+m_carka+sizeText.cy*2, m_strOsaX);
		}
	}
}

//kresli cila a carky na ose Y
//x - v pixelech misto kde vede osa Y
void CGraf2::DescriptionOsyY(CDC* pDC, int x)
{
	double dilek; //dlouha cara
	double dilek_m; //kratka cara
	double d; //dilky bna ose po kterych se posouva
	int y;
	int i; //i =0,1,2,3,4 a kdyz i=0 tak dlouha cara
	CSize   sizeText;
	CString str;
	int ip; //posun textu u popisu osy Y
	CFont *font;

	dilek = Distance(m_y1, m_y2, m_rc.top, m_rc.bottom);
	dilek_m = dilek/5;

	ip = 0;
	sizeText.cx = sizeText.cy = 0;

	//dilky
	d = Get1Dilek(&i, m_y1, dilek);
	for(d=d; d<=m_y2; d+=dilek_m)
	{
		if ((-0.0000000001<d)&&(d<0.0000000001))
		{
			d = 0;
		}
		y = (int)(GetPosOsy(d, m_y1, m_y2, m_rc.top, m_rc.bottom, FALSE)+0.5);
		if (i!=0)
		{
			pDC->MoveTo(x-m_carka, y);
			pDC->LineTo(x+m_carka, y);
		}
		else
		{
			pDC->MoveTo(m_rc.left, y);
			pDC->LineTo(m_rc.right, y);
			str = GetTextNumber(d, dilek);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(x-sizeText.cx-m_carka,y-sizeText.cy/2, str);
			i = 0;
		}
		i++;
		if (i==5)
		{
			i = 0;
		}
	}
	//Text u osy Y
	if (m_strOsaY != "")
	{
		ip = sizeText.cx;
		font = GetFont90();
		if (font)
		{
			pDC->SelectObject(font);
		}
		sizeText = pDC->GetTextExtent(m_strOsaY);
		pDC->TextOut(x-sizeText.cy-ip-m_carka,m_rc.top+sizeText.cx, m_strOsaY);
		if (font)
		{
			font->DeleteObject();
		}
	}
}

//funkce ktera se kresli
//x - souradnice x
//num - poradi v grafu, standartne 0, tj. v grafu je jedna funkce
double CGraf2::Funkce(double x, int num /*= 0*/)
{ //x^2
	double y;

	y = pow(x, 2);

	return y;
}

//nastaveni barvy grafu
//rgb - barva grafu
//num - poradi v grafu, standartne 0, tj. v grafu je jedna funkce
void CGraf2::SetColorGraf(int rgb, int num /*= 0*/)
{
	m_rgb[num] = rgb;
}

//Kresli graf
void CGraf2::Graf(CDC* pDC)
{
	switch(m_iTypGraf)
	{
		case FUNKCE: //0
		{
			GrafFunkce(pDC);
		}
		break;
		case BAR: //1
		case BAR_3D: //2
		{
			GrafBar(pDC);
		}
		break;
		default:
		{
		}
		break;
	}
}

//Kresli graf funce nebo zadany daty
void CGraf2::GrafFunkce(CDC* pDC)
{
	CPen pen, *pOldPen;
	double x,y;
	int ix,iy;
	BOOL b; //je potreba zadat prvni bod
	int iPocetDat; //pocet dat
	BOOL bNalezeno;
	int i;
	double x_end;
	int iFce;

	for (iFce=0; iFce<m_numFce; iFce++)
	{
		pen.CreatePen (PS_SOLID, 1, m_rgb[iFce]);
		if(pen.m_hObject)
		{
			pOldPen = pDC->SelectObject(&pen);
		}

		b = TRUE;

		if (m_bFunkce[iFce]) //funkce
		{
			ix = m_rc.left;
			for (ix=m_rc.left; ix<=m_rc.right; ix++)
			{
				x = GetOsyPosCis(ix, TRUE);
				y = Funkce(x, iFce);
				iy = GetPosOsyPixY(y);
				if ((iy<m_rc.top) || (iy>m_rc.bottom))
				{
					b = TRUE;
				}
				if (b)
				{
					pDC->MoveTo(ix, iy);
				}
				else
				{
					pDC->LineTo(ix, iy);
				}
				b = FALSE;
			}
		}
		else //data
		{
			iPocetDat = (int)MapBody[iFce].GetSize();

			//Najit 1. bod pred zacatkem obdelniku pro graf
			x = GetOsyPosCis(m_rc.left, TRUE);
			x_end = GetOsyPosCis(m_rc.right, TRUE);
			bNalezeno = FALSE;
			i = 0;
			while (!bNalezeno)
			{
				if (MapBody[iFce][i].x > x)
				{
					bNalezeno = TRUE;
				}
				i++;
			}
			if (bNalezeno) //nebude splneno, kdyz budou osy zadane mimo rozsah dat
			{
				i -= 2; //vraceni k predchozimu bodu
				if (i<0)
				{
					i = 0;
				}
				for (i=i; i<iPocetDat; i++)
				{
					ix = GetPosOsyPixX(MapBody[iFce][i].x);
					iy = GetPosOsyPixY(MapBody[iFce][i].y);
					if (b)
					{
						pDC->MoveTo(ix, iy);
					}
					else
					{
						pDC->LineTo(ix, iy);
					}	

					b = FALSE;
					if (x_end<MapBody[iFce][i].x) //nema smysl kreslit mimo obrazovku
					{
						ix = GetPosOsyPixX(MapBody[iFce][i-1].x);
						ix = GetPosOsyPixX(MapBody[iFce][i-2].x);
						ix = GetPosOsyPixX(x_end);
						i = iPocetDat;
					}
				}
			}

		}
		if(pen.m_hObject)
		{
			pDC->SelectObject(pOldPen);
			pen.DeleteObject();
		}
	}
}

//Kresli sloupcovy graf
void CGraf2::GrafBar(CDC* pDC)
{
	CPen pen, *pOldPen;
	CFont *font;
	CFont *pOldFont;
	CBrush *brush;
	CBrush *pOldBrush;
	int iBar;
	double dMezera;
	int iLeft, iTop, iRight, iBottom;
	int iPixelXs; //misto v pixelech pro m_xs
	int rgb;
	CSize sizeText;

	pen.CreatePen (PS_SOLID, 1, m_rgb[0]);
	if(pen.m_hObject)
	{
		pOldPen = pDC->SelectObject(&pen);
	}
	brush = GetBrush();
	if (brush)
	{
		pOldBrush = pDC->SelectObject(brush);
	}

	rgb = pDC->GetBkColor();

	iPixelXs = GetPosOsyPixX(m_xs);
	dMezera = (double)(m_rc.right-iPixelXs)/m_iPocBar;

	font = GetFont90();
	if (font)
	{
		pOldFont = pDC->SelectObject(font);
	}

	for(iBar=0; iBar<m_iPocBar; iBar++)
	{
		iTop = GetPosOsyPixY(MapBar[iBar].iPocet);
		iBottom = GetPosOsyPixY(0);
		if (m_iTypGraf == BAR)
		{
			iLeft = iPixelXs + (int)(dMezera*iBar+dMezera/5);
			iRight = iLeft+(int)(dMezera/5*3);
		}
		else //BAR_3D
		{
			iLeft = iPixelXs + (int)(dMezera*iBar+dMezera/4);
			iRight = iLeft+(int)(dMezera/4*2);
		}
		if (iLeft>=iRight)
		{
			iRight = iLeft+1;
		}
		pDC->SetBkColor(RGB(0xff, 0x8f, 0));
		if (m_iTypGraf == BAR)
		{
			pDC->Rectangle(iLeft, iTop, iRight, iBottom);
		}
		else //BAR_3D
		{
			Graf1Bar(pDC, iLeft, iTop, iRight, iBottom);
		}
		//popis
		pDC->SetBkColor(rgb);
		sizeText = pDC->GetTextExtent(MapBar[iBar].sPopis);
		pDC->TextOut(iLeft+(iRight-iLeft-sizeText.cy)/2,iBottom+2*m_carka+sizeText.cx, MapBar[iBar].sPopis);
	}

	if(pen.m_hObject)
	{
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}
	if(font)
	{
		pDC->SelectObject(pOldFont);
		font->DeleteObject();
	}
	if (brush)
	{
		pDC->SelectObject(pOldBrush );
		brush->DeleteObject();
	}

//	pDC->SetBkColor(rgb);
}

//kresli jeden 3D bar
//pDC - krslici zarizeni
//iLeft, iTop, iRight, iBottom - rozmery baru
void CGraf2::Graf1Bar(CDC* pDC, int iLeft, int iTop, int iRight, int iBottom)
{
	int k;
	CRgn   rgnA, rgnB;
	CPoint ptVertex[4];
	CBrush *brush;

	k = (int)((iRight-iLeft)/2/pow(2, 0.5));

	pDC->Rectangle(iLeft, iTop, iRight, iBottom);

	if (k>0)
	{
	/*	pDC->MoveTo(iLeft, iTop);
		pDC->LineTo(iLeft+k, iTop-k);
		pDC->LineTo(iRight+k, iTop-k);
		pDC->LineTo(iRight, iTop);
		pDC->MoveTo(iRight+k, iTop-k);
		pDC->LineTo(iRight+k, iBottom-k);
		pDC->LineTo(iRight, iBottom);*/

		//Vrsek
		ptVertex[0].x = iLeft;
		ptVertex[0].y = iTop;
		ptVertex[1].x = iLeft+k;
		ptVertex[1].y = iTop-k;
		ptVertex[2].x = iRight+k;
		ptVertex[2].y = iTop-k;
		ptVertex[3].x = iRight;
		ptVertex[3].y = iTop;

		//VERIFY
		rgnA.CreatePolygonRgn( ptVertex, 4, ALTERNATE);
		brush = GetBrushTop();
		if (brush)
		{
			pDC->FillRgn(&rgnA, brush);
			brush->DeleteObject();
		}

		//bok
		ptVertex[0].x = iRight+k;
		ptVertex[0].y = iTop-k;
		ptVertex[1].x = iRight+k;
		ptVertex[1].y = iBottom-k;
		ptVertex[2].x = iRight;
		ptVertex[2].y = iBottom;
		ptVertex[3].x = iRight;
		ptVertex[3].y = iTop;

		rgnB.CreatePolygonRgn( ptVertex, 4, ALTERNATE);
		brush = GetBrushRight();
		if (brush)
		{
			pDC->FillRgn( &rgnB, brush);
			brush->DeleteObject();
		}
	}
}

// podle pozice v pixelech vrati pozici v cislech
//i - v pixelech pozice
//b - TRUE  osa X
//  - FALSE osa Y
double CGraf2::GetOsyPosCis(int i, BOOL b)
{
	double d;

	if (b) //X
	{
		d = (m_x2-m_x1)/(m_rc.right-m_rc.left)*(i-m_rc.left)+m_x1;
	}
	else //Y
	{
		d = (m_y2-m_y1)/(m_rc.bottom-m_rc.top)*((m_rc.bottom-m_rc.top)-i)+m_y1;
	}

	return d;
}

//podle pozice v cislech vrati pozici v pixelech
int CGraf2::GetPosOsyPixX(double d)
{
	int x;

	x = (int)(GetPosOsy(d, m_x1, m_x2, m_rc.left, m_rc.right, TRUE)+0.5);

	return x;
}

//podle pozice v cislech vrati pozici v pixelech
int CGraf2::GetPosOsyPixY(double d)
{
	int y;

	y = (int)(GetPosOsy(d, m_y1, m_y2, m_rc.top, m_rc.bottom, FALSE)+0.5);

	return y;
}

//vrati minymalni a maximalni hodnty grafu (pouzit po SetOsyXY)
//pokud pro funci zadanou daty  data nenajde, tak navratova hodnota funkce je FALSE
BOOL CGraf2::GetMinMaxY(double x_min, double x_max, double *y_min, double *y_max)
{
	int ix;
	double x,y;
	int iPocetDat;
	BOOL bNalezeno;
	int i;

	*y_min = *y_max = 0;
	bNalezeno = FALSE;
	for (i=0; i<m_numFce; i++)
	{
		if (m_bFunkce[i])
		{
			m_x1 = x_min;
			m_x2 = x_max;

			for (ix=m_rc.left; ix<=m_rc.right; ix++)
			{
				if ((ix == m_rc.left) && (i==0))
				{
					x = GetOsyPosCis(ix, TRUE);
					*y_min = *y_max = Funkce(x,0);
				}
				else
				{
					x = GetOsyPosCis(ix, TRUE);
					y = Funkce(x,i);
					if (y < *y_min)
					{
						*y_min = y;
					}
					if (y > *y_max)
					{
						*y_max = y;
					}
				}
			}
			bNalezeno = TRUE;
		}
		else
		{
			iPocetDat = (int)MapBody[i].GetSize();
			for (ix=0; ix<iPocetDat; ix++)
			{
				if (x_max < MapBody[i][ix].x)
				{
					break;
				}
				if (bNalezeno)
				{
					if (MapBody[i][ix].y < *y_min)
					{
						*y_min = MapBody[i][ix].y;
					}
					if (MapBody[i][ix].y > *y_max)
					{
						*y_max = MapBody[i][ix].y;
					}
				}
				if ((!bNalezeno)&&(MapBody[i][ix].x >= x_min))
				{
					bNalezeno = TRUE;
					*y_min = *y_max = MapBody[i][ix].y;
				}
			}
		}
	}
	return bNalezeno;
}

//nastavi nadpis u grafu a jeho pozici
void CGraf2::SetCaption(UINT nCap, int ix, int iy)
{
	CFont *font;
	CFont *pOldFont;
	CSize sizeText;
	CPaintDC dc(this); // device context for painting

	m_sCaption.LoadString(nCap);
	m_ixCap = ix;
	m_iyCap = iy;

	//zjisteni vysky nadpisu a upraveni m_rc
	font = GetFontCaption();
	if (font)
	{
		pOldFont = dc.SelectObject(font);
	}

	sizeText = dc.GetTextExtent(m_sCaption);

	m_rc.top += sizeText.cy;
	if (font)
	{
		dc.SelectObject(pOldFont);
		font->DeleteObject();
	}
}

//Kresli nadpis u grafu
void CGraf2::DrawCaption(CDC* pDC)
{
	CFont *font;
	CFont *pOldFont;

	font = GetFontCaption();
	if (font)
	{
		pOldFont = pDC->SelectObject(font);
	}

	pDC->TextOut(m_ixCap,m_iyCap, m_sCaption);

	if (font)
	{
		pDC->SelectObject(pOldFont);
		font->DeleteObject();
	}
}

//Nacte data, ma vyznam pro graf zaddany daty
//a zjisti z nacenych dat
//min a max pro delku a nerovnost
//sFileName - soubor s daty
//num - poradi v grafu, standartne 0, tj. v grafu je jedna funkce
void CGraf2::NactiData(CString sFileName, double *x_min, double *x_max, double *y_min, double *y_max, int num /*= 0*/)
{
	CFile file;
	CFileException e;
	double dataX, dataY;
	char c[1];
	CString str;
	ULONGLONG size;
	DWORD d;
	__int32 radek;
	//typ jestli jde o x nebo y; pravidelne se strida x, y
	__int16 typ; //x-0; y-1
	int iPoc; //Pocet prvky v CMap
	PointDounle point;
	BOOL bElse;

	m_bFunkce[num] = FALSE;
	iPoc = 0;

	//presypaci hodiny
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	m_sFileName = sFileName;
	if (!file.Open(sFileName,CFile::modeRead,&e))
		return;
	else
	{
		CFileStatus status;
		file.GetStatus(status);
		timeModified = status.m_mtime;
//		s = t.Format( "%d.%m.%Y %H:%M:%S" );

		//nacte Data
		size = file.GetLength();
		str = "";
		d = 0;
		typ = 0;
		radek = 0;
		while (d<size)
		{
			file.Read(c,1);
			d++;
			if ((c[0] != ' ')&&(c[0] != 9)&&(c[0] != ';')&&(c[0] != 10)&&(c[0] != 13))
			{
				if(c[0] == ',')
				{
					c[0] = '.';
				}
				str += c[0];
				bElse = FALSE;
			}
			else //else1
			{
				bElse = TRUE;
			}
			if ((d == size) && (!bElse))//doplneni aby to pralo i posledni hodnotu, ktera neni ukoncena entrem
			{
				bElse = TRUE;
			}
			if (bElse)
			{
				if (str.GetLength()>0) //ulozit cislo
				{
					switch(typ)
					{
						case 0: //x
						{
							dataX = _tstof(str);
							typ = 1;
						}
						break;
						case 1: //y
						{
							dataY = _tstof(str);
							typ = 0;
							radek++;
							point.x = dataX;
							point.y = dataY;
							MapBody[num].SetAt(iPoc, point);
							iPoc++;
							if (iPoc == 1)
							{
								*x_min = *x_max = dataX;
								*y_min = *y_max = dataY;
							}
							else
							{
								if (*x_min > dataX)
								{
									*x_min = dataX;
								}
								if (*x_max < dataX)
								{
									*x_max = dataX;
								}
								if (*y_min > dataY)
								{
									*y_min = dataY;
								}
								if (*y_max < dataY)
								{
									*y_max = dataY;
								}
							}
						}
						break;
					}
					str = "";
				}
			}//else1
		}
	}
	file.Close();

	SeradData();
	//presypaci hodiny jsou pryc
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
}

//Seradi data podle x
void CGraf2::SeradData()
{
	int i, j;
	int iPocetDat;
	int iKonec;
	PointDounle point;
	bool bTest; //Test pro zjisteni jestli jsou data serazena a neni je potreba radit dal
	int ii;

	for (ii=0; ii<m_numFce; ii++)
	{
		iPocetDat = (int)MapBody[ii].GetSize();

		for (j=1; j<iPocetDat; j++)
		{
			iKonec = (iPocetDat-j);
			bTest = true;
			for (i=0; i<iKonec; i++)
			{
				if (MapBody[ii][i].x>MapBody[ii][i+1].x)
				{
					point = MapBody[ii][i];
					MapBody[ii][i] = MapBody[ii][i+1];
					MapBody[ii][i+1] = point;
					bTest = false;
				}
			}
			if (bTest)
			{
				j = iPocetDat;//data jsou serazena
			}
		}
	}
	//MapBody
}

//Nastvi typ grafu
//FUNKCE - funkce(data) startni typ
//BAR    - sloupcovy graf
//BAR_3D - 3D sloupcovy graf
void CGraf2::SetTypGraf(int iTyp)
{
	m_iTypGraf = iTyp;
}

//Nastavi parametry sloupcove grafu a  prepocte y-souradnice
//nutne aby byli na vstupu y_min a y_max sprvne podle nich se rozdeli data do sloupcu
//iTyp - typ grafu, zde ma sysl BAR a BAR_3D (pri FUNKCE bude vracet nesmyslne y_min, y_max)
//iPocBar - pocet sloupecku v grafu
//y_min - na vystupu 0
//y_max - nevyssi pocet v jednom sloupci
BOOL CGraf2::SetBar(int iTyp, int iPocBar, double *y_min, double *y_max)
{
	double ymin, ymax;
	double dRozpeti; //rozpeti pro jeden dilek
//	double dYmin, dYmax; //konkretni hodnoty pro jeden dilek (dYmax-ten do intervalu, krome posledniho dilku nepatri)
	int iBar;
//	int iPocet; //Pocet prvku v jednom baru
	int iPocetDat;
	int i;
	CString str;

	//presypaci hodiny
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	MapInterval.RemoveAll();

	m_iTypGraf = iTyp; //typ grafu

	if (*y_min >= *y_max)
	{
		return FALSE;
	}

	ymin = *y_min;
	ymax = *y_max;
	dRozpeti = (ymax-ymin)/iPocBar;

	iPocetDat = (int)MapBody[0].GetSize();

	//Prvne si vytvori intervali
//	int sum = 0;
//	int sum1 = 0;
//	BOOL b;
	for(iBar=0; iBar<iPocBar; iBar++)
	{
		if ((iBar)==0)
		{
			MapInterval[iBar].yMin = ymin;
		}
		else
		{
			MapInterval[iBar].yMin = MapInterval[iBar-1].yMax;
		}
//		MapInterval[iBar].yMin = ymin+iBar*dRozpeti;
//		MapInterval[iBar].yMax = MapInterval[iBar].yMin+dRozpeti;
		if ((iBar+1)==iPocBar)
		{
//			MapInterval[iBar].yMax += 0.0001;//prodlemy se zaokrouhlovanim a posledni ivnterval obshuje i horni hranici
			MapInterval[iBar].yMax = ymax+0.0001;
		}
		else
		{
			MapInterval[iBar].yMax = MapInterval[iBar].yMin+dRozpeti;
		}
		str = GetString(MapInterval[iBar].yMin, dRozpeti);
		str += _T(" - ");
		str += GetString(MapInterval[iBar].yMax, dRozpeti);
		MapBar[iBar].iPocet = 0;
		MapBar[iBar].sPopis = str;
	}
	
	//projde data za zada pocty do jednotlivych intervalu
	//intervali museji byt po poradku
	for (i=0; i<iPocetDat; i++)
	{
//		b = FALSE;
		//vypocet do ktereho intervalu honota patri
		iBar = (int)((MapBody[0][i].y-ymin)/dRozpeti);
		while(((MapBody[0][i].y < MapInterval[iBar].yMin) && (iBar >= 0)) ||
			(iBar >= iPocBar))
		{
			iBar--;
		}
		while(((MapBody[0][i].y >= MapInterval[iBar].yMax) && (iBar<iPocBar)) ||
			(iBar < 0))
		{
			iBar++;
		}
		if ((MapInterval[iBar].yMin <= MapBody[0][i].y) && (MapBody[0][i].y < MapInterval[iBar].yMax))
		{
//			b = TRUE;
			MapBar[iBar].iPocet += 1;
//			sum1++;
			if (i == 0)
			{
				*y_max = MapBar[iBar].iPocet;
			}
			else
			{
				if (*y_max<MapBar[iBar].iPocet)
				{
					*y_max = MapBar[iBar].iPocet;
				}
			}
			iBar = iPocBar;
		}
/*		for(iBar=0; iBar<iPocBar; iBar++)
		{
			if ((MapInterval[iBar].yMin <= MapBody[i].y) && (MapBody[i].y < MapInterval[iBar].yMax))
			{
//				b = TRUE;
				MapBar[iBar].iPocet += 1;
//				sum1++;
				if (i == 0)
				{
					*y_max = MapBar[iBar].iPocet;
				}
				else
				{
					if (*y_max<MapBar[iBar].iPocet)
					{
						*y_max = MapBar[iBar].iPocet;
					}
				}
				iBar = iPocBar;
			}
		}*/
	}
/*	for(iBar=0; iBar<iPocBar; iBar++)
	{
		sum += MapBar[iBar].iPocet;
	}*/
//	int sum = 0;
/*	for(iBar=0; iBar<iPocBar; iBar++)
	{
		if ((iBar)==0)
		{
			dYmin = ymin;
		}
		else
		{
			dYmin = dYmax;
		}
//		dYmin = ymin+iBar*dRozpeti;
//		dYmax = dYmin+dRozpeti;
		if ((iBar+1)==iPocBar)
		{
//			dYmax += 0.0001;//prodlemy se zaokrouhlovanim a posledni ivntrval obshuje i horni hranici
			dYmax = ymax+0.0001;
		}
		else
		{
			dYmax = dYmin+dRozpeti;
		}
		iPocet = 0;
		for (i=0; i<iPocetDat; i++)
		{
			if ((dYmin <= MapBody[i].y) && (MapBody[i].y < dYmax))
			{
				iPocet++;
			}
		}
//		sum += iPocet;
		str = GetString(dYmin, dRozpeti);
		str += _T(" - ");
		str += GetString(dYmax, dRozpeti);
		MapBar[iBar].iPocet = iPocet;
		MapBar[iBar].sPopis = str;
		if (iBar == 0)
		{
			*y_max = iPocet;
		}
		else
		{
			if (*y_max<iPocet)
			{
				*y_max = iPocet;
			}
		}
	}*/
	*y_min = 0;

	m_iPocBar = iPocBar;

	//presypaci hodiny jsou pryc
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	return TRUE;
}

//Pro data kde x-je casova osa. Nastavi parametry sloupcove grafu a prepocte y-souradnice
//nutno predtim volat AddBarTime
//iTyp - typ grafu, zde ma sysl BAR a BAR_3D (pri FUNKCE bude vracet nesmyslne y_min, y_max)
//porGraf - pradi funkce, pro pripad, ze je v jednom grafu vice fci
void CGraf2::SetBarTime(int iTyp, double *y_min, double *y_max, int porGraf /*= 0*/)
{
	int iBar;
	int iPocetDat;
	int lPocet; //Pocet prvku v jednom baru
	int iPocBar; //Pocet baru-sloupecku
	int i;
	CString str;
	CTime t;

	//presypaci hodiny
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	m_iTypGraf = iTyp; //typ grafu

	iPocetDat = (int)MapBody[porGraf].GetSize();
	iPocBar = (int)MapIntervalT.GetSize();

	*y_min = *y_max = 0;
	//projde data za zada pocty do jednotlivych intervalu
	//intervali museji byt po poradku
	i = 0;
	for(iBar=0; iBar<iPocBar; iBar++)
	{
		lPocet = 0;
		if (i<iPocetDat)
		{
			t = (__int64)MapBody[porGraf][i].x;
		}
		while ((t < MapIntervalT[iBar].yMax)&&(i<iPocetDat))
		{
			if ((MapIntervalT[iBar].yMin <= t) && (t < MapIntervalT[iBar].yMax))
			{
				lPocet += (int)MapBody[porGraf][i].y;
			}
			i++;
			if (i<iPocetDat)
			{
				t = (__int64)MapBody[porGraf][i].x;
			}
		}
		str = GetTime(MapIntervalT[iBar].yMin);
		MapBar[iBar].iPocet = lPocet;
		MapBar[iBar].sPopis = str;
		if (*y_max < lPocet)
		{
			*y_max = lPocet;
		}
	}

	m_iPocBar = iPocBar;

	//presypaci hodiny jsou pryc
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
}

//vytvori bary-sloupecky podle casoveho typu (mesic, den, hodina)
//iTyp - typ casoveho udaje rok mesic, den, hodina, minuta; YEAR MONTH DAY HOUR MINUTE
//tMin - dolni hranice casovych rozpeti
//tMax - horni hranice casovych rozpeti
void CGraf2::AddBarTime(int iTyp, __int64 tMin, __int64 tMax)
{
	CTime t, t1, t2, t_min, t_max;
	int iBar;
//	CString s;

	t1 = tMin;
	t2 = tMax;
//	s = t1.Format( "%d.%m.%Y %H:%M:%S" );
//	s = t2.Format( "%d.%m.%Y %H:%M:%S" );

	MapIntervalT.RemoveAll();

	m_iTypTime = iTyp;
	switch(iTyp)
	{
		case YEAR: //0
		{
			t_min = CTime(t1.GetYear(), 1, 1, 0, 0, 0); 
			t_max = CTime(t2.GetYear(), 1, 1, 0, 0, 0);
			if (t_max<t2)
			{
				t_max = CTime(t2.GetYear()+1, 1, 1, 0, 0, 0);
			}
		}
		break;
		case MONTH: //1
		{
			t_min = CTime(t1.GetYear(), t1.GetMonth(), 1, 0, 0, 0); 
			t_max = CTime(t2.GetYear(), t2.GetMonth(), 1, 0, 0, 0);
			if (t_max<t2)
			{
				if (t2.GetMonth()<11)
				{
					t_max = CTime(t2.GetYear(), t2.GetMonth()+1, 1, 0, 0, 0);
				}
				else
				{
					t_max = CTime(t2.GetYear()+1, 1, 1, 0, 0, 0);
				}
			}
		}
		break;
		case DAY: //2
		{
			t_min = CTime(t1.GetYear(), t1.GetMonth(), t1.GetDay(), 0, 0, 0); 
			t_max = CTime(t2.GetYear(), t2.GetMonth(), t2.GetDay(), 0, 0, 0);
			if (t_max<t2)
			{
				t_max += 24*60*60;
			}
		}
		break;
		case HOUR: //3
		{
//			t_min = CTime(t1.GetYear(), t1.GetMonth(), t1.GetDay(), t1.GetHour(),  0,  0); 
//			t_max = CTime(t2.GetYear(), t2.GetMonth(), t2.GetDay(), t2.GetHour(), 59, 59); 
			t_min = CTime(t1.GetYear(), t1.GetMonth(), t1.GetDay(), t1.GetHour(), 0, 0); 
			t_max = CTime(t2.GetYear(), t2.GetMonth(), t2.GetDay(), t2.GetHour(), 0, 0);
			if (t_max<t2)
			{
				t_max += 60*60;
			}
		}
		break;
		case MINUTE: //4
		{
			t_min = CTime(t1.GetYear(), t1.GetMonth(), t1.GetDay(), t1.GetHour(), t1.GetMinute(), 0);
			t_max = CTime(t2.GetYear(), t2.GetMonth(), t2.GetDay(), t2.GetHour(), t2.GetMinute(), 0);
			if (t_max<t2)
			{
				t_max += 60;
			}
		}
		break;
		default:
		{
			t_min = t1; 
			t_max = t2; 
		}
		break;
	}

	t = t_min;
	iBar = 0;
	while (t<t_max)
	{
		MapIntervalT[iBar].yMin = t;
		switch(iTyp)
		{
			case YEAR: //0
			{
				t = CTime(t.GetYear()+1,  1,  1,  0,  0,  0); 
			}
			break;
			case MONTH: //1
			{
				if (t.GetMonth()<11)
				{
					t = CTime(t.GetYear(), t.GetMonth()+1, 1, 0, 0, 0);
				}
				else
				{
					t = CTime(t.GetYear()+1, 1, 1, 0, 0, 0);
				}
			}
			break;
			case DAY: //2
			{
				t += 24*60*60; 
			}
			break;
			case HOUR: //3
			{
				t += 60*60; 
			}
			break;
			case MINUTE: //4
			{
				t += 60; 
			}
			break;
			default:
			{
				t += 24*60*60; 
			}
			break;
		}
		MapIntervalT[iBar].yMax = t;
		iBar++;
	}
}

//Nastaveni vyplne sloupcu v grafu
CBrush *CGraf2::GetBrush()
{
	CBrush *pBrush = new(CBrush);

	//vypln pozadi
	pBrush->CreateHatchBrush(HS_DIAGCROSS, m_rgb[0]);

	return pBrush;
}

//Nastaveni vyplne vrchu u 3D sloupcu v grafu
CBrush *CGraf2::GetBrushTop()
{
	CBrush *pBrush = new(CBrush);

	//vypln vrchu
	VERIFY(pBrush->CreateSolidBrush( RGB(230, 140, 140) ));

	return pBrush;
}

//Nastaveni vyplne boku u 3D sloupcu v grafu
CBrush *CGraf2::GetBrushRight()
{
	CBrush *pBrush = new(CBrush);

	//vypln boku
	VERIFY(pBrush->CreateSolidBrush( RGB(128, 50, 50) ));

	return pBrush;
}

//Prevede cislo na string ve hodnem tvaru
//pocet desetinych mist pocile s log
//d - cislo, ktere se ma prevest
//dRozpeti - rozpeti intervalu
CString CGraf2::GetString(double d, double dRozpeti)
{
	CString str;
	double logar;
	int i;
	CString str1;

	logar = log10(dRozpeti);
	if (logar>=1)
	{
		str1 = "%.0f";
	}
	else
	{
		i = abs((int)floor(logar)-1);
		str1.Format(_T("%d"),i);
		str1 = _T("%.")+str1+_T("f");
	}

	str.Format(str1,d);

	return str;
}

//Fce neni potreba, lae je funkcni
//Jen pro BAR, BAR_3D
// Pokud mys je mimno jaky koliv bar vrati FALSE, jinak TRUE.
//point - bod kde je mys
//iPocet - pocet hodnot v baru, na kterem je mys.
/*BOOL CGraf2::GetVyber(CPoint point, int *iPocet)
{
	int iBar;
	BOOL b;
	int iPocBar;
	int iPixelXs; //misto v pixelech pro m_xs
	double dMezera;
	CRect rc;

	b = FALSE;

	iPocBar = (int)MapBar.GetSize();
	if ((m_iTypGraf == BAR) || (m_iTypGraf == BAR_3D)) //ma to smysl jen pro grafy s bary
	{
		iPixelXs = GetPosOsyPixX(m_xs);
		dMezera = (double)(m_rc.right-iPixelXs)/m_iPocBar;

		for(iBar=0; iBar<iPocBar; iBar++)
		{
			rc.top = GetPosOsyPixY(MapBar[iBar].iPocet);
			rc.bottom = GetPosOsyPixY(0);
			if (m_iTypGraf == BAR)
			{
				rc.left = iPixelXs + (int)(dMezera*iBar+dMezera/5);
				rc.right = rc.left+(int)(dMezera/5*3);
			}
			else //BAR_3D
			{
				rc.left = iPixelXs + (int)(dMezera*iBar+dMezera/4);
				rc.right = rc.left+(int)(dMezera/4*2);
			}
			if (rc.left>=rc.right)
			{
				rc.right = rc.left+1;
			}

			//zjisteni jestli je to tento bar
			if (rc.PtInRect(point))
			{
				b = TRUE;
				*iPocet = MapBar[iBar].iPocet;
				iBar = iPocBar;
			}
		}
	}

	return b;
}*/

//Jen pro BAR, BAR_3D
// Vrati pocet hodnot v baru a odelnik baru.
//iBar - poradove cilso sloupecku-baru, pocitano od 0
//rc - obdelnik sloupecku-baru.
int CGraf2::GetVyber(int iBar, CRect *rc)
{
	BOOL b;
	int iPixelXs; //misto v pixelech pro m_xs
	double dMezera;

	b = FALSE;
	CRect rc1;

	if ((m_iTypGraf == BAR) || (m_iTypGraf == BAR_3D)) //ma to smysl jen pro grafy s bary
	{
		iPixelXs = GetPosOsyPixX(m_xs);
		dMezera = (double)(m_rc.right-iPixelXs)/m_iPocBar;

		rc->top = GetPosOsyPixY(MapBar[iBar].iPocet);
		rc->bottom = GetPosOsyPixY(0);
		if (m_iTypGraf == BAR)
		{
			rc->left = iPixelXs + (int)(dMezera*iBar+dMezera/5);
			rc->right = rc->left+(int)(dMezera/5*3);
		}
		else //BAR_3D
		{
			rc->left = iPixelXs + (int)(dMezera*iBar+dMezera/4);
			rc->right = rc->left+(int)(dMezera/4*2);
		}
		if (rc->left>=rc->right)
		{
			rc->right = rc->left+1;
		}
	}

	return MapBar[iBar].iPocet;
}

//Vrati pocet sloupecku-baru v grafu
int CGraf2::GetSumBar()
{
	return (int)MapBar.GetSize();
}

//vrati timeModified - cas zmeny souboru
CTime CGraf2::GetTimeModified()
{
	return timeModified;
}

//zjisti jestli soubor byl zmenen (ten z fce NactiData)
BOOL CGraf2::GetChangeFile()
{
	CFileStatus status;
	CTime t;
	BOOL b;

	b = FALSE;
	if( CFile::GetStatus( m_sFileName, status ))
	{
		t = status.m_mtime;
		if (timeModified != t)
		{
			b = TRUE;
			timeModified = t;
		}
	}

	return b;
}

//nastavi pocet funkci v grafu (max 10)
void CGraf2::SetNumFce(int num)
{
	m_numFce = num;
}

//vrati casovy udaj podle toho jak se nastaveno casove rozpeti
CString CGraf2::GetTime(CTime t)
{
	CString str;

	switch(m_iTypTime)
	{
		case YEAR: //0
		{
			str = t.Format("%Y");
		}
		break;
		case MONTH: //1
		{
			str = t.Format("%Y/%m");
		}
		break;
		case DAY: //2
		{
			str = t.Format("%d.%m.%Y");
		}
		break;
		case HOUR: //3
		{
			str = t.Format("%d.%m.%Y %H");
		}
		break;
		case MINUTE: //4
		default:
		{
			str = t.Format("%d.%m.%Y %H:%M");
		}
		break;
	}

	return str;
}

//vrati casovy udaj podle toho jak jsou carky na ose daleko
CString CGraf2::GetTimeKomplet(CTime t)
{
	CString str;

	str = t.Format("%d.%m.%Y %H:%M");

	return str;
}

//vrati casovy udaj - datum
CString CGraf2::GetTimeKomplet1(CTime t)
{
	CString str;

	str = t.Format("%d.%m.%Y");

	return str;
}

//vrati casovy udaj - cas
CString CGraf2::GetTimeKomplet2(CTime t)
{
	CString str;

	str += t.Format("%H:%M");

	return str;
}