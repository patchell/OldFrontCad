// CadPolygonFill.h: interface for the CCadPolygonFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADPOLYGONFILL_H__651BD531_2526_4268_936A_C9B80A183CD3__INCLUDED_)
#define AFX_CADPOLYGONFILL_H__651BD531_2526_4268_936A_C9B80A183CD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CadPolygon.h"

typedef struct {
	int m_Width;
	COLORREF m_LineColor;
	COLORREF m_FillColor;
}PolygonFillAttributes;

class CFileParser;

class CCadPolygonFill : public CCadPolygon  
{
	friend CFileParser;
	friend CCadPolygon;
public:
	virtual int Parse(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser);
	int PolyParams(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser);
	int Vertex(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser);
	int VertexList(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser);
	int VertexList1(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser);
	virtual void Save(FILE *pO,  int Indent);
	CCadPolygonFill();
	CCadPolygonFill(int size);
	virtual ~CCadPolygonFill();
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	void SetFillColor(COLORREF c){m_FillColor = c;}
	COLORREF GetFillColor(void){return m_FillColor;}
	CCadPolygonFill operator=(CCadPolygonFill &v);
	virtual CPoint GetCenter();
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
private:
	COLORREF m_FillColor;
	CBrush * m_pBrFill;
public:
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADPOLYGONFILL_H__651BD531_2526_4268_936A_C9B80A183CD3__INCLUDED_)
