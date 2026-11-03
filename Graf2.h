#pragma once
#include "graf.h"

//Trida pro 2D grafy
//kresli grafy bud zadane funkci nebo daty
//pro grafy zadana funci je potreba tuto tridu zdedit a prepsat finkci "Funkce"
//pro grafy zadana daty se vola funkce "NactiData" kde se jako pramter dava cesta k souboru s daty\
//pro zobrazeni jineho nez klasickeho grafu volat funkci SetTypGraf a tam zadat typ grafu
//pro sloupcovy graf se pocet sloupcu zadava v SetBar

//pro nactena data se souboru
struct PointDounle 
{
	double x;
	double y;
};

//pro data rozdelana do Baru
struct BarData
{
	int iPocet;
	CString sPopis;
};

//intervali jednotlivych baru
struct IntervalMy
{
	double yMin;
	double yMax;
};

struct IntervalMyT
{
	CTime yMin;
	CTime yMax;
};

class CGraf2 :
	public CGraf
{
public:
	CGraf2(void);
	~CGraf2(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	CMap<int, int, PointDounle, PointDounle> MapBody[100]; //pro grafy zadany daty
	CMap<int, int, BarData, BarData> MapBar;//hodnoty pro sloucovy graf
	CMap<int, int, IntervalMy, IntervalMy> MapInterval;//hodnoty pro jednotlive intervali Baru-sloupcu
	CMap<int, int, IntervalMyT, IntervalMyT> MapIntervalT;//hodnoty pro jednotlive CASOVE intervali Baru-sloupcu

protected:
	double m_x1, m_x2; //rozpeti osy X
	double m_y1, m_y2; //rozpeti osy Y
	double m_xs, m_ys; //bod kde se osy krizi
	CRect m_rc; //obdelnik s grafem
	int m_rgb[100]; //barva grafu
	CString m_sCaption; //Nadpis u grafu
	int m_ixCap, m_iyCap; //Pozice nadpisu
	double *m_xxData, *m_yyData;  //vsechna data x-delka-osa x, y-funkce
	BOOL m_bFunkce[100]; //urcuje jak je zadana funkce; TRUE -funkci, FALSE - daty
	int m_iTypGraf; //Typ grafu 0 - funkce, 1 - sloupcovy graf, 2 - 3D sloupcovy graf
	int m_iPocBar; //Pocet sloupecku ve sloucovem grafu
	CTime timeModified; //cas zmeny souboru kde jsou data
	CString m_sFileName; //jmeno souboru ze ktereho se cte
	int m_numFce; //pocet funkci v grafu, standartne 1
//	int m_iTypTime; //typ rozpeti casu, -1 neni x-osa casova

	void Graf(CDC* pDC); //Kresli graf
	void GrafFunkce(CDC* pDC); //Kresli graf funce nebo zadany daty
	void GrafBar(CDC* pDC); //Kresli sloupcovy graf
	void Graf1Bar(CDC* pDC, int iLeft, int iTop, int iRight, int iBottom); //kresli jeden 3D bar
	void DrawCaption(CDC* pDC); //Kresli nadpis u grafu
	CBrush *GetBrush(); //Nastaveni vyplne sloupcu v grafu
	CBrush *GetBrushTop(); //Nastaveni vyplne vrchu u 3D sloupcu v grafu
	CBrush *GetBrushRight(); //Nastaveni vyplne boku u 3D sloupcu v grafu
	CString GetString(double d, double dRozpeti); //Prevede cislo na string ve hodnem tvaru
	CString GetTime(CTime t); //vrati casovy udaj podle toho jak se nastaveno casove rozpeti
	CString GetTimeKomplet(CTime t); //vrati casovy udaj podle toho jak jsou carky na ose daleko
	CString GetTimeKomplet1(CTime t); //vrati casovy udaj - datum
	CString GetTimeKomplet2(CTime t); //vrati casovy udaj - cas

public:
	void SetOsyXY(double x1, double x2, double y1, double y2); //nastavi rozpeti os
	void SetCentre(double xs, double ys); //nastavi pod kde se kreslene osy krizi
	void SetSizeRect(int left, int top, int right, int bottom); //nastaveni zmenseni obelniku s grafem
	void DrawOsy(CDC* pDC); //kresli osy x,y
	void DescriptionOsyX(CDC* pDC, int y); //kresli cila a carky na ose X
	void DescriptionOsyY(CDC* pDC, int x); //kresli cila a carky na ose Y
	virtual double Funkce(double x, int num = 0); //funkce ktera se kresli
	void SetColorGraf(int rgb, int num = 0);//nastaveni barvy grafu
	double GetOsyPosCis(int i, BOOL b); //podle pozice v pixelech vrati pozici v cislech
	int GetPosOsyPixX(double d); //podle pozice v cislech vrati pozici v pixelech
	int GetPosOsyPixY(double d); //podle pozice v cislech vrati pozici v pixelech
	BOOL GetMinMaxY(double x_min, double x_max, double *y_min, double *y_max); //vrati minymalni a maximalni hodnty grafu
	void SetCaption(UINT nCap, int ix, int iy); //nastavi nadpis u grafu
	void NactiData(CString sFileName, double *x_min, double *x_max, double *y_min, double *y_max, int num = 0);//Nacte data, ma vyznam pro graf zaddany daty
	void SeradData(); //Seradi data podle x
	//Nastvi typ grafu
	//FUNKCE - funkce(data) startni typ
	//BAR    - sloupcovy graf
	//BAR_3D - 3D sloupcovy graf
	void SetTypGraf(int iTyp);
	BOOL SetBar(int iTyp, int iPocBar, double *y_min, double *y_max); //Nastavi parametry sloupcove grafu a prepocte y-souradnice
	//Pro data kde x-je casova osa. Nastavi parametry sloupcove grafu a prepocte y-souradnice
	//nutno predtim volat AddBarTime
	void SetBarTime(int iTyp, double *y_min, double *y_max, int porGraf = 0);
	void AddBarTime(int iTyp, __int64 tMin, __int64 tMax); //vytvori bary-sloupecky podle casoveho typu (mesic, den, hodina)
	// Pokud mys je mimno jaky koliv bar vrati FALSE, jinak TRUE. V parametru vrati pocet hodnot v baru, na kterem je mys.
//	BOOL GetVyber(CPoint point, int *iPocet);
	// Vrati pocet hodnot v baru a odelnik baru.
	int GetVyber(int iBar, CRect *rc);
	int GetSumBar(); //Vrati pocet sloupecku-baru v grafu
	CTime GetTimeModified(); //vrati timeModified - cas zmeny souboru
	BOOL GetChangeFile(); //zjisti jestli soubor byl zmenen (ten z fce NactiData)
	void SetNumFce(int num); //nastavi pocet funkci v grafu (max 10)
};
