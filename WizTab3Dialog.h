#if !defined(AFX_WIZTAB3DIALOG_H__90EBC9C2_78A9_433D_BAAF_3548B788E407__INCLUDED_)
#define AFX_WIZTAB3DIALOG_H__90EBC9C2_78A9_433D_BAAF_3548B788E407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizTab3Dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizTab3Dialog dialog

class CWizTab3Dialog : public CDialog
{
// Construction
public:
	CWizTab3Dialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizTab3Dialog)
	enum { IDD = IDD_DIALOG_WIZ_TAB3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizTab3Dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWizTab3Dialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZTAB3DIALOG_H__90EBC9C2_78A9_433D_BAAF_3548B788E407__INCLUDED_)
