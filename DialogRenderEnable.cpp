// DialogRenderEnable.cpp : implementation file
//

#include "stdafx.h"

// CDialogRenderEnable dialog

IMPLEMENT_DYNAMIC(CDialogRenderEnable, CDialog)

CDialogRenderEnable::CDialogRenderEnable(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_RENDER_ENABLE, pParent)
{

}

CDialogRenderEnable::~CDialogRenderEnable()
{
}

void CDialogRenderEnable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ARCCENTEREDENABLE, m_Check_ArcCenteredEnable);
	DDX_Control(pDX, IDC_CHECK_ARCENABLE, m_Check_ArcEnable);
	DDX_Control(pDX, IDC_CHECK_ARROWENABLE, m_Check_ArrowEnable);
	DDX_Control(pDX, IDC_CHECK_DIMENSIONENABLE, m_Check_DimensionEnable);
	DDX_Control(pDX, IDC_CHECK_ELLISPEENABLE, m_Check_EllipseEnable);
	DDX_Control(pDX, IDC_CHECK_FILLEDPOLYENABLE, m_Check_FillPolygonEnable);
	DDX_Control(pDX, IDC_CHECK_LINEENABLE, m_Check_LineEnable);
	DDX_Control(pDX, IDC_CHECK_ONEFLATENABLE, m_Check_OneFlatHoleEnable);
	DDX_Control(pDX, IDC_CHECK_ORIGINENABLE, m_Check_OriginEnable);
	DDX_Control(pDX, IDC_CHECK_POLYGONENABLE, m_Check_PolygonEnable);
	DDX_Control(pDX, IDC_CHECK_RECTENABLE, m_Check_RectEnable);
	DDX_Control(pDX, IDC_CHECK_RECTHOLEENABLE, m_Check_RectHoleEnable);
	DDX_Control(pDX, IDC_CHECK_ROUNDEDRECTENABLE, m_CheckRoundedRectEnable);
	DDX_Control(pDX, IDC_CHECK_ROUNDHOLEENABLE, m_Check_RoundHole);
	DDX_Control(pDX, IDC_CHECK_TEXTENABLED, m_Check_TextEnable);
	DDX_Control(pDX, IDC_CHECK_TWOFLATENABLE, m_Check_TwoFlatRoundHoleEnable);
}


BEGIN_MESSAGE_MAP(CDialogRenderEnable, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ARCCENTEREDENABLE, &CDialogRenderEnable::OnClickedCheckArccenteredenable)
	ON_BN_CLICKED(IDC_CHECK_ARCENABLE, &CDialogRenderEnable::OnClickedCheckArcenable)
	ON_BN_CLICKED(IDC_CHECK_ARROWENABLE, &CDialogRenderEnable::OnClickedCheckArrowenable)
	ON_BN_CLICKED(IDC_CHECK_DIMENSIONENABLE, &CDialogRenderEnable::OnClickedCheckDimensionenable)
	ON_BN_CLICKED(IDC_CHECK_ELLISPEENABLE, &CDialogRenderEnable::OnClickedCheckEllispeenable)
	ON_BN_CLICKED(IDC_CHECK_FILLEDPOLYENABLE, &CDialogRenderEnable::OnClickedCheckFilledpolyenable)
	ON_BN_CLICKED(IDC_CHECK_LINEENABLE, &CDialogRenderEnable::OnClickedCheckLineenable)
	ON_BN_CLICKED(IDC_CHECK_ONEFLATENABLE, &CDialogRenderEnable::OnClickedCheckOneflatenable)
	ON_BN_CLICKED(IDC_CHECK_ORIGINENABLE, &CDialogRenderEnable::OnClickedCheckOriginenable)
	ON_BN_CLICKED(IDC_CHECK_POLYGONENABLE, &CDialogRenderEnable::OnClickedCheckPolygonenable)
	ON_BN_CLICKED(IDC_CHECK_RECTENABLE, &CDialogRenderEnable::OnClickedCheckRectenable)
	ON_BN_CLICKED(IDC_CHECK_RECTHOLEENABLE, &CDialogRenderEnable::OnClickedCheckRectholeenable)
	ON_BN_CLICKED(IDC_CHECK_ROUNDEDRECTENABLE, &CDialogRenderEnable::OnClickedCheckRoundedrectenable)
	ON_BN_CLICKED(IDC_CHECK_ROUNDHOLEENABLE, &CDialogRenderEnable::OnClickedCheckRoundholeenable)
	ON_BN_CLICKED(IDC_CHECK_TEXTENABLED, &CDialogRenderEnable::OnClickedCheckTextenabled)
	ON_BN_CLICKED(IDC_CHECK_TWOFLATENABLE, &CDialogRenderEnable::OnClickedCheckTwoflatenable)
END_MESSAGE_MAP()


// CDialogRenderEnable message handlers


void CDialogRenderEnable::OnClickedCheckArccenteredenable()
{
	CCadArcCentered::m_RenderEnable = m_Check_ArcCenteredEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckArcenable()
{
	CCadArc::m_RenderEnable = m_Check_ArcEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckArrowenable()
{
	CCadArrow::m_RenderEnable = m_Check_ArrowEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckDimensionenable()
{
	CCadDimension::m_RenderEnable = m_Check_DimensionEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckEllispeenable()
{
	CCadElipse::m_RenderEnable = m_Check_EllipseEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckFilledpolyenable()
{
	CCadPolygonFill::m_RenderEnable = m_Check_FillPolygonEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckLineenable()
{
	CCadLine::m_RenderEnable = m_Check_LineEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckOneflatenable()
{
	CCadHoleRnd1Flat::m_RenderEnable = m_Check_OneFlatHoleEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckOriginenable()
{
	CCadOrigin::m_RenderEnable = m_Check_OriginEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckPolygonenable()
{
	CCadPolygon::m_RenderEnable = m_Check_PolygonEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckRectenable()
{
	CCadRect::m_RenderEnable = m_Check_RectEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckRectholeenable()
{
	CCadRectHole::m_RenderEnable = m_Check_RectHoleEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckRoundedrectenable()
{
	CCadRoundRect::m_RenderEnable = m_CheckRoundedRectEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckRoundholeenable()
{
	CCadHoleRound::m_RenderEnable = m_Check_RoundHole.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckTextenabled()
{
	CCadText::m_RenderEnable = m_Check_TextEnable.GetCheck();
}


void CDialogRenderEnable::OnClickedCheckTwoflatenable()
{
	CCadHoleRnd2Flat::m_RenderEnable = m_Check_TwoFlatRoundHoleEnable.GetCheck();
}


BOOL CDialogRenderEnable::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Check_ArcCenteredEnable.SetCheck(CCadArcCentered::m_RenderEnable);
	m_Check_ArcEnable.SetCheck(CCadArc::m_RenderEnable);
	m_CheckRoundedRectEnable.SetCheck(CCadRoundRect::m_RenderEnable);
	m_Check_ArrowEnable.SetCheck(CCadArrow::m_RenderEnable);
	m_Check_DimensionEnable.SetCheck(CCadDimension::m_RenderEnable);
	m_Check_EllipseEnable.SetCheck(CCadElipse::m_RenderEnable);
	m_Check_FillPolygonEnable.SetCheck(CCadPolygonFill::m_RenderEnable);
	m_Check_LineEnable.SetCheck(CCadLine::m_RenderEnable);
	m_Check_OneFlatHoleEnable.SetCheck(CCadHoleRnd1Flat::m_RenderEnable);
	m_Check_OriginEnable.SetCheck(CCadOrigin::m_RenderEnable);
	m_Check_PolygonEnable.SetCheck(CCadPolygon::m_RenderEnable);
	m_Check_RectEnable.SetCheck(CCadRect::m_RenderEnable);
	m_Check_RectHoleEnable.SetCheck(CCadRectHole::m_RenderEnable);
	m_Check_RoundHole.SetCheck(CCadHoleRound::m_RenderEnable);
	m_Check_TextEnable.SetCheck(CCadText::m_RenderEnable);
	m_Check_TwoFlatRoundHoleEnable.SetCheck(CCadHoleRnd2Flat::m_RenderEnable);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
