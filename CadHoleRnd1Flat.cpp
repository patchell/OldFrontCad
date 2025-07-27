// CadHoleRnd1Flat.cpp: implementation of the CCadHoleRnd1Flat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CCadHoleRnd1Flat::m_RenderEnable = 1;

CCadHoleRnd1Flat::CCadHoleRnd1Flat():CCadObject(OBJECT_TYPE_HOLERND1FLAT)
{
	m_pPenLine = 0;
}

CCadHoleRnd1Flat::CCadHoleRnd1Flat(CCadHoleRnd1Flat &h):CCadObject(OBJECT_TYPE_HOLERND1FLAT)
{
	m_pPenLine = 0;
	SetP1(h.GetP1());
	SetP2(h.GetP2());
	m_FlatDist = h.m_FlatDist;
	m_LineColor = h.m_LineColor;
	m_Radius = h.m_Radius;
	m_Width = h.m_Width;
}

CCadHoleRnd1Flat::~CCadHoleRnd1Flat()
{
	if(m_pPenLine) delete m_pPenLine;
}

void CCadHoleRnd1Flat::Draw(CDC *pDC, int mode,CPoint Offset,CScale Scale)
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
	int Rad;
	CPoint start,end;
	CPoint P1,P2;
	int Lw,Fd;

	if (CCadHoleRnd1Flat::m_RenderEnable)
	{
		P1 = Scale * GetP1() + Offset;
		dx = int(Scale.m_ScaleX * dx);
		Rad = dx;
		Lw = int(Scale.m_ScaleX * m_Width);
		Fd = int(Scale.m_ScaleX * m_FlatDist);
		if (Lw < 1) Lw = 1;

		rect.SetRect(P1 + CPoint(-dx, -dx), P1 + CPoint(dx, dx));
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
				m_pPenLine = new CPen(PS_SOLID, Lw, RGB(0, 250, 0));
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
			start.x = P1.x + Fd;
			start.y = int(SolveIntersection(1, P1, Fd, Rad));
			end.x = start.x;
			end.y = int(SolveIntersection(0, P1, Fd, Rad));
			pDC->Arc(&rect, start, end);
			pDC->MoveTo(P1.x + Fd, int(SolveIntersection(1, P1, Fd, Rad)));
			pDC->LineTo(P1.x + Fd, int(SolveIntersection(0, P1, Fd, Rad)));
			pDC->MoveTo(P1.x + dx, P1.y + dx);
			pDC->LineTo(P1.x - dx, P1.y - dx);
			pDC->MoveTo(P1.x - dx, P1.y + dx);
			pDC->LineTo(P1.x + dx, P1.y - dx);

			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		SetLastMode(mode);
	}
}

double CCadHoleRnd1Flat::SolveIntersection(int m,CPoint P1,int FlatDist,int Radius)
{
	double Y,T,Xd,R;

	Xd = double(FlatDist);
	R = double(Radius);
	Y = double(P1.y);
	if(m)
		T = Y - sqrt(R*R-Xd*Xd);
	else
		T = Y + sqrt(R*R-Xd*Xd);
	return T;
}

int CCadHoleRnd1Flat::CheckSelected(CPoint p,CSize O)
{
	int dx,dy;
	CPoint P1 = GetP1() + O;
	dx = this->m_FlatDist;
	dy = this->m_Radius;
	CRect rect(P1 + CPoint(-dy,-dy),P1+CPoint(dx,dy));
	rect.NormalizeRect();
	return rect.PtInRect(p);
}


CCadHoleRnd1Flat CCadHoleRnd1Flat::operator=(CCadHoleRnd1Flat &v)
{
	SetP1(v.GetP1());
	SetP2(v.GetP2());
	m_FlatDist = v.m_FlatDist;
	m_LineColor = v.m_LineColor;
	m_Radius = v.m_Radius;
	m_Width = v.m_Width;
	return *this;
}


void CCadHoleRnd1Flat::Move(CPoint p)
{
	SetP1(p);
}

int CCadHoleRnd1Flat::Parse(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser)
{
	LookAHeadToken = pParser->Expect(TOKEN_HOLE_1FLAT, LookAHeadToken, pIN);
	LookAHeadToken = pParser->Expect('(', LookAHeadToken, pIN);
	LookAHeadToken = pParser->Point(TOKEN_POINT_1, pIN, GetP1(), LookAHeadToken);
	LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
	LookAHeadToken = pParser->DecimalValue(TOKEN_RADIUS, pIN, m_Radius, LookAHeadToken);
	LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
	LookAHeadToken = pParser->DecimalValue(TOKEN_FLATDIST, pIN, m_FlatDist, LookAHeadToken);
	LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
	LookAHeadToken = pParser->Color(TOKEN_LINE_COLOR, pIN, m_LineColor, LookAHeadToken);
	LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
	LookAHeadToken = pParser->DecimalValue(TOKEN_LINE_WIDTH, pIN, m_Width, LookAHeadToken);
	LookAHeadToken = pParser->Expect(')', LookAHeadToken, pIN);
	(*ppDrawing)->AddObject(this);
	return LookAHeadToken;
}

void CCadHoleRnd1Flat::Save(FILE* pO, int Indent)
{
	char* s = new char[256];
	char* s1 = new char[64];
	char* s2 = new char[64];
	char* s3 = new char[64];
	char* s4 = new char[64];
	char* s5 = new char[64];

	fprintf(pO, "%s%s(%s,%s,%s,%s,%s)\n",
		theApp.IndentString(s, 256, Indent),
		CFileParser::TokenLookup(TOKEN_HOLE_1FLAT),
		CFileParser::SavePoint(s1, 64, TOKEN_POINT_1, GetP1()),
		CFileParser::SaveDecimalValue(s2, 64, TOKEN_RADIUS, m_Radius),
		CFileParser::SaveDecimalValue(s3,64, TOKEN_FLATDIST,m_FlatDist),
		CFileParser::SaveColor(s4,64,m_LineColor,TOKEN_LINE_COLOR),
		CFileParser::SaveDecimalValue(s5,64,TOKEN_LINE_WIDTH,m_Width)
	);
	delete[]s5;
	delete[]s4;
	delete[]s3;
	delete[]s2;
	delete[]s1;
	delete[]s;
}

int CCadHoleRnd1Flat::GrabVertex(CPoint point)
{
	return -1;
}

void CCadHoleRnd1Flat::SetVertex(int Vi, CPoint p)
{

}

CPoint CCadHoleRnd1Flat::GetReference()
{
	return GetP1();
}

void CCadHoleRnd1Flat::AdjustRefernce(CPoint p)
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


CRect CCadHoleRnd1Flat::GetRect()
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

void CCadHoleRnd1Flat::RenderEnable(int e)
{
	CCadHoleRnd1Flat::m_RenderEnable = e;
}

CPoint CCadHoleRnd1Flat::GetCenter()
{
	return GetP1();
}

// Moves the center of the object to the spcified point
void CCadHoleRnd1Flat::ChangeCenter(CSize p)
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


CSize CCadHoleRnd1Flat::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadHoleRnd1Flat::ChangeSize(CSize Sz)
{
	m_Radius = Sz.cx / 2;
}
