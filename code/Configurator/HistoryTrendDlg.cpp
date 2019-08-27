// HistoryTrendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "HistoryTrendDlg.h"
#include "afxdialogex.h"

#include "ConfiguratorDoc.h"
#include "HistoryFileLoadCtrl.h"
#include "SelectTNDlg.h"

// CHistoryTrendDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryTrendDlg, CDialogEx)

CHistoryTrendDlg::CHistoryTrendDlg( CConfiguratorDoc* pDoc, CViewHistoryCtrl *pViewCtrl, CWnd* pParent )
	: m_pDoc( pDoc )
	, m_pViewCtrl( pViewCtrl )
	, m_pLoadCtrl( new CHistoryFileLoadCtrl( pDoc ))
	, CDialogEx(CHistoryTrendDlg::IDD, pParent)
	, m_nRefH(0)
	, m_nRefM(0)
	, m_nRefS(0)
	, m_bBrowseSetHisFile(true)
	, m_strRecordingFileName( L"" )
	, m_strPreHisFileName( L"" )
	, m_lHint( CConfiguratorDoc::HIS_DYNUPDATE )
{
	ASSERT( pDoc && pViewCtrl && m_pLoadCtrl );
}

CHistoryTrendDlg::~CHistoryTrendDlg()
{
	SAFE_DELETE( m_pLoadCtrl );
}

void CHistoryTrendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEB_FILE, m_mebFile);

	DDX_Control(pDX, IDC_CHK_MM, m_chkMM);
	DDX_Control(pDX, IDC_CHK_DD, m_chkDD);
	DDX_Control(pDX, IDC_CHK_YY, m_chkYY);
	DDX_Control(pDX, IDC_CHK_HH, m_chkHH);
	DDX_Control(pDX, IDC_CHK_MIN, m_chkMIN);
	DDX_Control(pDX, IDC_CHK_SS, m_chkSS);

	DDX_Control(pDX, IDC_SPIN_RATEH, m_spnRateH);
	DDX_Control(pDX, IDC_SPIN_RATEM, m_spnRateM);
	DDX_Control(pDX, IDC_SPIN_RATES, m_spnRateS);

	DDX_Text(pDX, IDC_EDT_RATEH, m_nRefH);
	DDX_Text(pDX, IDC_EDT_RATEM, m_nRefM);
	DDX_Text(pDX, IDC_EDT_RATES, m_nRefS);

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

	DDX_Control(pDX, IDC_BTN_LOAD_HISFILE, m_btnLoad);
}


BEGIN_MESSAGE_MAP(CHistoryTrendDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHistoryTrendDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_MEB_FILE, &CHistoryTrendDlg::OnEnChangeHisFile)
	ON_BN_CLICKED(IDC_BTN_LOAD_HISFILE, &CHistoryTrendDlg::OnBnClicked_LoadHisfile)
	ON_BN_CLICKED(IDC_TREND_RES, &CHistoryTrendDlg::OnBnClicked_Reset)
	ON_BN_CLICKED(IDC_BTN_TN1, &CHistoryTrendDlg::OnBnClickedBtnTn1)
	ON_BN_CLICKED(IDC_BTN_TN2, &CHistoryTrendDlg::OnBnClickedBtnTn2)
	ON_BN_CLICKED(IDC_BTN_TN3, &CHistoryTrendDlg::OnBnClickedBtnTn3)
	ON_BN_CLICKED(IDC_BTN_TN4, &CHistoryTrendDlg::OnBnClickedBtnTn4)
	ON_BN_CLICKED(IDC_BTN_TN5, &CHistoryTrendDlg::OnBnClickedBtnTn5)
	ON_BN_CLICKED(IDC_BTN_TN6, &CHistoryTrendDlg::OnBnClickedBtnTn6)
	ON_BN_CLICKED(IDC_BTN_TN7, &CHistoryTrendDlg::OnBnClickedBtnTn7)
	ON_BN_CLICKED(IDC_BTN_TN8, &CHistoryTrendDlg::OnBnClickedBtnTn8)
END_MESSAGE_MAP()


// CHistoryTrendDlg 消息处理程序

// =======================================================================
// Function name  : Initialize
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.11 09:00:00
// =======================================================================
BOOL CHistoryTrendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/// RecordingFilePathName
	m_pLoadCtrl->m_strViewFileName = m_pViewCtrl->GetFilePathName();
	
	if( CViewHistoryCtrl::VIEW_CURRENT == m_pViewCtrl->GetViewType())
	{
		m_strRecordingFileName = m_pLoadCtrl->m_strViewFileName;
	}
	else
	{
		m_strRecordingFileName = m_pViewCtrl->GetRecordingFilePathName();
	}

	/// History Data File
	m_mebFile.SetReadOnly();           // Read only

	// Ext and Filter
	CString strProductName( theApp.GetProductName());
	CString strFilter;
	strFilter.Format( L"Tri-Sen %s History Files (*-%s-*.xml)|*-%s-*.xml||", strProductName, strProductName, strProductName );
	m_mebFile.EnableFileBrowseButton( L"xml",strFilter );

	/// Load Btn - Disbale
	m_btnLoad.EnableWindow( FALSE );

	/// Refresh Rate
	m_spnRateH.SetBuddy( GetDlgItem(IDC_EDT_RATEH));
	m_spnRateM.SetBuddy( GetDlgItem(IDC_EDT_RATEM));
	m_spnRateS.SetBuddy( GetDlgItem(IDC_EDT_RATES));

	m_spnRateH.SetRange( 0, 23 );
	m_spnRateM.SetRange( 0, 59 );
	m_spnRateS.SetRange( 0, 59 );

	/// Pen properties: TagName, Color and Width
	ResetPens();
	DisablePens();
	
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
// Author         : WangDong 2013.04.23 16:30:00
// =======================================================================
void CHistoryTrendDlg::DisablePens()
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
// Description    : Tagname: null;   Color: global::TraceColors(fixed);   Width:2
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.12 13:00:00
// =======================================================================
void CHistoryTrendDlg::ResetPens()
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
// Author         : WangDong 2013.04.12 13:00:00
// ----------------------------------------------------------------------
void CHistoryTrendDlg::EnablePens( BOOL bEnable )
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
void CHistoryTrendDlg::LoadFromViewCtrl()
{
	ASSERT( m_pViewCtrl );

	/// History Data File - File Path Name
	CString strHisFile;
	m_mebFile.GetWindowTextW( strHisFile );
	CString strCtrlFileName = m_pViewCtrl->GetFilePathName();
	if( strHisFile.Compare( strCtrlFileName ) != 0 )
	{
		m_bBrowseSetHisFile = false;
		m_mebFile.SetWindowText( strCtrlFileName ); 
		EnablePens( TRUE );
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

	// Refresh Rate
	UINT value = m_pViewCtrl->GetUpdateRate();
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
void CHistoryTrendDlg::LoadFromViewDatas()
{
	ASSERT( m_pDoc != NULL );

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
// Function name  : OnEnChangeHisFile
// Description    : The edit text was changed
//  Non Browse:    Reset m_bBrowseSetHisFile flag and return
//  Browse Change:
//    File Valid:     Enable Load Button
//    File Non Valid: Disable Load Button and Tell user
//    Non NULL:  Set CHistoryFileDatas according to the history file user selected
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.12 14:30:00
// =======================================================================
void CHistoryTrendDlg::OnEnChangeHisFile()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if( !m_bBrowseSetHisFile )
	{
		m_mebFile.GetWindowTextW( m_strPreHisFileName );
		m_bBrowseSetHisFile = true;
		return;
	}

	// Browse set
	CString strFilePathName;
	m_mebFile.GetWindowTextW( strFilePathName );
	UINT type = ValidateHisFile( strFilePathName );
	if( E_NONE == type )     // Check the history file user selected
	{
		if( !IsHisFileSameArray( strFilePathName, m_strPreHisFileName ))
		{
			m_btnLoad.EnableWindow();
		}

		m_mebFile.GetWindowTextW( m_strPreHisFileName );
	}
	else
	{
		// Tell user The history file is invalid!
		CString str, strError;
		str.LoadStringW( IDS_HISFILE_INVALID );
		strError.Format( str, strFilePathName );
		AfxMessageBox( strError );

		// Log
		strError.Format( L"History Trend Dlg:History file %s is invalid.Error reason:%s.", strFilePathName, GetEHisFileString(type));
		theApp.LogInfo( strError );
		
		// Rename with its pre history file name
		m_bBrowseSetHisFile = false;
		m_mebFile.SetWindowTextW( m_strPreHisFileName );
	}
}

// =======================================================================
// Function name  : OnBnClicked_LoadHisfile
// Description    : 用户即使点击了Load，也可能选择Cancel，所以此操作只能
//  更新界面显示，不能改变原有的文档数据及控制类。只有当用户点击ok时，
//  才能更新相关文档数据和控制类
//  Disable Load Button
//  Only update dlg display: Reset Pens
//  
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.12 17:15:00
// =======================================================================
void CHistoryTrendDlg::OnBnClicked_LoadHisfile()
{
	ASSERT( m_pLoadCtrl != NULL );
	UpdateData( TRUE );                // Ctrl -> Data

	m_btnLoad.EnableWindow( FALSE );   // Avoid re-click
	ResetPens();                       // Reset Pens
	DisablePens();                     // Disable Pens
	UpdateData( FALSE );               // Data -> Ctrl
	
	CString strFilePathName;
	m_mebFile.GetWindowTextW( strFilePathName );
	ASSERT( E_NONE == ValidateHisFile(strFilePathName));

	if( m_pLoadCtrl->Load( strFilePathName ))
	{
		EnablePens( TRUE );
	}
}

// =======================================================================
// Function name  : OnBnClickedBtnTn1/8
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.18 15:00:00
// =======================================================================
void CHistoryTrendDlg::OnBnClickedBtnTn1()
{
	SelectTagname( 0 );
}

void CHistoryTrendDlg::OnBnClickedBtnTn2()
{
	SelectTagname( 1 );
}

void CHistoryTrendDlg::OnBnClickedBtnTn3()
{
	SelectTagname( 2 );
}

void CHistoryTrendDlg::OnBnClickedBtnTn4()
{
	SelectTagname( 3 );
}

void CHistoryTrendDlg::OnBnClickedBtnTn5()
{
	SelectTagname( 4 );
}

void CHistoryTrendDlg::OnBnClickedBtnTn6()
{
	SelectTagname( 5 );
}

void CHistoryTrendDlg::OnBnClickedBtnTn7()
{
	SelectTagname( 6 );
}

void CHistoryTrendDlg::OnBnClickedBtnTn8()
{
	SelectTagname( 7 );
}

// -----------------------------------------------------------------------
// Function name  : SelectTagname
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.15 16:40:00
// -----------------------------------------------------------------------
void CHistoryTrendDlg::SelectTagname( int nPenIndex )
{
	ASSERT((nPenIndex>=0) && (nPenIndex<=7));
	if((nPenIndex<0) || (nPenIndex>7))
	{
		return;
	}

	CString strTagname;
	m_edtTNV[nPenIndex].GetWindowTextW( strTagname );
	CSelectTNDlg dlg( m_pLoadCtrl->m_bDlgLoad ? m_pLoadCtrl->m_aHisFileDatas : m_pDoc->m_aHisFileDatas, strTagname );
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
// Author         : WangDong 2013.04.19 14:20:00
// =======================================================================
void CHistoryTrendDlg::SetViewHistoryCtrl( CString strHisFileName )
{
	// View Type
	int nViewType = CViewHistoryCtrl::VIEW_NONE;
	if( E_NONE == ValidateHisFile( strHisFileName ))
	{
		nViewType = CViewHistoryCtrl::VIEW_FILE;
		if( m_pDoc->IsRecording() && (E_NONE == ValidateHisFile( m_strRecordingFileName )))
		{
			if( IsHisFileSameArray( strHisFileName, m_strRecordingFileName ))
			{
				if( GetHisFileArrayCode( strHisFileName ) >= GetHisFileArrayCode( m_strRecordingFileName ))
				{
					nViewType = CViewHistoryCtrl::VIEW_CURRENT;
					theApp.LogInfo( L"View Type: view current!" );
				}
			}
		}
	}

	m_pViewCtrl->SetViewType( nViewType);

	// File Path Name
	m_pViewCtrl->SetFilePathName( strHisFileName );   

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

	// Refresh Rate
	m_pViewCtrl->SetUpdateRate( m_nRefH*global::HOUR_SECS + m_nRefM*global::MIN_SECS + m_nRefS );
}

// =======================================================================
// Function name  : SetHisViewDatas
// Description    :
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.15 16:40:00
// =======================================================================
void CHistoryTrendDlg::SetHisViewDatas()
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
// Author         : WangDong 2013.04.15 16:40:00
// =======================================================================
void CHistoryTrendDlg::OnBnClicked_Reset()
{
	// Time Format: default HH:MIN:SS
	m_chkMM.SetCheck( BST_UNCHECKED );
	m_chkDD.SetCheck( BST_UNCHECKED );
	m_chkYY.SetCheck( BST_UNCHECKED );

	m_chkHH.SetCheck( BST_CHECKED );
	m_chkMIN.SetCheck( BST_CHECKED );
	m_chkSS.SetCheck( BST_CHECKED );

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
// 注意：用户可能选择了一个有效的历史文件，需要加载但没有加载: 
//       只有此时Load按钮处于使能状态
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.11 10:00:00
// =======================================================================
void CHistoryTrendDlg::OnBnClickedOk()
{
	if( m_btnLoad.IsWindowEnabled())
	{
		CString strWarning;
		strWarning.LoadStringW( IDS_HIS_NOTLOAD );
		if( IDNO == AfxMessageBox( strWarning, MB_YESNO ))
		{
			return;
		}
	}
	else
	{
		// Ctrl -> Control datas
		UpdateData();

		CString strHisFileName;
		m_mebFile.GetWindowTextW( strHisFileName );
		if( E_NONE == ValidateHisFile( strHisFileName ))     // Check the history file
		{
			// Update doc::hisfiledatas
			if( m_pLoadCtrl->m_bDlgLoad )
			{
				m_pDoc->m_aHisFileDatas->operator=( *m_pLoadCtrl->m_aHisFileDatas );
				m_lHint = CConfiguratorDoc::HIS_INIT;   // Update History View Type：Init
			}
			else
			{
				m_lHint = CConfiguratorDoc::HIS_DYNUPDATE;   // Update History View Type：Init
			}
		}
		else
		{
			// Update doc::hisfiledatas
			m_pDoc->m_aHisFileDatas->Clear();      // Clear History File Datas
			strHisFileName.Empty();
			m_lHint = CConfiguratorDoc::HIS_CLEAR; // Update History View Type：Clear
		}

		SetViewHistoryCtrl( strHisFileName );        // Set History View Ctrl
		SetHisViewDatas();
	}

	CDialogEx::OnOK();
}
