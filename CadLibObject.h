// CadLibObject.h: interface for the CCadLibObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADLIBOBJECT_H__291CD60B_9372_480F_9754_8061ECEFF91F__INCLUDED_)
#define AFX_CADLIBOBJECT_H__291CD60B_9372_480F_9754_8061ECEFF91F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define OBJECT_MODE_FINAL		0
#define OBJECT_MODE_SELECTED	1
#define OBJECT_MODE_SKETCH		2

class CCadLibObject : public CCadObject  
{
	CPen* m_pRefPen;
	CPoint m_Ref;
	char* m_pName;
	CCadObject* m_pStart;
	CCadObject* m_pEnd;
public:
	static int m_RenderEnable;
	CCadLibObject();
	CCadLibObject(CCadObject* pChain);
	CCadLibObject(CCadLibObject& LibObj);
	virtual ~CCadLibObject();
	virtual CRect GetRect(void);
	void AdjustRefernce(CPoint Ref);
	virtual void MakeDirty(void);
	virtual CPoint GetReference();
	virtual void Move(CPoint p);
	virtual void SetVertex(int Vi,CPoint p);
	virtual int GrabVertex(CPoint p);
	virtual int Parse(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser);
	virtual void Save(FILE *pO,  int Indent);
	virtual void Draw(CDC *pDC,int mode=0,CPoint Offset=CPoint(0,0),CScale Scale=CScale(0.1,0.1));
	virtual void AddObject(CCadObject *pO);
	virtual void RemoveObject(CCadObject *pO);
	virtual CCadObject *GetHead(void){return m_pStart;}
	virtual void SetName(char *pName);
	virtual int CheckSelected(CPoint p, CSize Offset = CSize(0, 0));
	virtual void RenderEnable(int e);
	virtual CPoint GetCenter();
	CPoint& GetRef() { return m_Ref; }
	char* GetPartName() { return m_pName; }
	// Moves the center of the object to the spcified point
	virtual void ChangeCenter(CSize p);
	virtual CSize GetSize();
	virtual void ChangeSize(CSize Sz);
};

#endif // !defined(AFX_CADLIBOBJECT_H__291CD60B_9372_480F_9754_8061ECEFF91F__INCLUDED_)
