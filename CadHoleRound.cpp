// CadHoleRound.cpp: implementation of the CCadHoleRound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CCadHoleRound::m_RenderEnable = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadHoleRound::CCadHoleRound():CCadObject(OBJECT_TYPE_HOLEROUND)
{
	m_pPenLine = 0;
}

CCadHoleRound::CCadHoleRound(CCadHoleRound &h):CCadObject(OBJECT_TYPE_HOLEROUND)
{
	m_pPenLine = 0;
	SetP1(h.GetP1());
	SetP2(h.GetP2());
	m_LineColor = h.m_LineColor;
	m_Radius = h.m_Radius;
	m_Width = h.m_Width;
}

CCadHoleRound::~CCadHoleRound()
{
	if(m_pPenLine) delete m_pPenLine;
}

void CCadHoleRound::Draw(CDC *pDC, int mode,CPoint O,CScale Scale)
{
	///---------------------------------------------
	///	Draw
	///		This function draws the object onto the
	///	specified device context.
	///
	/// parameters:
	///		pDC......pointer to the device context
	///		mode.....mode to use when drawing
	///		Offset...Offset to add to points
	///		Scale....Sets Units to Pixels ratio
	///---------------------------------------------
	CPen *pOld;
	CRect rect;
	int dx = m_Radius;
	CPoint start;
	CPoint P1,P2;
	CBrush* pOldBrush = 0, brushFill;
	int Lw;

	if (CCadHoleRound::m_RenderEnable)
	{
		brushFill.CreateSolidBrush(RGB(255, 255, 255));
		P1 = Scale * GetP1() + O;
		dx = int(Scale.m_ScaleX * dx);
		Lw = int(Scale.m_ScaleX * m_Width);
		if (Lw < 1) Lw = 1;

		rect.SetRect(P1 + CPoint(-dx, -dx), P1 + CPoint(dx, dx));
		start = P1 + CPoint(dx, 0);
		dx = dx / 2;

		if (GetLastMode() != mode || GetDirty())
		{
			if (m_pPenLine) delete m_pPenLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pPenLine = new CPen(PS_SOLID, Lw, m_LineColor);
				break;
			case OBJECT_MODE_SELECTED:
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(0, 255, 0));
				break;
			case OBJECT_MODE_SKETCH:
				m_pPenLine = new CPen(PS_SOLID, 1, m_LineColor);
				break;
			}
			SetDirty(0);
		}
		switch (mode)
		{
		case OBJECT_MODE_FINAL:
		case OBJECT_MODE_SELECTED:
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pPenLine);
			pOldBrush = pDC->SelectObject(&brushFill);
			pDC->Ellipse(&rect);
//			pDC->Arc(&rect, start, start);
			pDC->MoveTo(P1 + CPoint(dx, dx));
			pDC->LineTo(P1 + CPoint(-dx, -dx));
			pDC->MoveTo(P1 + CPoint(-dx, dx));
			pDC->LineTo(P1 + CPoint(dx, -dx));
			pDC->SelectObject(pOld);
			pDC->SelectObject(pOldBrush);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		SetLastMode(mode);
	}
}

int CCadHoleRound::CheckSelected(CPoint p,CSize O)
{
	double a,b,xo,yo,v;
	int rV;
	static int count = 0;

	CPoint P1 = GetP1() + O;
	a = double(m_Radius);
	b = double(m_Radius);
	xo = double(p.x) - double(P1.x);
	yo = double(p.y) - double(P1.y);
	v = (xo * xo)/(a * a) + (yo * yo)/(b * b);
	if( v < 1.0) rV = TRUE;
	else rV = FALSE;
	return rV;
}


CCadHoleRound CCadHoleRound::operator=(CCadHoleRound &v)
{
	SetP1(v.GetP1());
	SetP2(v.GetP2());
	m_LineColor = v.m_LineColor;
	m_Radius = v.m_Radius;
	m_Width = v.m_Width;
	return *this;
}

void CCadHoleRound::Move(CPoint p)
{
	SetP1(p);
}

int CCadHoleRound::Parse(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser)
{
	LookAHeadToken = pParser->Expect(TOKEN_HOLERND, LookAHeadToken, pIN);
	LookAHeadToken = pParser->Expect('(', LookAHeadToken, pIN);
	LookAHeadToken = pParser->Point(TOKEN_POINT_1, pIN, GetP1(), LookAHeadToken);
	LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
	LookAHeadToken = pParser->DecimalValue(TOKEN_RADIUS, pIN, m_Radius, LookAHeadToken);
	LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
	LookAHeadToken = pParser->Color(TOKEN_LINE_COLOR, pIN, m_LineColor, LookAHeadToken);
	LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
	LookAHeadToken = pParser->DecimalValue(TOKEN_LINE_WIDTH, pIN, m_Width, LookAHeadToken);
	LookAHeadToken = pParser->Expect(')', LookAHeadToken, pIN);
	(*ppDrawing)->AddObject(this);
	return LookAHeadToken;
}

void CCadHoleRound::Save(FILE* pO, int Indent)
{
	char* s = new char[256];
	char* s1 = new char[64];
	char* s2 = new char[64];
	char* s3 = new char[64];
	char* s4 = new char[64];

	fprintf(pO, "%s%s(%s,%s,%s,%s)\n",
		theApp.IndentString(s, 256, Indent),
		CFileParser::TokenLookup(TOKEN_HOLERND),
		CFileParser::SavePoint(s1,64,TOKEN_POINT_1,GetP1()),
		CFileParser::SaveDecimalValue(s2,64,TOKEN_RADIUS,m_Radius),
		CFileParser::SaveColor(s3,64,m_LineColor,TOKEN_LINE_COLOR),
		CFileParser::SaveDecimalValue(s4,64,TOKEN_LINE_WIDTH,m_Width)
	);
	delete[]s4;
	delete[]s3;
	delete[]s2;
	delete[]s1;
	delete[]s;
}

int CCadHoleRound::GrabVertex(CPoint point)
{
	return -1;
}

void CCadHoleRound::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadHoleRound::GetReference()
{
	return GetP1();
}

void CCadHoleRound::AdjustRefernce(CPoint p)
{
	///-----------------------------------------
	///	AdjustRefernce
	///		Thhis function is used to normalize
	///	the location of points in the object
	/// relative to a point choseen on the
	/// drawing.
	///	parameters:
	///		p.....selected reference point
	///-----------------------------------------
	SetP1(GetP1() - p);
}

CRect CCadHoleRound::GetRect()
{
	CRect rect;
	CPoint p1,p2;
	int dx,dy;

	dx = m_Radius;
	dy = m_Radius;
	rect.SetRect(GetP1() + CPoint(dx, dy), GetP1() + CPoint(-dx, -dy));
	rect.NormalizeRect();
	return rect;
}

void CCadHoleRound::RenderEnable(int e)
{
	CCadHoleRound::m_RenderEnable = e;
}

CPoint CCadHoleRound::GetCenter()
{
	return GetP1();
}

// Moves the center of the object to the spcified point
void CCadHoleRound::ChangeCenter(CSize p)
{
	///-----------------------------------------
	///	ChangeCenter
	///		Thhis function is used to normalize
	///	the location of points in the object
	/// relative to a point choseen on the
	/// drawing.
	///	parameters:
	///		p.....selected reference point
	///-----------------------------------------
	SetP1(GetP1() - p);
}


CSize CCadHoleRound::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadHoleRound::ChangeSize(CSize Sz)
{
	m_Radius = Sz.cx / 2;
}
