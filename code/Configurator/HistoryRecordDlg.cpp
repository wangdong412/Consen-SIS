// HistoryRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "HistoryRecordDlg.h"
#include "afxdialogex.h"

#include "ConfiguratorDoc.h"
#include "RecordHistoryCtrl.h"

// CHistoryRecordDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryRecordDlg, CDialogEx)

CHistoryRecordDlg::CHistoryRecordDlg( CConfiguratorDoc* pDoc, CRecordHistoryCtrl* pRecordCtrl, CWnd* pParent /*=NULL*/)
	: m_pRecordCtrl( pRecordCtrl )
	, CDialogEx(CHistoryRecordDlg::IDD, pParent)
	, m_nFileSize(0)
{
	ASSERT( m_pRecordCtrl != NULL );
}

CHistoryRecordDlg::~CHistoryRecordDlg()
{
}

void CHistoryRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_RATEH, m_spnRateH);
	DDX_Control(pDX, IDC_SPIN_RATEM, m_spnRateM);
	DDX_Control(pDX, IDC_SPIN_RATES, m_spnRateS);

	DDX_Text(pDX, IDC_EDT_RATEH, m_nRateH);
	DDX_Text(pDX, IDC_EDT_RATEM, m_nRateM);
	DDX_Text(pDX, IDC_EDT_RATES, m_nRateS);

	DDV_MinMaxInt(pDX, m_nRateH, 0, 23);
	DDV_MinMaxInt(pDX, m_nRateM, 0, 59);
	DDV_MinMaxInt(pDX, m_nRateS, 0, 59);

	DDX_Text(pDX, IDC_EDT_MFS, m_nFileSize);
	DDX_Control(pDX, IDC_EDT_RECFILE, m_edtRecordFile);
}


BEGIN_MESSAGE_MAP(CHistoryRecordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHistoryRecordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RECORD_RES, &CHistoryRecordDlg::OnBnClicked_Reset)
	ON_BN_CLICKED(IDC_BTN_RECSAVE, &CHistoryRecordDlg::OnBnClicked_Browse)
END_MESSAGE_MAP()


// CHistoryRecordDlg 消息处理程序


// =======================================================================
// Function name  : Initialize
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.05.20 14:50:00
// =======================================================================
BOOL CHistoryRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Record File Path Name
	m_edtRecordFile.SetReadOnly();

	// Sampling Rate
	m_spnRateH.SetBuddy( GetDlgItem(IDC_EDT_RATEH));
	m_spnRateM.SetBuddy( GetDlgItem(IDC_EDT_RATEM));
	m_spnRateS.SetBuddy( GetDlgItem(IDC_EDT_RATES));

	m_spnRateH.SetRange( 0, 23 );
	m_spnRateM.SetRange( 0, 59 );
	m_spnRateS.SetRange( 0, 59 );

	ASSERT( m_pRecordCtrl != NULL );
	int value = m_pRecordCtrl->GetSamplingRate();
	m_nRateH = value / 3600;
	m_nRateM = (value-m_nRateH*3600) / 60;
	m_nRateS = value - m_nRateH*3600 - m_nRateM*60;

	// Maximum File Size
	m_nFileSize = m_pRecordCtrl->GetMaxSize()/(1024*1024);   // MB

	// Hide Maximum File Size Settings
	GetDlgItem(IDC_EDT_MFS)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_HISREC_MAXSIZE)->ShowWindow( SW_HIDE );

	// Data -> Ctrl
	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// =======================================================================
// Function name  : Browse
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.05.20 15:50:00
// =======================================================================
void CHistoryRecordDlg::OnBnClicked_Browse()
{
	std::auto_ptr<CFileDialog> dlg( new CFileDialog( false ));
	if( dlg.get() == NULL )
	{
		return;
	}

	// Ext and Filter
	CString strProductName( theApp.GetProductName());
	CString strFilter;
	strFilter.Format( L"Tri-Sen %s History Files (*-%s-*.xml)|*-%s-*.xml||", strProductName, strProductName, strProductName );
	dlg->m_ofn.lpstrFilter = strFilter;
	dlg->m_ofn.lpstrDefExt = L"xml";  

	if( dlg->DoModal() == IDOK )
	{
		CString strFilePath = dlg->GetPathName();
		strFilePath = strFilePath.Left( strFilePath.GetLength() -4 );

		SYSTEMTIME tm;
		GetLocalTime( &tm );
		CTime time( tm );
		strFilePath.AppendFormat( L"-%s-%s-1.xml",time.Format(L"%m-%d-%Y %H-%M-%S"), strProductName );

		m_edtRecordFile.SetWindowTextW( strFilePath );
	}
}

// =======================================================================
// Function name  : Reset
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.05.20 14:50:00
// =======================================================================
void CHistoryRecordDlg::OnBnClicked_Reset()
{
	// Record File Pathname
	m_edtRecordFile.SetWindowTextW( L"" );

	// Sampling Rate
	m_nRateH = 0;
	m_nRateM = 0;
	m_nRateS = 1;

	// Maximum File Size
	m_nFileSize = 500;

	// Data -> Ctrl
	UpdateData( FALSE );
}

// =======================================================================
// Function name  : Start
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.05.20 14:50:00
// =======================================================================
void CHistoryRecordDlg::OnBnClickedOk()
{
	// Ctrl -> Control datas
	UpdateData();

	// Check
	// File PathName
	CString strFilePathName;
	m_edtRecordFile.GetWindowTextW( strFilePathName );
	if( strFilePathName.IsEmpty())
	{
		CString str;
		str.LoadStringW( IDS_RECORDFILE_EMPTY );
		AfxMessageBox( str );

		return;
	}

	// Sample Rate
	int nSample = m_nRateH*3600+m_nRateM*60+m_nRateS;
	if( nSample <= 0 )
	{
		CString str;
		str.LoadStringW( IDS_RECORDRATE_ZERO );
		AfxMessageBox( str );

		return;
	}

	m_pRecordCtrl->SetFilePathName( strFilePathName );
	m_pRecordCtrl->SetSamplingRate( nSample );
	m_pRecordCtrl->SetMaxSize( m_nFileSize*1024*1024 );

	CDialogEx::OnOK();
}
