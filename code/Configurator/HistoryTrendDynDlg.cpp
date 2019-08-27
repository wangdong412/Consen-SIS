// HistoryTrendDynDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "HistoryTrendDynDlg.h"
#include "afxdialogex.h"

#include "ConfiguratorDoc.h"
#include "SelectTNDlg.h"


// CHistoryTrendDynDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryTrendDynDlg, CDialogEx)

CHistoryTrendDynDlg::CHistoryTrendDynDlg( CConfiguratorDoc* pDoc, CViewHistoryCtrl *pViewCtrl, double dStartTime, CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoryTrendDynDlg::IDD, pParent)
	, m_pDoc( pDoc )
	, m_pViewCtrl( pViewCtrl )
	, m_dtStart( dStartTime )
	, m_nRefH(0)
	, m_nRefM(0)
	, m_nRefS(0)
	, m_nRangD(0)
	, m_nRangH(0)
	, m_nRangM(0)
	, m_nRangS(0)
{
	ASSERT( pDoc && pViewCtrl );
}

CHistoryTrendDynDlg::~CHistoryTrendDynDlg()
{
}

void CHistoryTrendDynDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHK_MM, m_chkMM);
	DDX_Control(pDX, IDC_CHK_DD, m_chkDD);
	DDX_Control(pDX, IDC_CHK_YY, m_chkYY);
	DDX_Control(pDX, IDC_CHK_HH, m_chkHH);
	DDX_Control(pDX, IDC_CHK_MIN, m_chkMIN);
	DDX_Control(pDX, IDC_CHK_SS, m_chkSS);

	DDX_Control(pDX, IDC_SPIN_RANGD, m_spnRangD);
	DDX_Control(pDX, IDC_SPIN_RANGH, m_spnRangH);
	DDX_Control(pDX, IDC_SPIN_RANGM, m_spnRangM);
	DDX_Control(pDX, IDC_SPIN_RANGS, m_spnRangS);
	DDX_Control(pDX, IDC_SPIN_RATEH, m_spnRateH);
	DDX_Control(pDX, IDC_SPIN_RATEM, m_spnRateM);
	DDX_Control(pDX, IDC_SPIN_RATES, m_spnRateS);

	DDX_Text(pDX, IDC_EDT_STARTM, m_nStartM);
	DDX_Text(pDX, IDC_EDT_STARTD, m_nStartD);
	DDX_Text(pDX, IDC_EDT_STARTY, m_nStartY);
	DDX_Text(pDX, IDC_EDT_STARTH, m_nStartH);
	DDX_Text(pDX, IDC_EDT_STARTMIN, m_nStartMIN);
	DDX_Text(pDX, IDC_EDT_STARTS, m_nStartS);

	DDX_Text(pDX, IDC_EDT_RANGD, m_nRangD);
	DDX_Text(pDX, IDC_EDT_RANGH, m_nRangH);
	DDX_Text(pDX, IDC_EDT_RANGM, m_nRangM);
	DDX_Text(pDX, IDC_EDT_RANGS, m_nRangS);

	DDX_Text(pDX, IDC_EDT_RATEH, m_nRefH);
	DDX_Text(pDX, IDC_EDT_RATEM, m_nRefM);
	DDX_Text(pDX, IDC_EDT_RATES, m_nRefS);

	DDV_MinMaxInt(pDX, m_nRangD, 0, 364);
	DDV_MinMaxInt(pDX, m_nRangH, 0, 23);
	DDV_MinMaxInt(pDX, m_nRangM, 0, 59);
	DDV_MinMaxInt(pDX, m_nRangS, 0, 59);

	DDV_MinMaxInt(pDX, m_nRefH, 0, 23);
	DDV_MinMaxInt(pDX, m_nRefM, 0, 59);
	DDV_MinMaxInt(pDX, m_nRefS, 0, 59);

	DDX_Control(pDX, IDC_EDT_TN1, m_edtTNV[0]);
	DDX_Control(pDX, IDC_EDT_TN2, m_edtTNV[1]);
	DDX_Control(pDX, IDC_EDT_TN3, m_edtTNV[2]);
	DDX_Control(pDX, IDC_EDT_TN4, m_edtTNV[3]);
	DDX_Control(pDX, IDC_EDT_TN5, m_edtTNV[4]);
	DDX_Control(pDX, IDC_EDT_TN6, m_edtTNV[5]);
	DDX_Control(pDX, IDC_EDT_TN7, m_edtTNV[6]);
	DDX_Control(pDX, IDC_EDT_TN8, m_edtTNV[7]);

	DDX_Control(pDX, IDC_BTN_TN1, m_btnTNV[0]);
	DDX_Control(pDX, IDC_BTN_TN2, m_btnTNV[1]);
	DDX_Control(pDX, IDC_BTN_TN3, m_btnTNV[2]);
	DDX_Control(pDX, IDC_BTN_TN4, m_btnTNV[3]);
	DDX_Control(pDX, IDC_BTN_TN5, m_btnTNV[4]);
	DDX_Control(pDX, IDC_BTN_TN6, m_btnTNV[5]);
	DDX_Control(pDX, IDC_BTN_TN7, m_btnTNV[6]);
	DDX_Control(pDX, IDC_BTN_TN8, m_btnTNV[7]);

	DDX_Control(pDX, IDC_MCB_1, m_mcbV[0]);
	DDX_Control(pDX, IDC_MCB_2, m_mcbV[1]);
	DDX_Control(pDX, IDC_MCB_3, m_mcbV[2]);
	DDX_Control(pDX, IDC_MCB_4, m_mcbV[3]);
	DDX_Control(pDX, IDC_MCB_5, m_mcbV[4]);
	DDX_Control(pDX, IDC_MCB_6, m_mcbV[5]);
	DDX_Control(pDX, IDC_MCB_7, m_mcbV[6]);
	DDX_Control(pDX, IDC_MCB_8, m_mcbV[7]);

	DDX_Control(pDX, IDC_EDT_W1, m_edtWV[0]);
	DDX_Control(pDX, IDC_EDT_W2, m_edtWV[1]);
	DDX_Control(pDX, IDC_EDT_W3, m_edtWV[2]);
	DDX_Control(pDX, IDC_EDT_W4, m_edtWV[3]);
	DDX_Control(pDX, IDC_EDT_W5, m_edtWV[4]);
	DDX_Control(pDX, IDC_EDT_W6, m_edtWV[5]);
	DDX_Control(pDX, IDC_EDT_W7, m_edtWV[6]);
	DDX_Control(pDX, IDC_EDT_W8, m_edtWV[7]);

	DDX_Text(pDX, IDC_EDT_W1, m_nWidth[0]);
	DDX_Text(pDX, IDC_EDT_W2, m_nWidth[1]);
	DDX_Text(pDX, IDC_EDT_W3, m_nWidth[2]);
	DDX_Text(pDX, IDC_EDT_W4, m_nWidth[3]);
	DDX_Text(pDX, IDC_EDT_W5, m_nWidth[4]);
	DDX_Text(pDX, IDC_EDT_W6, m_nWidth[5]);
	DDX_Text(pDX, IDC_EDT_W7, m_nWidth[6]);
	DDX_Text(pDX, IDC_EDT_W8, m_nWidth[7]);
}


BEGIN_MESSAGE_MAP(CHistoryTrendDynDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHistoryTrendDynDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_TREND_RES, &CHistoryTrendDynDlg::OnBnClicked_Reset)
	ON_BN_CLICKED(IDC_BTN_TN1, &CHistoryTrendDynDlg::OnBnClickedBtnTn1)
	ON_BN_CLICKED(IDC_BTN_TN2, &CHistoryTrendDynDlg::OnBnClickedBtnTn2)
	ON_BN_CLICKED(IDC_BTN_TN3, &CHistoryTrendDynDlg::OnBnClickedBtnTn3)
	ON_BN_CLICKED(IDC_BTN_TN4, &CHistoryTrendDynDlg::OnBnClickedBtnTn4)
	ON_BN_CLICKED(IDC_BTN_TN5, &CHistoryTrendDynDlg::OnBnClickedBtnTn5)
	ON_BN_CLICKED(IDC_BTN_TN6, &CHistoryTrendDynDlg::OnBnClickedBtnTn6)
	ON_BN_CLICKED(IDC_BTN_TN7, &CHistoryTrendDynDlg::OnBnClickedBtnTn7)
	ON_BN_CLICKED(IDC_BTN_TN8, &CHistoryTrendDynDlg::OnBnClickedBtnTn8)
END_MESSAGE_MAP()


// CHistoryTrendDynDlg 消息处理程序

// =======================================================================
// Function name  : Initialize
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.24 14:30:00
// =======================================================================
BOOL CHistoryTrendDynDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/// Start Time
	m_nStartM = m_dtStart.GetMonth();
	m_nStartD = m_dtStart.GetDay();
	m_nStartY = m_dtStart.GetYear();

	m_nStartH = m_dtStart.GetHour();
	m_nStartMIN = m_dtStart.GetMinute();
	m_nStartS = m_dtStart.GetSecond();

	/// Axis Time Range
	m_spnRangD.SetBuddy( GetDlgItem(IDC_EDT_RANGD));
	m_spnRangH.SetBuddy( GetDlgItem(IDC_EDT_RANGH));
	m_spnRangM.SetBuddy( GetDlgItem(IDC_EDT_RANGM));
	m_spnRangS.SetBuddy( GetDlgItem(IDC_EDT_RANGS));

	m_spnRangD.SetRange( 0, 364 );
	m_spnRangH.SetRange( 0, 23 );
	m_spnRangM.SetRange( 0, 59 );
	m_spnRangS.SetRange( 0, 59 );

	/// Refresh Rate
	m_spnRateH.SetBuddy( GetDlgItem(IDC_EDT_RATEH));
	m_spnRateM.SetBuddy( GetDlgItem(IDC_EDT_RATEM));
	m_spnRateS.SetBuddy( GetDlgItem(IDC_EDT_RATES));

	m_spnRateH.SetRange( 0, 23 );
	m_spnRateM.SetRange( 0, 59 );
	m_spnRateS.SetRange( 0, 59 );

	/// Pen properties: TagName, Color and Width
	EnablePens( TRUE );
	
	/// Load
	LoadFromViewCtrl();
	LoadFromViewDatas();

	// Hide Color
	GetDlgItem( IDC_HISREC_COLOR1 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_HISREC_COLOR2 )->ShowWindow( SW_HIDE );
	for( UINT i=IDC_MCB_1; i <=IDC_MCB_8; i++ )
	{
		GetDlgItem( i )->ShowWindow( SW_HIDE );
	}

	/// Data -> Ctrl
	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// =======================================================================
// Function name  : DisablePens
// Description    : Disable Select Button and Width Edit
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.24 14:30:00
// =======================================================================
void CHistoryTrendDynDlg::DisablePens()
{
	for( size_t i=0; i < sizeof(m_nWidth)/sizeof(m_nWidth[0]); ++i )
	{
		m_btnTNV[i].EnableWindow( FALSE );  // Select Button
		m_mcbV[i].EnableWindow( FALSE );    // Color
		m_edtWV[i].SetReadOnly();           // Width Edit
	}
}

// =======================================================================
// Function name  : ResetPens
// Description    : Tagname: ""; Color: global::TraceColors(fixed); Width:2
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.24 14:30:00
// =======================================================================
void CHistoryTrendDynDlg::ResetPens()
{
	for( size_t i=0; i < sizeof(m_nWidth)/sizeof(m_nWidth[0]); ++i )
	{
		m_edtTNV[i].SetWindowTextW( L"" );
		m_mcbV[i].SetColor( global::TraceColors[i]);
		m_nWidth[i] = 2;
	}
}

// -----------------------------------------------------------------------
// Function name  : EnablePens
// Description    : Enable Select Button and Width Edit
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.24 14:40:00
// ----------------------------------------------------------------------
void CHistoryTrendDynDlg::EnablePens( BOOL bEnable )
{
	for( size_t i=0; i < sizeof(m_nWidth)/sizeof(m_nWidth[0]); ++i )
	{
		m_btnTNV[i].EnableWindow( TRUE );  // Select Button
		m_mcbV[i].EnableWindow( TRUE );    // Color
		m_edtWV[i].SetReadOnly( FALSE );   // Width Edit
	}
}

// =======================================================================
// Function name  : LoadFromViewCtrl
// Description    : Set Control with ViewCtrl configed before
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.11 09:00:00
// =======================================================================
void CHistoryTrendDynDlg::LoadFromViewCtrl()
{
	ASSERT((m_pViewCtrl != NULL) && (E_NONE == ValidateHisFile( m_pViewCtrl->GetFilePathName())));

	if((NULL == m_pViewCtrl) || (ValidateHisFile( m_pViewCtrl->GetFilePathName()) != E_NONE))
	{
		return;
	}
	
	// Time Format
	CString strFormat( m_pViewCtrl->GetTimeFormatW()); 
	int check = (strFormat.Find( L"MM" ) != -1) ? BST_CHECKED : BST_UNCHECKED;
	m_chkMM.SetCheck( check );

	check = (strFormat.Find( L"DD" ) != -1) ? BST_CHECKED : BST_UNCHECKED;
	m_chkDD.SetCheck( check );

	check = (strFormat.Find( L"YYYY" ) != -1) ? BST_CHECKED : BST_UNCHECKED;
	m_chkYY.SetCheck( check );

	check = (strFormat.Find( L"HH" ) != -1) ? BST_CHECKED : BST_UNCHECKED;
	m_chkHH.SetCheck( check );

	check = (strFormat.Find( L"NN" ) != -1) ? BST_CHECKED : BST_UNCHECKED;
	m_chkMIN.SetCheck( check );

	check = (strFormat.Find( L"SS" ) != -1) ? BST_CHECKED : BST_UNCHECKED;
	m_chkSS.SetCheck( check );

	// Axis Time Range
	UINT value = m_pViewCtrl->GetTimeRange();
	m_nRangD = value / (3600*24);
	m_nRangH = (value - m_nRangD*3600*24) / 3600;
	m_nRangM = (value - m_nRangD*3600*24 - m_nRangH*3600) / 60;
	m_nRangS = value - m_nRangD*3600*24 - m_nRangH*3600 - m_nRangM*60;

	// Refresh Rate
	value = m_pViewCtrl->GetUpdateRate();
	m_nRefH = value / 3600;
	m_nRefM = (value-m_nRefH*3600) / 60;
	m_nRefS = value - m_nRefH*3600 - m_nRefM*60;
}

// =======================================================================
// Function name  : LoadFromViewDatas
// Description    : Set Pen properties with before
//                  Tagname, Color(fixed) and Width
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.12 13:00:00
// =======================================================================
void CHistoryTrendDynDlg::LoadFromViewDatas()
{
	ASSERT( m_pDoc != NULL );
	if( NULL == m_pDoc )
	{
		return;
	}

	CHistoryViewDatas *pViewDatas = m_pDoc->m_aHisViewDatas;
	ASSERT( pViewDatas != NULL );
	if( pViewDatas != NULL )
	{
		for( size_t i=0; i < pViewDatas->Size(); ++i )
		{
			const CHistoryViewData* data = pViewDatas->GetHisViewData( i );
			ASSERT( data != NULL );
			if( data != NULL )
			{
				m_edtTNV[i].SetWindowText( data->GetTagName());  // Tagname
				m_mcbV[i].SetColor( data->GetColor());           // Color(fixed)
				m_nWidth[i] = data->GetWidth();                  // Width
			}
		}
	}
}

// =======================================================================
// Function name  : OnBnClickedBtnTn1/8
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.24 15:00:00
// =======================================================================
void CHistoryTrendDynDlg::OnBnClickedBtnTn1()
{
	SelectTagname( 0 );
}

void CHistoryTrendDynDlg::OnBnClickedBtnTn2()
{
	SelectTagname( 1 );
}

void CHistoryTrendDynDlg::OnBnClickedBtnTn3()
{
	SelectTagname( 2 );
}

void CHistoryTrendDynDlg::OnBnClickedBtnTn4()
{
	SelectTagname( 3 );
}

void CHistoryTrendDynDlg::OnBnClickedBtnTn5()
{
	SelectTagname( 4 );
}

void CHistoryTrendDynDlg::OnBnClickedBtnTn6()
{
	SelectTagname( 5 );
}

void CHistoryTrendDynDlg::OnBnClickedBtnTn7()
{
	SelectTagname( 6 );
}

void CHistoryTrendDynDlg::OnBnClickedBtnTn8()
{
	SelectTagname( 7 );
}

// -----------------------------------------------------------------------
// Function name  : SelectTagname
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.24 15:10:00
// -----------------------------------------------------------------------
void CHistoryTrendDynDlg::SelectTagname( int nPenIndex )
{
	ASSERT((nPenIndex>=0) && (nPenIndex<=7));
	if((nPenIndex<0) || (nPenIndex>7))
	{
		return;
	}

	CString strTagname;
	m_edtTNV[nPenIndex].GetWindowTextW( strTagname );
	CSelectTNDlg dlg( m_pDoc->m_aHisFileDatas, strTagname );
	if( IDOK == dlg.DoModal())
	{
		m_edtTNV[nPenIndex].SetWindowTextW( dlg.GetSelTagName());
	}
}

// =======================================================================
// Function name  : SetViewHistoryCtrl
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.24 15:10:00
// =======================================================================
void CHistoryTrendDynDlg::SetViewHistoryCtrl()
{
	// DateTime Format
	CString strDateFormat, strTimeFormat;
	if( BST_CHECKED == m_chkMM.GetCheck())
	{
		strDateFormat = L"MM";
	}

	if( BST_CHECKED == m_chkDD.GetCheck())
	{
		strDateFormat += strDateFormat.IsEmpty()? L"DD" : L"-DD";
	}

	if( BST_CHECKED == m_chkYY.GetCheck())
	{
		strDateFormat += strDateFormat.IsEmpty()? L"YYYY" : L"-YYYY";
	}

	if( BST_CHECKED == m_chkHH.GetCheck())
	{
		strTimeFormat = strDateFormat.IsEmpty() ? L"HH" : L" HH";
	}

	if( BST_CHECKED == m_chkMIN.GetCheck())
	{
		strTimeFormat += strTimeFormat.IsEmpty()? L" NN" : L":NN";
	}

	if( BST_CHECKED == m_chkSS.GetCheck())
	{
		strTimeFormat += strTimeFormat.IsEmpty()? L" SS" : L":SS";
	}

	strDateFormat += strTimeFormat;
	m_pViewCtrl->SetTimeFormat( strDateFormat );

	// Axis Time Range: s
	m_pViewCtrl->SetTimeRange( m_nRangD*global::DAY_SECS + m_nRangH*global::HOUR_SECS + m_nRangM*global::MIN_SECS + m_nRangS );

	// Refresh Rate
	m_pViewCtrl->SetUpdateRate( m_nRefH*global::HOUR_SECS + m_nRefM*global::MIN_SECS + m_nRefS );
}

// =======================================================================
// Function name  : SetHisViewDatas
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.24 15:20:00
// =======================================================================
void CHistoryTrendDynDlg::SetHisViewDatas()
{
	ASSERT( m_pDoc != NULL );
	CHistoryViewDatas *pViewDatas = m_pDoc->m_aHisViewDatas;
	ASSERT( pViewDatas != NULL );
	if( pViewDatas != NULL )
	{
		for( size_t i=0; i < pViewDatas->Size(); ++i )
		{
			CHistoryViewData* data = pViewDatas->GetHisViewData( i );
			ASSERT( data != NULL );
			if( data != NULL )
			{
				CString strTagName;
				m_edtTNV[i].GetWindowTextW( strTagName );
				if( strTagName.IsEmpty())
				{
					data->SetSeriesIndex( -1 );
					data->SetFileData( NULL );
				}
				else
				{
					data->SetSeriesIndex( i );
					data->SetFileData( m_pDoc->m_aHisFileDatas->find_name(strTagName));
				}

				data->SetColor( m_mcbV[i].GetColor());
				data->SetWidth( m_nWidth[i] );
			}
		}
	}
}

// =======================================================================
// Function name  : OnBnClicked_Reset
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.24 15:30:00
// =======================================================================
void CHistoryTrendDynDlg::OnBnClicked_Reset()
{
	// Start Time
	m_nStartM = m_dtStart.GetMonth();
	m_nStartD = m_dtStart.GetDay();
	m_nStartY = m_dtStart.GetYear();

	m_nStartH = m_dtStart.GetHour();
	m_nStartMIN = m_dtStart.GetMinute();
	m_nStartS = m_dtStart.GetSecond();

	// Time Format: default HH:MIN:SS
	m_chkMM.SetCheck( BST_UNCHECKED );
	m_chkDD.SetCheck( BST_UNCHECKED );
	m_chkYY.SetCheck( BST_UNCHECKED );

	m_chkHH.SetCheck( BST_CHECKED );
	m_chkMIN.SetCheck( BST_CHECKED );
	m_chkSS.SetCheck( BST_CHECKED );

	// Axis Time Range: default 30 mins
	m_nRangD = 0;
	m_nRangH = 0;
	m_nRangM = 30;
	m_nRangS = 0;

	// Refresh Rate: default 1 min
	m_nRefH = 0;
	m_nRefM = 1;
	m_nRefS = 0;

	// Reset Pens
	ResetPens();

	// Data -> Ctrl
	UpdateData( FALSE );
}

// =======================================================================
// Function name  : OnBnClickedOk
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.24 15:40:00
// =======================================================================
void CHistoryTrendDynDlg::OnBnClickedOk()
{
	// Ctrl -> Control datas
	UpdateData();

	m_dtStart.SetDateTime( m_nStartY, m_nStartM, m_nStartD, m_nStartH, m_nStartMIN, m_nStartS ); // Start Time
	SetViewHistoryCtrl();
	SetHisViewDatas();

	CDialogEx::OnOK();
}
