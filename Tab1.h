#pragma once
#include "afxwin.h"
#include "ClickText.h"
#include "ButtonMy.h"

// CTab1 dialog

class CTab1 : public CDialog
{
	DECLARE_DYNAMIC(CTab1)

public:
	CTab1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTab1();

// Dialog Data
	enum { IDD = IDD_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetScrollBarMy(int iMax, int iPoz); //Nastavi ScrollBar

	int m_1pozice; //pozice prvniho buttonu
	int m_iPage; //velikost stranky - v pixelech vyska dialogu
	int m_iLine; //velikost radku v pixelech

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CScrollBar m_ScrollBarMy;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CButtonMy m_butt;
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnDestroy();
};
#pragma once
