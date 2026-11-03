#pragma once
#include "afxwin.h"
#include <math.h>

#define FUNKCE         (int)0
#define BAR            (int)1
#define BAR_3D         (int)2
#define YEAR           (int)0
#define MONTH          (int)1
#define DAY            (int)2
#define HOUR           (int)3
#define MINUTE         (int)4

//spolecna trida pro 2D a 3D grafy
class CGraf :
	public CStatic
{
public:
	CGraf(void);
	~CGraf(void);
enum {m_mezera = 40}; //minymalni mezera mezi carkami na ose
enum {m_carka = 4}; //velikost carek na ose

protected:
	CString m_strOsaX; //popis osy X
	CString m_strOsaY; //popis osy Y
	int m_iTypTime; //typ rozpeti casu, -1 neni x-osa casova
	int a[11]; //skoky po casove ose pro minuty a sekundy (cim jde delit 60 bez zbytku)
	int b[7]; //skoky po casove ose pro hodiny (cim jde delit 24 bez zbytku)
	int c[7]; //skoky po casove ose pro dny (cim jde delit 30 bez zbytku)
	int d[5]; //skoky po casove ose pro mesice (cim jde delit 12 bez zbytku)
	int e[3]; //skoky po casove ose pro roky (cim jde delit 10 bez zbytku) nasobky roku max 10
	__int64 m_utc; //casovy posun vuci nultemu casu

	BOOL GetPodil(__int64 *l, int *typ); //vrati dalsi cislo z casove osy vhodne pro rozdeleni na dilky

public:
	double GetPosOsy(double pos, double od_, double do_, int i1, int i2, BOOL b); // vrati pozici na v pixelech
	double Distance(double od_, double do_, int i1, int i2); //Vrati vzdalenost mezi jednotlivymi dilku
	double GetIdealDistance(double d); //Vrati idealni vzdalenost (1,2,5,10,20,50,100...)
	__int64 DistanceTimeX(double od_, double do_, int i1, int i2); //Vrati vzdalenost mezi jednotlivymi dilku
	CString GetTextNumber(double d, double dilek); //vrati double cislo v rozumnem formatu ve tvaru CStringu
	CFont *GetFont();
	CFont *GetFont90(); //text pod uhlem 90
	CFont *GetFontCaption(); //Font pro nadpis
	double Zaokrouhli(double d); //kvuli problemum se zaokrouhlovanim
	double Get1Dilek(int *i, double xy1, double dilek); //Vrati prvni dilek na ose a typ jeho vykreslovani (i)
	double Get1DilekTime(int *i, double xy1, double dilek); //Vrati prvni dilek na casove ose a typ jeho vykreslovani (i)
	double Get1DilekSpolecne(int *i, double xy1, double dilek, double d); //Spolecna cast pro Get1Dilek, Get1DilekTime
	double Get1DilekVelkyTime(int *i, double xy1, double *dilek); //Vrati prvni dilek na casove ose a typ jeho vykreslovani (i), jen pro cas kdy dilek je minymalne velky jako mesic (60*60*24*30)
	double GetIntervalVelkyTime(double d, double dilek_puv); //cas se pro interval mesic a vetsi pocita podle postu dnu v mesici
	void SetLegendX(UINT sX); //Popis osy X
	void SetLegendY(UINT sY); //Popis osy Y
};

