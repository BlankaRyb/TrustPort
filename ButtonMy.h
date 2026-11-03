#pragma once
#include "resource.h"
//#include "Add0.h"

//zobrazovani ruzneho postu textu, ktere se se po kliku zobrazuji v rozbalene forme
//Tato trida se pridava na CButton, jeho velikost a umisteni je libovolene,
//bude prepocteno podle zadani
struct DataText
{
	CString str1;//string v nadpisu
	CString str2;//string pod nadpisem
	HICON hIco1; //ikona pro zbaleny text
	HICON hIco2; //ikona pro rozbaleny text
	UINT nIDD; //dialog, ktery patri k rozbalovacimu textu, muze byt NULL
	CDialog* pDialog; //pro nacteni dialogu z nIDD
	CRect rc; //obdelnik v nemz je dialog; pro rychlost ulozena a zapamtovano v Add
	BOOL bRozbaleno; //TRUE - text je rozbalen; jinak zabalen
	int iHeight; //vyska pro text (str1+str2)
	int iPosY; //pozicet textu str1
	CRect recText; //obdelnik pro text v nadpisu, aby se neprovadel casto jejich vypocet
	CRect recIco; //obdelnik pro ikonu, aby se neprovadel casto jejich vypocet
};

class CButtonMy :
	public CStatic
{
public:
	CButtonMy(void);
	~CButtonMy(void);
	//Prida rozbalvaci button
	void Add(UINT nStr1, UINT nStr2, UINT nID1, UINT nID2, UINT nIDD);
	void SetWidth(int iWidth); //nastaveni sirky objektu - sirka mista pro text
	void SetInterval(int iInterval); //nastavi mezeru v pixelech mezi texty
	void SetSizeIco(int iSizeIco); //rozmer Ikony
	void SetPosText(CPoint pPixel); //posice textu
	void SetPosIco(CPoint pPixelIco); //pozice Ikony
	void Calculation(); //Vypocet vsech textu
	void SetPos(CPoint pPos); //nastaveni pozice buttonu; levy horni roh
	CPoint GetPos(); //Vrati pozici buttonu; levy horni roh
	void Click(CPoint point); //Spolecna funkce pro Click dubleClick
	int GetSizeLine(); //Vrati vysku radku pro text pod nadpisem
	CRect GetRectButt(); //Vrati odelnik buttonu
	BOOL GetClick(); //Vraci TRUE jestli bylo kliknuto na ikonu nebo text, jinak FALSE
	int GetChangeHeight(); //Vrati zmenu velikosti buttonu po poslednim kliku
	void SetParam(int i, UINT nIDD, CString str); //predani parametru
	CString GetParam(int i, UINT nIDD); //vraceni parametru

//	CAdd0 m_Add0;

private:
enum { m_interval = 0}; //pridavek na sirku buttonu

protected:
	int m_iPocet; //pocet prvku v CMap, maji poradova cisla od 0
	int m_iWidth; //sirka pro text a obdelnik
	int m_iInterval; //mezera v pixelech mezi texty
	int m_iSizeIco; //rozmer Ikony
	CPoint m_pPixel; //pozice textu v polozce
	CPoint m_pPixelIco; //Pozice Ikony
	CRect m_rc; //obelnik buttonu
	CPoint m_pPos; //pozice buttonu; levy horni roh
	BOOL m_bClickSelect; //Jestli bylo kliknuto na nejakou ikonu nebo text
	int m_iChangeHeight; //zde je zmena velikosti buttonu pri kliku//werty

	CFont *GetFont1(); //font v nadpisu
	CFont *GetFont2(); //font rozbalovaciho textu
	void DrawText(HDC hDC, int i); //kresli dva texty patrici k sobe
	CSize DrawText(HDC hDC, int x, int y, CFont *font, CString sText); //Nakresli test a vrati jeho rozmery
	CString DelMezery(CString s); //Odstrani ze stringu na zacatku a na konci 32 (' ')
	CString RozdelString(HDC hDC, CFont *font, CString s, CString *s1, SIZE *size); //Rozdeli string tak, aby navratova hodnota byla pri zobrazeni mela maximalni delku nebo jen jedno slovo
	CSize GetSizeText(CFont *font, CString sText); //vrati rozmery textu s ohledem na nutny pocet radku

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
