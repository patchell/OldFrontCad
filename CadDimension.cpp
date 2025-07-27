#include "stdafx.h"


int CCadDimension::m_RenderEnable = 1;

CCadDimension::CCadDimension():CCadObject(OBJECT_TYPE_DIMENSION)
{
	m_pLine = 0;
	m_pText = new CCadText;
	TextAttributes* pAtrb;
	
	pAtrb = m_pText->GetAttributes();
	pAtrb->m_Angle = 0;
	pAtrb->m_BkColor = RGB(255, 255, 255);
	pAtrb->m_Color = RGB(0, 0, 0);
	pAtrb->m_FontHeight = 200;
	pAtrb->m_FontWidth = 0;
	pAtrb->m_Format = DT_BOTTOM | DT_SINGLELINE;
	pAtrb->m_pFontName = new char[LF_FACESIZE];
	strcpy_s(pAtrb->m_pFontName, LF_FACESIZE, "Arial");
	pAtrb->m_pText = new char[32];
	pAtrb->m_Transparent = 1;
	pAtrb->m_Weight = FW_DEMIBOLD;
}

CCadDimension::CCadDimension(CCadDimension &cd) :CCadObject(OBJECT_TYPE_DIMENSION)
{
	m_pLine = 0;
	SetP1(cd.GetP1());
	SetP2(cd.GetP2());
	m_pText = new CCadText(*cd.m_pText);
}

CCadDimension::~CCadDimension()
{
	if (m_pText) delete m_pText;
}

CCadDimension CCadDimension::operator=(CCadDimension &v)
{
	SetP1(v.GetP1());
	SetP2(v.GetP2());
	*m_pText = *v.m_pText;
	return *this;
}

void CCadDimension::Move(CPoint p)
{
	CPoint Diff;
	Diff = p - GetP1();
	SetP1(GetP1() + Diff);
	SetP2(GetP2() + Diff);
	m_pText->Move(m_pText->GetP1() + Diff);
}

int CCadDimension::Parse(
	FILE* pIN, 
	int LookAHeadToken, 
	CCadDrawing** ppDrawing, 
	CFileParser* pParser
)
{
	BOOL Loop = TRUE;
	char* s = new char[256];

	LookAHeadToken = pParser->Expect(TOKEN_DIMENSION, LookAHeadToken, pIN);
	LookAHeadToken = pParser->Expect('(', LookAHeadToken, pIN);
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case TOKEN_POINT_1:
			LookAHeadToken = pParser->Point(TOKEN_POINT_1, pIN, GetP1(), LookAHeadToken);
			break;
		case TOKEN_POINT_2:
			LookAHeadToken = pParser->Point(TOKEN_POINT_2, pIN, GetP2(), LookAHeadToken);
			break;
		case TOKEN_LINE_WIDTH:
			LookAHeadToken = pParser->DecimalValue(TOKEN_LINE_WIDTH, pIN, GetAttrib().m_Width, LookAHeadToken);
			break;
		case TOKEN_LINE_COLOR:
			LookAHeadToken = pParser->Color(TOKEN_LINE_COLOR, pIN, GetAttrib().m_Color, LookAHeadToken);
			break;
		case',':
			LookAHeadToken = pParser->Expect(',', LookAHeadToken, pIN);
			break;
		case')':
			LookAHeadToken = pParser->Expect(')', LookAHeadToken, pIN);
			break;
		case '{':
			LookAHeadToken = pParser->Expect('{', LookAHeadToken, pIN);
			LookAHeadToken = pParser->DrawObjects(pIN, LookAHeadToken, this);
			break;
		case '}':
			Loop = FALSE;
			break;
		default:
			sprintf_s(s, 256, "ERROR:Syntax Error LINE:%d Col:%d", pParser->GetLine(), pParser->GetCol());
			::MessageBoxA(NULL,s , "BooBoo", MB_OK);
			Loop = FALSE;
			break;
		}
	}
	LookAHeadToken = pParser->Expect('}', LookAHeadToken, pIN);
	(*ppDrawing)->AddObject(this);
	delete[] s;
	return LookAHeadToken;
}

void CCadDimension::Save(FILE *pO,  int Indent)
{
	char* s = new char[256];
	char* s1 = new char[64];
	char* s2 = new char[64];
	char* s3 = new char[64];
	char* s4 = new char[64];

	fprintf(pO, "%s%s(%s,%s,%s,%s){\n", 
		theApp.IndentString(s,256,Indent),
		CFileParser::TokenLookup(TOKEN_DIMENSION),
		CFileParser::SavePoint(s1,64,TOKEN_POINT_1, GetP1()),
		CFileParser::SavePoint(s2, 64, TOKEN_POINT_2, GetP2()),
		CFileParser::SaveDecimalValue(s3,64,TOKEN_LINE_WIDTH, m_Atrib.m_Width),
		CFileParser::SaveColor(s4,64, m_Atrib.m_Color,TOKEN_LINE_COLOR)
	);
	m_pText->Save(pO,Indent+4);
	fprintf(pO, "%s}\n",s);
	delete[]s4;
	delete[]s3;
	delete[]s2;
	delete[]s1;
	delete[]s;
}

void CCadDimension::SetVertex(int Vi, CPoint p)
{
}

int CCadDimension::GrabVertex(CPoint p)
{
	int rV = -1;
	return rV;
}

void CCadDimension::Draw(CDC *pDC, int mode, CPoint Offset, CScale Scale)
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
	CPoint P1, P2;
	int Lw;

	if (CCadDimension::m_RenderEnable)
	{
		P1 = (Scale * GetP1()) + Offset;
		P2 = (Scale * GetP2()) + Offset;
		Lw = int(Scale.m_ScaleX * m_Atrib.m_Width);
		if (Lw < 1) Lw = 1;
		if ((GetLastMode() != mode) || GetDirty())
		{
			if (m_pLine) delete m_pLine;
			switch (mode)
			{
			case OBJECT_MODE_FINAL:
				m_pLine = new CPen(PS_SOLID, Lw, m_Atrib.m_Color);
				break;
			case OBJECT_MODE_SELECTED:
				m_pLine = new CPen(PS_SOLID, Lw, RGB(0, 255, 50));
				break;
			case OBJECT_MODE_SKETCH:
				m_pLine = new CPen(PS_DOT, 1, RGB(0, 0, 255));
				break;
			}
		}
		switch (mode)
		{
		case OBJECT_MODE_SELECTED:
		case OBJECT_MODE_FINAL:
			pOld = pDC->SelectObject(m_pLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			m_pText->Draw(pDC, mode, Offset, Scale);
			break;
		case OBJECT_MODE_SKETCH:
			pOld = pDC->SelectObject(m_pLine);
			pDC->MoveTo(P1);
			pDC->LineTo(P2);
			pDC->SelectObject(pOld);
			break;
		case OBJECT_MODE_ERASE:
			break;
		}
		SetLastMode(mode);
	}
}

int CCadDimension::CheckSelected(CPoint p,CSize O)
{
	int rV = 0;
	CPoint P1 = GetP1() + O;
	CPoint P2 = GetP2() + O;
	int s1 = 0, s2 = 0;

	if (P1.x == P2.x)
	{
		s1 = 20;
		s2 = 0;
	}
	else if (P1.y == P2.y)
	{
		s2 = 20;
		s1 = 0;
	}
	CSize rOff = CSize(s1, s2);
	P1 += rOff;
	P2 -= rOff;
	CRect rect;
	rect.SetRect(P1, P2);
	rect.NormalizeRect();
	rV = rect.PtInRect(p);
	rV |= m_pText->CheckSelected(p);
	return rV;
}

CPoint CCadDimension::GetReference()
{
	return GetP1();
}

void CCadDimension::MakeDirty(void)
{
	SetDirty(1);
	if (m_pText)m_pText->MakeDirty();
}

void CCadDimension::SetSelected(int Flag)
{
	CCadObject::SetSelected(Flag);
	m_pText->SetSelected(Flag);
}

void CCadDimension::AdjustRefernce(CPoint Ref)
{
	///-----------------------------------------
	///	AdjustRefernce
	///		Thhis function is used to normalize
	///	the location of points in the object
	/// relative to a point choseen on the
	/// drawing.
	///	parameters:
	///		Ref.....selected reference point
	///-----------------------------------------
	SetP1(GetP1() - Ref);
	SetP2(GetP2() - Ref);
	m_pText->AdjustRefernce(Ref);
	CPoint Org = ((CFrontCadApp *)AfxGetApp())->GetMainView()->GetOrigin();
	UpdateText(Org);
}

CRect CCadDimension::GetRect(void)
{
	CRect rR;
	return rR;
}

void CCadDimension::UpdateText(CPoint Org)
{
	CPoint P1, P2;
	P1 = GetP1();
	P2 = GetP2();
	int Dim,ofx;
	CRect rect;
	CSize off;

	if (P1.x == P2.x)	//vertical facing dimension
	{
		Dim = P1.x - Org.x;
		SetValue(Dim, 3);
		m_pText->SetAngle(-900);
		rect = m_pText->GetRect();
		if (P1.y > P2.y)
			ofx = -(rect.Width() + 60);
		else
			ofx = 60;
		off = CSize(-rect.Height() / 2, -ofx);
	}
	else if (P1.y == P2.y)	//horizontal facing dimension
	{
		Dim = -(P2.y - Org.y);
		SetValue(Dim, 3);
		rect = m_pText->GetRect();
		if (P1.x > P2.x)
			ofx = rect.Width()+60;
		else
			ofx = -60;
		off = CSize(ofx, rect.Height() / 2);
	}
	m_pText->SetP1(P2 - off);
}

void CCadDimension::SetValue(int v, int dp)
{
	int Intpart, Fracpart, Div;

	Div = 1;

	int i;

	for (i = 0; i < dp; ++i)
		Div *= 10;
	Intpart = v / Div;
	Fracpart = v % Div;
	Fracpart = int(((double)Fracpart / (double)Div) * double(Div));
	char *s = new char[32];
	sprintf_s(s,32, "%d.%03d", Intpart,Fracpart);
	m_pText->SetText(s);
	delete[] s;

}
void CCadDimension::AddObject(CCadObject *pO)
{
	if (pO->GetType() == OBJECT_TYPE_TEXT)
	{
		m_pText = (CCadText *)pO;
	}
	else
	{
		MessageBox(NULL, "Only TEXT objects in DIMENSION", "ERROR", MB_OK | MB_ICONHAND);
	}
}

void CCadDimension::RemoveObject(CCadObject *pO)
{
	if (pO->GetType() == OBJECT_TYPE_TEXT)
	{
		delete m_pText;
		m_pText = 0;
	}
	else
	{
		MessageBox(NULL, "Remove Only TEXT objects in DIMENSION", "ERROR", MB_OK | MB_ICONHAND);
	}
}
void CCadDimension::RenderEnable(int e)
{
	CCadDimension::m_RenderEnable = e;
}


void CCadDimension::AlignToPoint(CPoint p)
{
	if (GetP1().x == GetP2().x)	//vertical dimension
	{
		SetP1X(p.x);
		SetP2X(p.x);
	}
	else if (GetP1().y == GetP2().y)	//Horizontal dimension
	{
		SetP1Y(p.y);
		SetP2Y(p.y);
	}
}

CPoint CCadDimension::GetCenter()
{
	return GetP1();
}

// Moves the center of the object to the spcified point
void CCadDimension::ChangeCenter(CSize p)
{
}


CSize CCadDimension::GetSize()
{
	CRect rect = GetRect();
	return rect.Size();
}


void CCadDimension::ChangeSize(CSize Sz)
{
}


void CCadDimension::SetFontName(char * pN)
{
	m_pText->SetFontName(pN);
}


char * CCadDimension::GetFontName()
{
	return m_pText->GetFontName();
}
