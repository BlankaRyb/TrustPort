#pragma once
#include "afxcmn.h"

//trida pro zalozky, ktere jsou umisteny nahore, dole, vlevo nebo vpravo.
//Maji velikost podle zadani a kazda zalozka muze mit ikonu
class CMyTabCtrl : public CTabCtrl
{
public:
	CImageList m_imageList;

	CMyTabCtrl(void);
	~CMyTabCtrl(void);

	void SetTypTable(int iTyp);
	void SetWayText(int iWayText);
	CRect GetRectNoneDialog(CRect rc, int iTyp, int height, int i); //vypocet obedlniku pro vlozeni child dialogu

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnPaint();

protected:
	//typ zalozek 
	// 0 - nahore
	// 1 - vlevo
	// 2 - dole
	// 3 - vpravo
	int m_iTyp;
	int m_iWayText; //smer textu v desetinach uhlu (90' = 900)

	void DrawMyTabItem(CDC* pDC, int ixItem, const CRect& rcPage, const CRect& rcItem);//kresleni zalozek
	void PaintItemTyp0(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv); //kresleni zalozky
	void PaintItemTyp1(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv); //kresleni zalozky
	void PaintItemTyp2(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv); //kresleni zalozky
	void PaintItemTyp3(CDC* pDC, const CRect& rcPage, const CRect& rcItem, int ixItem, bool bAktiv); //kresleni zalozky
	void PaintItemText(CDC* pDC, const CRect& rcItem, int ixItem); //text zalozce
	COLORREF SetColRect(); //barva pro ramecek
	COLORREF SetColIsert(); //barva pro vybrany list (oranzova)

private:
	enum { m_arc = 5}; //oblouk v zalozkach
};
