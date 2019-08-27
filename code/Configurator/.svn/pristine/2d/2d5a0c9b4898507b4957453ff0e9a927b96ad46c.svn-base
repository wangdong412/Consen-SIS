// CTRLDateTimeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "CTRLDateTimeDlg.h"
#include "afxdialogex.h"

#include "ConfiguratorDoc.h"
#include "MainFrm.h"
#include "ModbusThread.h"

extern std::auto_ptr<modbus::CModbusThread> ModbusThread;

UINT DateTimeThreadFunc( LPVOID pParam )
{
	CCTRLDateTimeDlg* pDlg = (CCTRLDateTimeDlg*)pParam;
	if( pDlg->GetSafeHwnd())
	{
		while( !pDlg->m_bDateTimeTer )
		{
			pDlg->UpdateTime();
		}

		pDlg->SetEvent();
	}

	return 0;
}

// CCTRLDateTimeDlg 对话框

IMPLEMENT_DYNAMIC(CCTRLDateTimeDlg, CDialogEx)

CCTRLDateTimeDlg::CCTRLDateTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCTRLDateTimeDlg::IDD, pParent)
	, m_pDateTimeThread( NULL )
	, m_pEvent( new CEvent())   // not signaled, auto reset
	, m_bDateTimeTer( false )
{
	m_pMainWnd = (CMainFrame*)theApp.GetMainWnd();
	ASSERT( m_pMainWnd );
	m_pDoc = (CConfiguratorDoc*)m_pMainWnd->GetActiveDocument();
	ASSERT( m_pDoc );
}

CCTRLDateTimeDlg::~CCTRLDateTimeDlg()
{
}

void CCTRLDateTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DTP_REALDATETIME, m_RealDateTime);
	DDX_Control(pDX, IDC_DTP_DATETIME, m_DateTime);
}


BEGIN_MESSAGE_MAP(CCTRLDateTimeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCTRLDateTimeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCTRLDateTimeDlg 消息处理程序

// =========================================================================
// Function name: OnInitDialog
// Description  : Set Timer
// Return type  : BOOL
// Parameters   : 
// Author       : WangDong 2012.01.12 15:30:00
// =========================================================================
BOOL CCTRLDateTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// Set format
	m_RealDateTime.SetFormat( L"yyyy-MM-dd  HH:mm:ss" );
	m_DateTime.SetFormat( L"yyyy-MM-dd  HH:mm:ss" );

	m_pDateTimeThread = AfxBeginThread( DateTimeThreadFunc, this );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// =========================================================================
// Function name: UpdateTime
// Description  : Display date and time in real-time
// Return type  : void
// Parameters   : 
// Author       : WangDong 2012.01.12 15:30:00
// =========================================================================
void CCTRLDateTimeDlg::UpdateTime()
{
	if( ModbusThread->HasEvent())
	{
		int nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMinute = 0, nSec = 0;
		
		CAliasItem *pAlias = m_pDoc->m_aAliases->find_name( L"iCTRLYear" );
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			nYear = pAlias->remI2();   // 1970-3000
			if( nYear < 1971 )
			{
				nYear = 1971;
			}
			else if( nYear > 3000 )
			{
				nYear = 3000;
			}
		}

		pAlias = m_pDoc->m_aAliases->find_name( L"iCTRLMonth" );
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			nMonth = pAlias->remI2();   // 1-12
			if( nMonth < 1 )
			{
				nMonth = 1;
			}
			else if( nMonth > 12 )
			{
				nMonth = 12;
			}
		}

		pAlias = m_pDoc->m_aAliases->find_name( L"iCTRLDay" );
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			nDay = pAlias->remI2();   // 1–31
			if( nDay < 1 )
			{
				nDay = 1;
			}
			else if( nDay > 31 )
			{
				nDay = 31;
			}
		}

		pAlias = m_pDoc->m_aAliases->find_name( L"iCTRLHour" );
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			nHour = pAlias->remI2();   // 0–23
			if( nHour < 0 )
			{
				nHour = 0;
			}
			else if( nHour > 23 )
			{
				nHour = 23;
			}
		}

		pAlias = m_pDoc->m_aAliases->find_name( L"iCTRLMinute" );
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			nMinute = pAlias->remI2(); 
			if( nMinute < 0 )
			{
				nMinute = 0;
			}
			else if( nMinute > 59 )
			{
				nMinute = 59;
			}
		}

		pAlias = m_pDoc->m_aAliases->find_name( L"iCTRLSec" );
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			nSec = pAlias->remI2();   
			if( nSec < 0 )
			{
				nSec = 0;
			}
			else if( nSec > 59 )
			{
				nSec = 59;
			}
		}

		SYSTEMTIME tm;
		m_RealDateTime.GetTime( &tm );
		if((tm.wSecond != nSec )   ||
		   (tm.wMinute != nMinute) ||
		   (tm.wHour != nHour )    ||
		   (tm.wDay != nDay )      ||
		   (tm.wMonth != nMonth)   ||
		   (tm.wYear != nYear))
		{
			CTime time( nYear, nMonth, nDay, nHour, nMinute, nSec );
			m_RealDateTime.SetTime( &time );
			UpdateData( FALSE );
		}
	}//if( ModbusThread->HasEvent())
}

// =========================================================================
// Function name: TerminateThread
// Description  : Terminate DateTime Thread, and wait it finish
// Return type  : Result of WaitForSingleObject
// Parameters   : dwMiliSecs
// Author       : WangDong 2012.05.25 15:00:00
// =========================================================================
DWORD CCTRLDateTimeDlg::TerminateTimeThread( DWORD dwMiliSecs  )
{
	m_bDateTimeTer = true;
	return ::WaitForSingleObject( m_pEvent->m_hObject, dwMiliSecs );
}

// =========================================================================
// Function name: OnBnClickedOk
// Description  : Get time, and set its parameters
// Return type  : void
// Parameters   : 
// Author       : WangDong 2012.01.12 15:30:00
// =========================================================================
void CCTRLDateTimeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	TerminateTimeThread();

	///Get Data and Time
	CTime time;
	m_DateTime.GetTime( time );

	CAliasItem *pAlias = m_pDoc->m_aAliases->find_name( L"iYear" );
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( time.GetYear());   
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iMonth" );
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( time.GetMonth());   // 1-12
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iDay" );
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( time.GetDay());     // 1–31
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iHour" );
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( time.GetHour());   // 0–23
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iMinute" );
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( time.GetMinute()); 
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iSec" );
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( time.GetSecond());   
	}

	CDialogEx::OnOK();
}

// =========================================================================
// Function name: OnCancel
// Description  : Terminate Time Thread
// Return type  : void
// Parameters   : 
// Author       : WangDong 2012.01.12 15:30:00
// =========================================================================
void CCTRLDateTimeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	TerminateTimeThread();

	CDialogEx::OnCancel();
}
