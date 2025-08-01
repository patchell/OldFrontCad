// HorzScrollBar.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void DefaultFormat(void *p,char *s,int l,int v)
{
	//--------------------------------------------------
	// displays text value in a string
	// this is the default callback function
	//
	// parameter:
	//	p.........user defined parameter, dependent on callback function
	//	s.........points to the character array where text is going to be put
	//	l.........maximum size of character array where text is put
	//	v.........value to display...somehow
	//----------------------------------------------------
	sprintf_s(s,l,"%d",v);	//default formating
}

/////////////////////////////////////////////////////////////////////////////
// CHorzScrollBar

CHorzScrollBar::CHorzScrollBar()
{
	FormatFunction = DefaultFormat;			//make sure pointers are initialized
	EditFormatFunction = DefaultFormat;
	Static_TextDisplay = 0;		//to zero
	CEdit_DisplayText = 0;
	StaticDebugDisplay = 0;
	lineinc = 1;
	pS = new char[256];
	LongMasterValue = 0;
	IntMasterValue = 0;
	pParam = 0;
	ActionMessage = 0;
	pos = 0;
	m_pWmsg = 0;
}

CHorzScrollBar::~CHorzScrollBar()
{
	delete[] pS;
}


BEGIN_MESSAGE_MAP(CHorzScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CHorzScrollBar)
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHorzScrollBar message handlers

void CHorzScrollBar::HScroll(UINT nSBCode, UINT nPos) 
{
	pS[0]=0;
	pos = GetScrollPos();
	int min,max;
	GetScrollRange(&min,&max);
	switch (nSBCode)
	{
		case SB_BOTTOM:
			pos = min;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEDOWN:
			pos += lineinc;
			if(pos > max)pos = max;
			SetScrollPos(pos);	//set scrollbar position
			break;
		case SB_LINEUP:
			pos -= lineinc;
			if(pos < min) pos = min;
			SetScrollPos(pos);	//set scrollbar position
			break;
		case SB_PAGEDOWN:
			pos += inc;
			if(pos > max) pos = max;
			SetScrollPos(pos);	//set scrollbar position
			break;
		case SB_PAGEUP:
			pos -= inc;
			if(pos < min) pos = min;
			SetScrollPos(pos);	//set scrollbar position
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			if(max < 32768)
				pos = nPos;
			SetScrollPos(pos);	//set scrollbar position
			break;
		case SB_TOP:
			pos = max;
			break;
	}
	if(StaticDebugDisplay)
	{
		sprintf_s(pS,256,"%0x",nPos);
		StaticDebugDisplay->SetWindowText(pS);
	}
}

void CHorzScrollBar::SetScrollPos(int v)
{
	if(v > this->max)
		v = max;
	pos = v;
	Modified = 1;
	if(m_pWmsg)
		m_pWmsg->PostMessageA(ActionMessage,(WPARAM)v,0l);
	if(LongMasterValue)
		*LongMasterValue = (long)pos;
	if(IntMasterValue)
		*IntMasterValue = pos;
	Update();
	CScrollBar::SetScrollPos(v);
}


void CHorzScrollBar::UpdateScrollPos(int v)
{
	pos = v;
	Modified = 1;
	if(LongMasterValue)
		*LongMasterValue = (long)pos;
	if(IntMasterValue)
		*IntMasterValue = pos;
	Update();
	CScrollBar::SetScrollPos(v);
}

void CHorzScrollBar::SetDisplayText(CStatic *pCStatic)
{
	Static_TextDisplay = pCStatic;	//set pointer to CStatic object to display string
}

void CHorzScrollBar::SetDisplayFormatFunction(void (__cdecl *pfunc)(void *,char *,int,int))
{
	FormatFunction = pfunc;
}

void CHorzScrollBar::SetDisplayEdit(CEdit *pE)
{
	CEdit_DisplayText = pE;
}

void CHorzScrollBar::SetEditDisplayFormatFunction(void (__cdecl *pfunc)(void *,char *,int,int))
{
	EditFormatFunction = pfunc;
}

void CHorzScrollBar::SetDebugDisplay(CStatic *d)
{
	StaticDebugDisplay = d;
}

void CHorzScrollBar::SetScrollRange(int minval, int maxval)
{
	min = minval;
	max = maxval;
	inc = (maxval - minval)/100;
	if(inc < 10) inc = 10;
	if(maxval > 32768)
	{
		lineinc = (maxval-minval)/1000;
	}
	CScrollBar::SetScrollRange(min,max);
}

/*
void CHorzScrollBar::SetNumEditDisplay(CNumEdit *pNE)
{
	CNumEdit_DisplayText = pNE;
}
*/

void CHorzScrollBar::Update()
{
	if(Static_TextDisplay)
	{
		FormatFunction(pParam,pS,256,pos);
		Static_TextDisplay->SetWindowText(pS);
	}
	if(CEdit_DisplayText)
	{
		EditFormatFunction(pParam,pS,256,pos);
		CEdit_DisplayText->SetWindowText(pS);
	}
}

void CHorzScrollBar::SetIntMasterValue(int *v)
{
	IntMasterValue = v;
}

void CHorzScrollBar::SetLongMasterValue(long *v)
{
	LongMasterValue = v;
}

void CHorzScrollBar::SetFormatParam(void *p)
{
	pParam = p;
}

void CHorzScrollBar::ClearModify()
{
	Modified = 0;
}

int CHorzScrollBar::IsModified()
{
	return Modified;
}

void CHorzScrollBar::SetActionMessage(CWnd *pW,UINT m)
{
	m_pWmsg = pW;
	ActionMessage = m;
}
