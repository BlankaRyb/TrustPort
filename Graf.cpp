#include "StdAfx.h"
#include "Graf.h"

CGraf::CGraf(void)
{
	m_strOsaX = m_strOsaY = "";
	m_iTypTime = -1;

	//minuty
	a[0] = 2;
	a[1] = 3;
	a[2] = 4;
	a[3] = 5;
	a[4] = 6;
	a[5] = 10;
	a[6] = 12;
	a[7] = 15;
	a[8] = 20;
	a[9] = 30;
	a[10] = 60;

	//hodiny
	b[0] = 2;
	b[1] = 3;
	b[2] = 4;
	b[3] = 6;
	b[4] = 8;
	b[5] = 12;
	b[6] = 24;

	//dny
	c[0] = 2;
	c[1] = 3;
	c[2] = 5;
	c[3] = 6;
	c[4] = 10;
	c[5] = 15;
	c[6] = 30;

	//mesice
	d[0] = 2;
	d[1] = 3;
	d[2] = 4;
	d[3] = 6;
	d[4] = 12;

	//roky
	e[0] = 2;
	e[1] = 5;
	e[2] = 10;

	CTime t, t2;
	t = CTime(0);
	m_utc = t.GetMinute()*60;
	m_utc += t.GetHour()*60*60; 
}

CGraf::~CGraf(void)
{
}

// vrati pozici na v pixelech
//pro vypocet bodu, kde se osu krizi
//pos - pozice hledaneho mista na ose
//od_ - cislo kde osa zacina
//do_ - cislo kde osa konci
//i1  - v pixelech kde osa zacina
//i2  - v pixelech kde osa konci
//b   - smer osy
//    TRUE - od a do jdou stenym smerem jak i1 a i2
//    FALSE - od a do jsou opacnym slerem nez i1 a i2
double CGraf::GetPosOsy(double pos, double od_, double do_, int i1, int i2, BOOL b)
{
	double dHonota;

	dHonota = (i2-i1)/(do_-od_)*(pos-od_);

	if (b)
	{
		dHonota = i1+dHonota;
	}
	else //opacny smer na ose a na pocitani v pixelech
	{
		dHonota = i2-dHonota;
	}

	return dHonota;
}

//Vrati vzdalenost mezi jednotlivymi dilky
//od_ - cislo kde osa zacina
//do_ - cislo kde osa zacina
//i1  - odpovidajici hodnota v pixelech na zacatku
//i2  - odpovidajici hodnota v pixelech na konci
double CGraf::Distance(double od_, double do_, int i1, int i2)
{
	//>>vzdalenost car v grafu u kterych jsou hodnoty
	//nasobky 2, 5, 10
	double d;//, logar, cis;
//	double dilek;
	double vzdalenost;

//	vzdalenost = 0;
	d = (i2-i1)/m_mezera;

	if (d != 0)
	{
		d = (do_-od_)/d;
		vzdalenost = GetIdealDistance(d);
	}

/*	if (d != 0)
	{
		d = (do_-od_)/d;
		logar = log10(d);
		if (d>0)
		{
			logar = floor(logar);
		}
		else
		{
			logar = ceil(logar);
		}
		cis = pow(10,logar);
		vzdalenost = cis;
		if (d>cis)                      //*2
		{
			vzdalenost = 2*cis;
			if(d>vzdalenost)            //*5
			{
				vzdalenost = 5*cis;
				if(d>vzdalenost)        //*10
				{
					vzdalenost = 10*cis;
				}
			}
		}
	}
	else
	{
		vzdalenost = 0;
	}*/

	return vzdalenost;
}

//Vrati idealni zaokrouhlenou vzdalenost (1,2,5,10,20,50,100...)
//d - pozadovana minimalni vzdalenost, ale nezaokrouhlena
double CGraf::GetIdealDistance(double d)
{
	double logar, cis;
	double vzdalenost;

	if (d != 0)
	{
		logar = log10(d);
		if (d>0)
		{
			logar = floor(logar);
		}
		else
		{
			logar = ceil(logar);
		}
		cis = pow(10,logar);
		vzdalenost = cis;
		if (d>cis)                      //*2
		{
			vzdalenost = 2*cis;
			if(d>vzdalenost)            //*5
			{
				vzdalenost = 5*cis;
				if(d>vzdalenost)        //*10
				{
					vzdalenost = 10*cis;
				}
			}
		}
	}
	else
	{
		vzdalenost = 0;
	}

	return vzdalenost;
}

//Vrati vzdalenost mezi jednotlivymi dilky pro osu X, ktera je casova
//od_ - cislo kde osa zacina
//do_ - cislo kde osa zacina
//i1  - odpovidajici hodnota v pixelech na zacatku
//i2  - odpovidajici hodnota v pixelech na konci
__int64 CGraf::DistanceTimeX(double od_, double do_, int i1, int i2)
{
	//>>vzdalenost car v grafu u kterych jsou hodnoty
	//nasobky 2, 5, 10
	double d, dd;
	__int64 podil;
	int typ; //pro hledani vhodneho casoveho intervalu
	BOOL b;

	podil = 0;
	typ = 0;
	d = (i2-i1)/(m_mezera+20);
	b = TRUE;

	if (d != 0)
	{
		d = (do_-od_)/d;
		while ((d>podil) && (b))
		{
			if (typ == 4)
			{
				dd = GetIdealDistance((d/(60*60*24*365)));
				podil = (__int64)(dd*(60*60*24*365));
			}
			else
			{
				b = GetPodil(&podil, &typ);
			}
		}
	}

	return podil;
}

//vrati dalsi cislo z casove osy vhodne pro rozdeleni na dilky
BOOL CGraf::GetPodil(__int64 *l, int *typ)
{
	__int64 l1, l2;
	int i;
	BOOL bb;

	bb = TRUE;

	switch(*typ)
	{
		case 0:
		{
			l1 = l2 = 60;//minuty
		}
		break;
		case 1:
		{
			l1 = l2 = 60*60;//hodiny
		}
		break;
		case 2:
		{
			l1 = l2 = 60*60*24;//dny
		}
		break;
		case 3:
		{
			l1 = l2 = 60*60*24*30;//mesice
		}
		break;
		case 4:
		{
			l1 = l2 = 60*60*24*365;//roky
		}
		break;
		default:
		{
			l1 = l2 = 60;//minuty
		}
		break;
	}
	i = 0;
	while ((l2 <= *l) && (bb))
	{
		switch(*typ)
		{
			case 0:	//minuty
			{l2 = l1*a[i];}
			break;
			case 1: //hodiny
			{l2 = l1*b[i];}
			break;
			case 2: //dny
			{l2 = l1*c[i];}
			break;
			case 3: //mesice
			{l2 = l1*d[i];}
			break;
			case 4: //roky
			{
				l2 = l1*e[i];//nepouziva se
			}
			break;
			default:
			{l2 = l1*a[i];}
			break;
		}
		i++;
		switch(*typ)
		{
			case 0:
				if (i == 11)//60*60
				{
					*typ = 1;
				}
			break;
			case 1:
				if (i == 7)//60*60*24
				{
					*typ = 2;
				}
			break;
			case 2:
				if (i == 7)//60*60*24*30
				{
					*typ = 3;
				}
			break;
			case 3:
				if (i == 5)//60*60*24*30*12
				{
					l2 = 60*60*24*365;//12 mesicu je rok
					*typ = 4;
				}
			break;
			case 4:
				if (i == 3)//60*60*24*365
				{
					if (l2 <= *l)
					{
						bb = FALSE;
					}
				}
			break;
		}
	}

	*l = l2;

	return bb;
}

//vrati double cislo v rozumnem formatu ve tvaru CStringu
//d - cilso, ktere se bude transformovat
//dilek - velikost dilku kdy se popisuje osa
CString CGraf::GetTextNumber(double d, double dilek)
{
	CString str, str1;
	double logar;
	int i;

	if (dilek == 0) 
	{
		logar = 0;
	}
	else
	{
		logar = log10(dilek);
	}
	if (logar>=0)
	{
		str1 = "%.1f";
	}
	else
	{
		logar -= 0.9999;
		i = abs((int)logar);
		str1.Format(_T("%d"),i);
		str1 = _T("%.")+str1+_T("f");
	}
	str.Format(str1,d);
	return str;
}

CFont *CGraf::GetFont()
{
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

//text pod uhlem 90
CFont *CGraf::GetFont90()
{
    CFont   *fontText = new(CFont);

	fontText->CreateFont(
			-13,                       // nHeight
			0,                         // nWidth
			900,                       // angle of escapement
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

CFont *CGraf::GetFontCaption()
{
    CFont   *fontText = new(CFont);

	fontText->CreateFont(
			-26,                       // nHeight
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

//kvuli problemum se zaokrouhlovanim
double CGraf::Zaokrouhli(double d)
{
	double dd = .00000000001;

	return d+dd;
}

//Vrati prvni dilek na ose a typ jeho vykreslovani (i)
//i - typ kresleni cary 1-4 kratka; 0 - dlouha
//xy1 - zacatek osy
//dilek - ciselna vzdalenost mezi dvme vekymi dilky na ose
double CGraf::Get1Dilek(int *i, double xy1, double dilek)
{
	double d;
	double dilek_m; //maly dilek
//	int cele;
//	double prvni_v; //prvni dilek kde bude dlouha cara
//	double d1;

	dilek_m = dilek/5;

	//dilky
	//zaokrohleni, nasledujici 2 radky
	d = (int)(xy1/dilek);
	if ((xy1<0) && (d>xy1))
	{
		d--;
	}
	d = d*dilek;
	
	d = Get1DilekSpolecne(i, xy1, dilek, d);

	return d;
}

//Vrati prvni dilek na casove ose a typ jeho vykreslovani (i)
//i - typ kresleni cary 1-4 kratka; 0 - dlouha
//xy1 - zacatek osy
//dilek - ciselna vzdalenost mezi dvme vekymi dilky na ose
double CGraf::Get1DilekTime(int *i, double xy1, double dilek)
{
	double d;
	double dilek_m; //maly dilek
//	int cele;
//	double prvni_v; //prvni dilek kde bude dlouha cara
//	double d1;

	d = 0;
	if ((60*60*24*30) > dilek)
	{
		dilek_m = dilek/5;

		//dilky
		//zaokrohleni, nasledujici 2 radky
		d = (int)(xy1/dilek);
		d = d*dilek - m_utc;

		d = Get1DilekSpolecne(i, xy1, dilek, d);
	}

	return d;
}

//Spolecna cast pro Get1Dilek, Get1DilekTime
//i - typ kresleni cary 1-4 kratka; 0 - dlouha
//xy1 - zacatek osy
//dilek - ciselna vzdalenost mezi dvme vekymi dilky na ose
//d - hodnota pro vypocet dilku
double CGraf::Get1DilekSpolecne(int *i, double xy1, double dilek, double d)
{
	double dilek_m; //maly dilek
//	int cele;
//	double prvni_v; //prvni dilek kde bude dlouha cara
//	double d1;
	CTime t;

	dilek_m = dilek/5;

	*i = 0;
	while (xy1 > d)
	{
		d += dilek_m;
		(*i)++;
	}
//	prvni_v = (int)(xy1/dilek);
//	prvni_v = prvni_v*dilek;
/*	if (xy1>prvni_v)
	{
		prvni_v+=dilek;
	}*/
	//vypocet i - poradi kdy se kresli dloha cara a kdy ne (dlouha pro i=0)
/*	cele = (int)(d/dilek);
	t = (__int64)d;
	str = t.Format("%d.%m.%Y %H:%M:%S");
	t = (__int64)cele;
	str = t.Format("%d.%m.%Y %H:%M:%S");
	t = (__int64)dilek;
	str = t.Format("%d.%m.%Y %H:%M:%S");
	if (d>=0)
	{
		d1 = d-cele*dilek;
	}
	else
	{
		d1 = (dilek+d)-cele*dilek;
	}
	d1 = Zaokrouhli(d1);//problemy se zaokrouhlovanim
	*i = (int)(d1/dilek_m);*/
	if (*i == 5)
	{
		*i = 0;
	}

	return d;
}

//Vrati prvni dilek na casove ose a typ jeho vykreslovani (i),
//jen pro cas kdy dilek je minymalne velky jako mesic (60*60*24*30)
//i - typ kresleni cary 1-4 kratka; 0 - dlouha
//xy1 - zacatek osy
//dilek - ciselna vzdalenost mezi dvme vekymi dilky na ose
double CGraf::Get1DilekVelkyTime(int *i, double xy1, double *dilek)
{
	double d;
	double dilekTime; //nahrada "dilek", u casove osy (mesice a vice) je kazdy dilek jiny
	double dilek_m; //maly dilek
	double prvni_v; //prvni dilek kde bude dlouha cara
	CTime t, t1, t2;
	int month, year, iPocMonth, iPocYear;

	t = (__int64)xy1;
	iPocMonth = 0;

	year = t.GetYear();
	if (*dilek < 60*60*24*365) //mensi nez rok
	{
		month = t.GetMonth();
		month--;
		iPocMonth = (int)*dilek/(60*60*24*30);
		//zaokrohleni, nasledujici 2 radky
		month = (int)month/iPocMonth;
		month = month*iPocMonth;
		month++;
		t1 = CTime(year, month, 1, 0, 0, 0);
		month += iPocMonth;
		if (month > 12)
		{
			month -=12;
			year++;
		}
		t2 = CTime(year, month, 1, 0, 0, 0);
	}
	else //roky
	{
		iPocYear = (int)(*dilek/(60*60*24*365));
		t1 = CTime(year, 1, 1, 0, 0, 0);
		t2 = CTime(year+iPocYear, 1, 1, 0, 0, 0);
	}
	dilekTime = (double)t2.GetTime()-t1.GetTime();
	*dilek = dilekTime;

	dilek_m = *dilek/5;

	//dilky
	prvni_v = (double)t2.GetTime();
	if (xy1>prvni_v)
	{
		prvni_v += *dilek;
	}
	d = prvni_v;
	*i = 5;
	while(d > xy1)
	{
		d -= dilek_m;
		(*i)--;
	}
	d += dilek_m;
	(*i)++;
	if (*i == 5)
	{
		*i = 0;
	}

	return d;
}

//cas se pro interval mesic a vetsi pocita podle postu dnu v mesici
double CGraf::GetIntervalVelkyTime(double d, double dilek_puv)
{
//	double d;
	double dilekTime; //nahrada "dilek", u casove osy (mesice a vice) je kazdy dilek jiny
//	double dilek_m; //maly dilek
//	double prvni_v; //prvni dilek kde bude dlouha cara
	CTime t, t1, t2;
	int month, year, iPocMonth, iPocYear;

	t = (__int64)d;
	iPocMonth = 0;

	year = t.GetYear();
	if (dilek_puv < 60*60*24*365) //mensi nez rok
	{
		month = t.GetMonth();
		month--;
		iPocMonth = (int)dilek_puv/(60*60*24*30);
		//zaokrohleni, nasledujici 2 radky
		month = (int)month/iPocMonth;
		month = month*iPocMonth;
		month++;
		t1 = CTime(year, month, 1, 0, 0, 0);
		month += iPocMonth;
		if (month > 12)
		{
			month -=12;
			year++;
		}
		t2 = CTime(year, month, 1, 0, 0, 0);
	}
	else //roky
	{
		iPocYear = (int)dilek_puv/(60*60*24*365);
		t1 = CTime(year, 1, 1, 0, 0, 0);
		t2 = CTime(year+iPocYear, 1, 1, 0, 0, 0);
	}
	dilekTime = (double)t2.GetTime()-t1.GetTime();

	return dilekTime;
}

//Popis osy X
void CGraf::SetLegendX(UINT sX)
{
	m_strOsaX.LoadString(sX);
}

//Popis osy Y
void CGraf::SetLegendY(UINT sY)
{
	m_strOsaY.LoadString(sY);
}
