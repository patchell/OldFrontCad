// CadDrawing.cpp: implementation of the CCadDrawing class.
// This is a top level object for a CAD drawing
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

int CCadDrawing::m_RenderEnable = 1;

CCadDrawing::CCadDrawing():CCadObject(OBJECT_TYPE_DRAWING)
{
	m_Error = 0;
	m_pHead = 0;
	m_pTail = 0;
	m_BkColor = RGB(255,255,255);
}

CCadDrawing::~CCadDrawing()
{
	if(m_pHead)
	{
		CCadObject *pO = m_pHead;
		while(pO)
		{
			CCadObject *pOd = pO;
			pO = pO->GetNext();
			delete pOd;
		}
	}
}

void CCadDrawing::AddObject(CCadObject *pLO)
{
	///----------------------------------------
	/// AddObject
	///		This function is used to add a new
	/// object to a drawing.  It puts the
	/// object at the endof the list so
	/// that it will be drawwn last.
	///
	/// parameters:
	///		pLO.....pointer to CCad Object
	///----------------------------------------
	if(m_pHead == 0)	//nothing in drawing
	{
		m_pHead = pLO;
		m_pTail = pLO;
	}
	else				//add part to end of drawing
	{
		m_pTail->SetNext(pLO);
		pLO->SetPrev(m_pTail);
		m_pTail = pLO;
	}
}

void CCadDrawing::InsertObject(CCadObject *pLO)
{
	///----------------------------------------
	/// InsertObject
	///		This function is used to add a new
	/// object to a drawing.  It puts the
	/// object at the front of the list so
	/// that it will be drawwn first.
	///
	/// parameters:
	///		pLO.....pointer to CCad Object
	///----------------------------------------
	if(m_pHead == 0)	//nothing in drawing
	{
		m_pHead = pLO;
		m_pTail = pLO;
	}
	else				//add object to front
	{
		m_pHead->SetPrev(pLO);
		pLO->SetNext(m_pHead);
		m_pHead = pLO;
	}
}

void CCadDrawing::RemoveObject(CCadObject *pLO)
{
	///------------------------------------------
	/// RemoveObject
	///			This function removes an object
	/// from the drawing.  This is a DELETE
	///
	///	parameters:
	///		pLO.....pointer to object to remove
	///------------------------------------------
	if(pLO == m_pHead)
	{
		m_pHead = (CCadObject *)m_pHead->GetNext();
		if (m_pHead) m_pHead->SetPrev(0);
	}
	else if (pLO == m_pTail)
	{
		m_pTail = (CCadObject*)m_pTail->GetPrev();
		if (m_pTail) m_pTail->SetNext(0);
	}
	else
	{
		pLO->GetPrev()->SetNext(pLO->GetNext());
		pLO->GetNext()->SetPrev(pLO->GetPrev());
	}
	pLO->SetPrev(0);
	pLO->SetNext(0);
}

void CCadDrawing::Draw(CDC *pDC, int mode, CPoint Offset, CScale Scale)
{
	///-----------------------------------------
	/// Draw
	///		This is the function that draws the
	/// Drawing onto the specified device
	/// context.
	///	parameters:
	///		pDC.....pointer to display context
	///		mode....Ddrawing Mode
	///		Offset..Offset to add to objects
	///		Scale...Ammount to scale objects by
	///-----------------------------------------
	CCadObject *pCO = m_pHead;
	// TODO: add draw code for native data here
	while(pCO)
	{
		if(pCO->GetSelected()) 
			mode = OBJECT_MODE_SELECTED;
		else 
			mode = OBJECT_MODE_FINAL;
		pCO->Draw(pDC,mode,Offset,Scale);
		pCO = pCO->GetNext();
	}
}

void CCadDrawing::Save(FILE *pO,  int Indent)
{
	///-------------------------------------------
	///	Save
	///		This function is used to save the
	/// drawing to a computer file
	///
	///	parameter:
	///		pO.....pointer to file tto save to
	///-------------------------------------------
	char* s = new char[256];
	char* s1 = new char[64];

	CCadObject *pCO = m_pHead;
	fprintf(pO, "%sDRAWFILE( %s )\n%s{\n",
		theApp.IndentString(s, 256, Indent),
		CFileParser::SaveColor(s1, 64, m_BkColor, TOKEN_BACKGROUND_COLOR),
		s
	);
	while(pCO)
	{
		pCO->Save(pO, Indent+4);
		pCO = pCO->GetNext();
	}
	fprintf(pO,"%s}\n",s);
	delete[]s1;
	delete[]s;
}

void CCadDrawing::Print(CDC *pDC, int mode, CPoint Offset, CScale Scale)
{
	///-----------------------------------------
	/// Print
	///		This is the draw function for drawing
	///	on the printer.
	///
	///	parameters:
	///		pDC....device context to draw to
	///		mode...drawing mode
	///		Offset.offset to add to objects
	///		Scale.amount to Scale drawing by
	///-----------------------------------------
	CCadObject *pCO = m_pHead;
	while(pCO)
	{
		//---------------------------------
		// we really don't give a hang
		// what the mode is
		//---------------------------------
		pCO->Draw(pDC,mode,Offset,Scale);
		pCO = pCO->GetNext();
	}
}

int CCadDrawing::Parse(FILE* pIN, int LookAHeadToken, CCadDrawing** ppDrawing, CFileParser* pParser)
{
    return 0;
}

int CCadDrawing::CheckSelected(CPoint p, CCadObject **ppSelList, int n,int flag)
{
	///--------------------------------------
	/// CheckSelected
	///
	/// This function is used to see if
	/// there are any objects under the
	/// point.  This function returns the
	/// first 'n' objects it finds.  It does
	/// not actually select anything.
	///
	/// parameters:
	///		p......point of interest
	///		ppSelList...pointer to an array of selected objects
	///		n.....maximum number of objects to check for
	///		flag..what kind of objects are inlcuded in list
	///				0-All Objects
	///				1-Unselected Objects
	///				2-Selected Objects
	///	returns:
	///		number of objects that are under the point
	///--------------------------------------------
	int count = 0;
	CCadObject *pCO = m_pHead;

	while(pCO && ((count < n) || !n))
	{
		if(pCO->CheckSelected(p))
		{
			switch (flag)
			{
				case CADDRAWUBG_CHECKSEL_FLAG_ALL:
					if(ppSelList) ppSelList[count] = pCO;
					count++;	///increment count
					break;
				case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
					if (!pCO->GetSelected())
					{
						if (ppSelList) ppSelList[count] = pCO;
						count++;	///increment count
					}
					break;
				case CADDRAWUBG_CHECKSEL_FLAG_SEL:
					if (pCO->GetSelected())
					{
						if (ppSelList) ppSelList[count] = pCO;
						count++;	///increment count
					}
					break;
			}
		}
		pCO = pCO->GetNext();
	}
	return count;
}

void CCadDrawing::MakeDirty()
{
	///------------------------------------------
	/// MakeDirty
	///		This function is used to set the Dirty
	/// flag in all of the objects.  The Dirty
	/// flag forces the object to reset pens
	///------------------------------------------
	CCadObject *pObj = m_pHead;
	while(pObj)
	{
		pObj->MakeDirty();
		pObj = pObj->GetNext();
	}
}

CCadLibObject * CCadDrawing::CreatePartFromSelected(char *name)
{
	///------------------------------------------------
	/// CreatePartFromSelected
	///		Thhis function is used to create a library
	/// part from all of the objects in the drawing that
	/// are selected.
	///
	/// parameters:
	///		name....name of the new part
	/// returns:
	///		pointer to the CCadLibObject that will
	///		represent the new poart.
	///------------------------------------------------
	CCadObject *pObj = m_pHead;
	CCadLibObject *pPart;
	if(m_pHead == NULL)
		pPart = NULL;
	else
	{
		pPart = new CCadLibObject;
		pPart->SetName(name);
		while(pObj)
		{
			if(pObj->GetSelected())
			{
				switch(pObj->GetType())
				{
					case OBJECT_TYPE_NONE:
						break;
					case OBJECT_TYPE_LINE:
						{
							CCadLine *pO = new CCadLine;
							*pO = *((CCadLine *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_RECT:
						{
							CCadRect *pO = new CCadRect;
							*pO = *((CCadRect *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_ELLIPSE:
						{
							CCadElipse *pO = new CCadElipse;
							*pO = *((CCadElipse *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_RNDRECT:
						{
							CCadRoundRect *pO = new CCadRoundRect;
							*pO = *((CCadRoundRect *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_POLY:
						{
							CCadPolygon *pO = new CCadPolygon;
							*pO = *((CCadPolygon *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_POLYFILL:
						{
							CCadPolygonFill *pO = new CCadPolygonFill;
							*pO = *((CCadPolygonFill *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_LIBCOMP:
						{
						}
						break;
					case OBJECT_TYPE_HOLEROUND:
						{
							CCadHoleRound *pO = new CCadHoleRound;
							*pO = *((CCadHoleRound *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_HOLERECT:
						{
							CCadRectHole *pO = new CCadRectHole;
							*pO = *((CCadRectHole *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_HOLERND1FLAT:
						{
							CCadHoleRnd1Flat *pO = new CCadHoleRnd1Flat;
							*pO = *((CCadHoleRnd1Flat *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_HOLERND2FLAT:
						{
							CCadHoleRnd2Flat *pO = new CCadHoleRnd2Flat;
							*pO = *((CCadHoleRnd2Flat *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_TEXT:
						{
							CCadText *pO = new CCadText;
							*pO = *((CCadText *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_ARC:
						{
							CCadArc *pO = new CCadArc;
							*pO = *((CCadArc *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_ARCCENTERED:
						{
							CCadArcCentered *pO = new CCadArcCentered;
							*pO = *((CCadArcCentered *)pObj);
							pPart->AddObject(pO);
						}
						break;
					case OBJECT_TYPE_ARROW:
						{

						}
						break;
				}
			}
			pObj = pObj->GetNext();
		}
	}
	return pPart;
}

void CCadDrawing::CheckPrev()
{
	if(m_pTail->GetPrev() == 0)
	{
		m_Error++;
	}
}


CCadPrintRect *CCadDrawing::GetPrintRect()
{
	CCadObject *pObj;
	pObj = GetHead();
	int loop = 0;
	while (!loop && pObj)
	{
		if (OBJECT_TYPE_PRINTRECT == pObj->GetType())
		{
			loop = 1;
		}
		else
			pObj = pObj->GetNext();
	}
	return (CCadPrintRect *)pObj;
}
