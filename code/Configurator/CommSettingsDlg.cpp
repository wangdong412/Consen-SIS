// CommSettingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "CommSettingsDlg.h"
#include "afxdialogex.h"

// Added by wangdong
#include "CommSettings.h"
#include "MainFrm.h"

// --------------------------------------------------------------------------
extern std::auto_ptr<CEvent> g_pTestCommEvent;

const int c_nMaxComPort = 16;
const BYTE c_byMaxSlaveID = 248;
const DWORD c_dwTestTimeout = 2000;
//======================================================================

// CCommSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CCommSettingsDlg, CDialogEx)

CCommSettingsDlg::CCommSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommSettingsDlg::IDD, pParent),
	  m_dwViewHelpID( HIDR_COMMSETTINGS ),
	  m_pSettings( NULL )
{
}

CCommSettingsDlg::~CCommSettingsDlg()
{
}

// =============================================================
// Function name: SetCtlEnableStatus
// Description  : Set dialog control's enable status 
//                according to the modbus type
// Return type  : void
// Parameters   : 
// Author       : WangDong 
// =============================================================
void CCommSettingsDlg::SetCtlEnableStatus( void )
{
	ASSERT( m_cboType.GetSafeHwnd());
	if( !m_cboType.GetSafeHwnd())
	{
		return;
	}

	// Modbus Message
	CString strCommMsg( theApp.GetCommMessage( m_cboType.GetCurSel()));
	m_stcCommMsg.SetWindowText( strCommMsg );

    BOOL bSerEnable = TRUE;
	if( m_cboType.GetCurSel() == 0 )  // Modbus Serial
	{
		bSerEnable = TRUE;
	}
	else
	{
        bSerEnable = FALSE;
	}

	//Serials
	GetDlgItem( IDC_SERIAL_PORT )->EnableWindow( bSerEnable );
	GetDlgItem( IDC_SERIAL_BAUD )->EnableWindow( bSerEnable );
	GetDlgItem( IDC_SERIAL_DATABITS )->EnableWindow( bSerEnable );
	GetDlgItem( IDC_SERIAL_PARITY )->EnableWindow( bSerEnable );
	GetDlgItem( IDC_SERIAL_STOPBITS )->EnableWindow( bSerEnable );
	GetDlgItem( IDC_SERIAL_RTS )->EnableWindow( bSerEnable && 
		                                       (CConfiguratorApp::DSC100 == theApp.GetProductType()));
	GetDlgItem( IDC_SERIAL_SLAVEID )->EnableWindow( bSerEnable );

	m_cboSerPort.EnableWindow( bSerEnable );
	m_cboBaud.EnableWindow( bSerEnable );
	m_cboDatabits.EnableWindow( bSerEnable );
	m_cboParity.EnableWindow( bSerEnable );
	m_cboStop.EnableWindow( bSerEnable );
    m_cboRTS.EnableWindow( bSerEnable && 
		                   (CConfiguratorApp::DSC100 == theApp.GetProductType()));
	m_chkHand.EnableWindow( bSerEnable );
	m_cboSerSlaveID.EnableWindow( bSerEnable );
    m_btnSerDef.EnableWindow( bSerEnable );

	//TCP
	GetDlgItem( IDC_TCP_IPADDRESS )->EnableWindow( !bSerEnable ); 
	GetDlgItem( IDC_TCP_PORT )->EnableWindow( !bSerEnable );
	GetDlgItem( IDC_TCP_SLAVEID )->EnableWindow( !bSerEnable );

	m_ipAddress.EnableWindow( !bSerEnable );
	m_edtTCPPort.EnableWindow( !bSerEnable );
	m_cboTCPSlaveID.EnableWindow( !bSerEnable );
    m_btnTCPDef.EnableWindow( !bSerEnable );
}

// =========================================================================
// Function name: SetSettings
// Description  : Set m_pSettings, and then dialog controls with m_pSettings,
//                last set control enable status
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.02 18:50:00
// =========================================================================
void CCommSettingsDlg::SetSettings( CCommSettings* pSettings )
{
	/// 1. Set m_pSettings
    ASSERT( CheckReadPtr(pSettings, sizeof(CCommSettings)));
	m_pSettings = pSettings;

	if( !CheckReadPtr(m_pSettings, sizeof(CCommSettings)))
	{
		return;
	}

	// If invalid, set default, not return 
	if( !m_pSettings->IsValid( m_pSettings->m_bModbusTcp ))
	{
		m_pSettings->SetDefault( m_pSettings->m_bModbusTcp );
	}

	/// 2. Dialog Controls
	// 2.1 Modbus Type
	if( m_pSettings->m_bModbusTcp )
	{
		m_cboType.SetCurSel( 1 );
	}
	else
	{
		m_cboType.SetCurSel( 0 );
	}

	int nIndex = -1;

	// 2.2 Serial Settings
	// 2.2.1 Serial Port
	nIndex = m_cboSerPort.FindStringExact( -1, m_pSettings->m_strPort );
	if( nIndex != CB_ERR )  // Search successful
	{
		if((nIndex >= 0) && (nIndex < m_cboSerPort.GetCount()))
		{
			m_cboSerPort.SetCurSel( nIndex );
		}
		else //if( m_cboSerPort.GetCount() >= 0 )
		{
			m_cboSerPort.SetCurSel( 0 );
		}
	}//if( nIndex != CB_ERR )

	CString str;

	// 2.2.2 Baud
	nIndex = -1;
	str.Format( L"%d", m_pSettings->m_dwBaud );
	nIndex = m_cboBaud.FindStringExact( -1, str );
	if((nIndex >= 0) && (nIndex < m_cboBaud.GetCount()))
	{
		m_cboBaud.SetCurSel( nIndex );
	}
	else
	{
		m_cboBaud.SetCurSel( m_cboBaud.FindStringExact(-1, L"38400"));
	}

	// 2.2.3 Data Bits
	nIndex = -1;
	str.Empty();
	str.Format( L"%d", m_pSettings->m_byDataBits );
	nIndex = m_cboDatabits.FindStringExact( -1, str );
	if((nIndex >= 0) && (nIndex < m_cboDatabits.GetCount()))
	{
		m_cboDatabits.SetCurSel( nIndex );
	}
	else
	{
		m_cboDatabits.SetCurSel( m_cboDatabits.FindStringExact( -1, L"8"));
	}

	// 2.2.4 Parity
	nIndex = -1;
	nIndex = m_pSettings->m_dwParity;
	if((nIndex >= 0) && (m_cboParity.GetCount()))
	{
		m_cboParity.SetCurSel( nIndex );
	}
	else
	{
		m_cboParity.SetCurSel( m_cboParity.FindStringExact(-1, L"None"));
	}

	// 2.2.5 Stop Bits
	nIndex = -1;
	str.Empty();
	str.Format( L"%d", m_pSettings->m_byStopBits );
	nIndex = m_cboStop.FindStringExact( -1, str );
	if((nIndex >= 0) && (nIndex < m_cboStop.GetCount()))
	{
		m_cboStop.SetCurSel( nIndex );
	}
	else
	{
		m_cboStop.SetCurSel( m_cboStop.FindStringExact(-1, L"1"));
	}

	// 2.2.6 RTS Control
	nIndex = -1;
	nIndex = m_pSettings->m_byRtsCtl;
	if((nIndex >= 0) && (nIndex < m_cboRTS.GetCount()))
	{
		m_cboRTS.SetCurSel( nIndex );
	}
	else
	{
		m_cboRTS.SetCurSel( m_cboRTS.FindStringExact(-1, L"Disable"));
	}

	// 2.2.7 HandShaking
	if( m_pSettings->m_bHandshake )
	{
		m_chkHand.SetCheck( BST_CHECKED ); 
	}
	else
	{
		m_chkHand.SetCheck( BST_UNCHECKED );
	}

	// 2.2.8 Serial Slave ID
	nIndex = -1;
	str.Empty();
	str.Format( L"%d", m_pSettings->m_bySerSlaveID );
	nIndex = m_cboSerSlaveID.FindStringExact( -1, str );
	if((nIndex >= 0) && (nIndex < m_cboSerSlaveID.GetCount()))
	{
		m_cboSerSlaveID.SetCurSel( nIndex );
	}
	else
	{
		m_cboSerSlaveID.SetCurSel( m_cboSerSlaveID.FindStringExact(-1, L"1"));
	}

	/// 3. TCP Settings
	// 3.1 IP Address
	m_ipAddress.SetAddress( m_pSettings->m_byIPAddress01, m_pSettings->m_byIPAddress02,
		                    m_pSettings->m_byIPAddress03, m_pSettings->m_byIPAddress04 );

	// 3.2 TCP/IP Port
	str.Format( L"%d", m_pSettings->m_dwIPPort );
	m_edtTCPPort.SetWindowText( str );

	// 3.3 TCP/IP Slave ID
	nIndex = -1;
	str.Empty();
	str.Format( L"%d", m_pSettings->m_byTcpSlaveID );
	nIndex = m_cboTCPSlaveID.FindStringExact( -1, str );
	if((nIndex >= 0) && (nIndex < m_cboTCPSlaveID.GetCount()))
	{
		m_cboTCPSlaveID.SetCurSel( nIndex );
	}
	else
	{
		m_cboTCPSlaveID.SetCurSel( m_cboTCPSlaveID.FindStringExact(-1, L"1"));
	}

	// 4 Set Control Enable status
	SetCtlEnableStatus();
}

// =========================================================================
// Function name: GetSettings
// Description  : Set pSettings with dialog control's value             
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.02 18:50:00
// =========================================================================
void CCommSettingsDlg::GetSettings( CCommSettings* pSettings )
{
    if( !CheckWritePtr( pSettings, sizeof(CCommSettings)))
	{
		return;
	}

	bool bModbusTcp = ((m_cboType.GetCurSel() == 1) ? true : false);

	///1. Modbus Type
	pSettings->m_bModbusTcp = bModbusTcp;

	CString str;

	///2. Serial Settings
	//2.1 Serial Port
	if( m_cboSerPort.GetCount() != 0 )
	{
		m_cboSerPort.GetLBText( m_cboSerPort.GetCurSel(), str );
		pSettings->m_strPort = str; 
	}
	else
	{
		pSettings->m_strPort = L"";
	}

	//2.2 Baud
	str.Empty();
	m_cboBaud.GetLBText( m_cboBaud.GetCurSel(), str );
	::swscanf_s( str, L"%d", &pSettings->m_dwBaud );
	
	//2.3 Data Bits
	str.Empty();
	m_cboDatabits.GetLBText( m_cboDatabits.GetCurSel(), str );
	::swscanf_s( str, L"%d", &pSettings->m_byDataBits );

	//2.4 Parity
	pSettings->m_dwParity = m_cboParity.GetCurSel();

	//2.5 Stop Bits
	if( m_cboStop.GetCurSel() == 0 )
	{
		pSettings->m_byStopBits = ONESTOPBIT;
	}
	else
	{
		pSettings->m_byStopBits = TWOSTOPBITS;
	}

	//2.6 RTS Control
	pSettings->m_byRtsCtl = m_cboRTS.GetCurSel();

	//2.7 HandShaking
	pSettings->m_bHandshake = (m_chkHand.GetCheck() == BST_CHECKED) ? true : false;

	//2.8 Serial Slave ID
	str.Empty();
	m_cboSerSlaveID.GetLBText( m_cboSerSlaveID.GetCurSel(), str );
	::swscanf_s( str, L"%d", &pSettings->m_bySerSlaveID );
	
	///3 TCP Settings
	//3.1 IP Address
	m_ipAddress.GetAddress( pSettings->m_byIPAddress01, pSettings->m_byIPAddress02, 
							pSettings->m_byIPAddress03, pSettings->m_byIPAddress04 );

	//3.2 TCP/IP Port
	str.Empty();
	m_edtTCPPort.GetWindowText( str );
	::swscanf_s( str, L"%d", &pSettings->m_dwIPPort );

	//3.3 TCP/IP SlaveID
	str.Empty();
	m_cboTCPSlaveID.GetLBText( m_cboTCPSlaveID.GetCurSel(), str );
	::swscanf_s( str, L"%d", &pSettings->m_byTcpSlaveID );
}

//==============================================================

void CCommSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CBO_SERIALPORT, m_cboSerPort);
	DDX_Control(pDX, IDC_CBO_BAUD, m_cboBaud);
	DDX_Control(pDX, IDC_CBO_DATABITS, m_cboDatabits);
	DDX_Control(pDX, IDC_CBO_PARITY, m_cboParity);
	DDX_Control(pDX, IDC_CBO_STOPBITS, m_cboStop);
	DDX_Control(pDX, IDC_CBO_RTS, m_cboRTS);
	DDX_Control(pDX, IDC_CHK_HAND, m_chkHand);
	DDX_Control(pDX, IDC_CBO_SERSLAVEID, m_cboSerSlaveID);
	DDX_Control(pDX, IDC_IP_ADDRESS, m_ipAddress);
	DDX_Control(pDX, IDC_EDT_TCPPORT, m_edtTCPPort);
	DDX_Control(pDX, IDC_CBO_TCPSLAVEID, m_cboTCPSlaveID);
	DDX_Control(pDX, IDC_BTN_SERDEF, m_btnSerDef);
	DDX_Control(pDX, IDC_BTN_TCPDEF, m_btnTCPDef);
	DDX_Control(pDX, IDC_BTN_TEST, m_btnTest );
	DDX_Control(pDX, IDOK, m_btnOK );
	DDX_Control(pDX, IDC_PCCOMM_CANCEL, m_btnCancel );
	DDX_Control(pDX, IDC_COMM_MSG, m_stcCommMsg);
	DDX_Control(pDX, IDC_BTN_PROSEL, m_btnProtocol);
}


BEGIN_MESSAGE_MAP(CCommSettingsDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CCommSettingsDlg::OnCbnSelchangeComboType)
	ON_BN_CLICKED( IDC_BTN_SERDEF, &OnBtnSerDef )
	ON_BN_CLICKED( IDC_BTN_TCPDEF, &OnBtnTCPDef )
	ON_BN_CLICKED( IDC_BTN_TEST, &OnBtnTest )
	ON_BN_CLICKED( IDOK, &CCommSettingsDlg::OnBnClickedOk )
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PCCOMM_CANCEL, &CCommSettingsDlg::OnBnClickedPccommCancel)
END_MESSAGE_MAP()


// CCommSettingsDlg 消息处理程序

// =======================================================================
// Function name: OnInitDialog
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 
// =======================================================================
BOOL CCommSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// Help ID
	SetWindowContextHelpId( m_dwViewHelpID );

	// Modbus Type
	m_cboType.ResetContent();
	m_cboType.AddString( L"Modbus Serial");
	m_cboType.AddString( L"Modbus TCP/IP");
	m_cboType.SetCurSel( 0 );                // Modbus Serial

	// Modbus Message
	CString strCommMsg( theApp.GetCommMessage( m_cboType.GetCurSel()));
	m_stcCommMsg.SetWindowText( strCommMsg );

	// Hide Protocol Selection Button
	m_btnProtocol.ShowWindow( SW_HIDE );

	/// 1. Serial Settings
	// 1.1 Possible communication ports
    CString strPort;
    for( int i = 1; i <= c_nMaxComPort; ++i )
    {
        if( CCommSettings::Exists(i))
        {
			strPort.Format( L"COM%d", i );
			m_cboSerPort.AddString( strPort );
        }
    }
	m_cboSerPort.SetCurSel( 0 );

	// 1.2 Baud
	m_cboBaud.ResetContent();
	if((CConfiguratorApp::DSC100 == theApp.GetProductType()) ||
	   (CConfiguratorApp::DSC110 == theApp.GetProductType()))
	{
		m_cboBaud.AddString( L"4800");
		m_cboBaud.AddString( L"9600");
		m_cboBaud.AddString( L"38400");
		m_cboBaud.AddString( L"115200");
	}
	else if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		m_cboBaud.AddString( L"1200");
		m_cboBaud.AddString( L"4800");
		m_cboBaud.AddString( L"9600");
		m_cboBaud.AddString( L"19200");
		m_cboBaud.AddString( L"38400");
		m_cboBaud.AddString( L"57600");
		m_cboBaud.AddString( L"115200");
	}
	else
	{
		m_cboBaud.AddString( L"1200");
		m_cboBaud.AddString( L"2400");
		m_cboBaud.AddString( L"4800");
		m_cboBaud.AddString( L"9600");
		m_cboBaud.AddString( L"19200");
		m_cboBaud.AddString( L"38400");
		m_cboBaud.AddString( L"57600");
		m_cboBaud.AddString( L"115200");
		m_cboBaud.AddString( L"128000");
		m_cboBaud.AddString( L"256000");
	}

	int nIndex = m_cboBaud.FindStringExact( -1, L"38400" );
	if( nIndex != CB_ERR )   
	{
	    m_cboBaud.SetCurSel( nIndex );
	}

	// 1.3 Data Bits
	m_cboDatabits.ResetContent();
	m_cboDatabits.AddString( L"7" );
	m_cboDatabits.AddString( L"8" );
	m_cboDatabits.SetCurSel( 1 );           // 8

	// 1.4 Parity
	m_cboParity.ResetContent();
	m_cboParity.AddString( L"None" );
	m_cboParity.AddString( L"Even" );
	m_cboParity.AddString( L"Odd" );
	m_cboParity.SetCurSel( 0 );             // None

	// 1.5 Stop bits
	m_cboStop.ResetContent();
	m_cboStop.AddString( L"1" );
	m_cboStop.AddString( L"2" );
	m_cboStop.SetCurSel( 0 );

	// 1.6 RTS
	m_cboRTS.ResetContent();
	m_cboRTS.AddString( L"Disable" );
	m_cboRTS.AddString( L"Enable" );
	m_cboRTS.AddString( L"Handshake" );
	m_cboRTS.AddString( L"Toggle" );
	m_cboRTS.SetCurSel( 0 );             // Disable

	// 1.7 Hardware Handshaking
	m_chkHand.SetCheck( BST_UNCHECKED );    // Unchecked

	// 1.8 Serial Slave ID
	m_cboSerSlaveID.ResetContent();
	for( int nIDIndex = 1; nIDIndex < c_byMaxSlaveID; nIDIndex++ )
	{
		CString strID;
		strID.Format( L"%d", nIDIndex );
		m_cboSerSlaveID.AddString( strID );
	}
	m_cboSerSlaveID.SetCurSel( 0 );

	/// 2. TCP/IP Settings
	// 2.1 IP Address
	m_ipAddress.ClearAddress();
	m_ipAddress.SetAddress( 192, 168, 1, 1 );    // 192.168.1.1

	// 2.2 Port
	m_edtTCPPort.SetWindowText( L"502" );        // 502

	// 2.3 Slave ID
	m_cboTCPSlaveID.ResetContent();
	for( int nIDIndex = 1; nIDIndex < c_byMaxSlaveID; nIDIndex++ )
	{
		CString strID;
		strID.Format( L"%d", nIDIndex );
		m_cboTCPSlaveID.AddString( strID );
	}
	m_cboTCPSlaveID.SetCurSel( 0 );

	// Enable/Disable
	SetCtlEnableStatus();

	// Set Dialog controls with global CommSettings
	SetSettings( m_pSettings );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// =======================================================================
// Function name: OnCbnSelendokComboType
// Description  : Occured when user confirmed his selection
//                Update control's enable/disable status
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.01 16:00:00
// =======================================================================
void CCommSettingsDlg::OnCbnSelchangeComboType()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCtlEnableStatus();
}

// =======================================================================
// Function name: OnBtnSerDef
// Description  : Reset Dialog Serial Controls;
//                Reset m_pSettings's Serial members
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.03 11:30:00
// =======================================================================
void CCommSettingsDlg::OnBtnSerDef()
{
	int nIndex = CB_ERR;

	/// 1. Dialog Serial Controls
	// Serial Port has no default value
	int nCurSel = m_cboSerPort.GetCurSel();
	if((nCurSel == CB_ERR) && (m_cboSerPort.GetCount() > 0))
	{
		m_cboSerPort.SetCurSel( 0 );
	}

    // Baud: 38400
	nIndex = m_cboBaud.FindStringExact( -1, L"38400" );
	if((nIndex >= 0) && (nIndex < m_cboBaud.GetCount()))
	{
		m_cboBaud.SetCurSel( nIndex );
	}

	// Data Bits: 8
	nIndex = m_cboDatabits.FindStringExact( -1, L"8" );
	if((nIndex >= 0) && (nIndex < m_cboDatabits.GetCount()))
	{
		m_cboDatabits.SetCurSel( nIndex );
	}

	// Parity: None
	nIndex = m_cboParity.FindStringExact( -1, L"None" );
	if((nIndex >= 0) && (nIndex < m_cboParity.GetCount()))
	{
		m_cboParity.SetCurSel( nIndex );
	}

	// Stop bits: 1
	nIndex = m_cboStop.FindStringExact( -1, L"1" );
	if((nIndex >= 0) && (nIndex < m_cboStop.GetCount()))
	{
		m_cboStop.SetCurSel( nIndex );
	}

	// RTS: Disable
	nIndex = m_cboRTS.FindStringExact( -1, L"Disable" );
	if((nIndex >= 0) && (nIndex < m_cboRTS.GetCount()))
	{
		m_cboRTS.SetCurSel( nIndex );
	}

	// Hardware Handshaking: unchecked
	m_chkHand.SetCheck( BST_UNCHECKED ); 

	// SlaveID: 1
	nIndex = m_cboSerSlaveID.FindStringExact( -1, L"1" );
	if((nIndex >= 0) && (nIndex < m_cboSerSlaveID.GetCount()))
	{
		m_cboSerSlaveID.SetCurSel( nIndex );
	}

	///2. Reset m_pSettings's Serial members
	ASSERT( CheckWritePtr( m_pSettings, sizeof(CCommSettings)));
	if( CheckWritePtr( m_pSettings, sizeof(CCommSettings)))
	{
	    m_pSettings->SetDefault( false );

	    // Serial Port has no default value, so set with current serial port control's value
	    m_cboSerPort.GetLBText( m_cboSerPort.GetCurSel(), m_pSettings->m_strPort );
	}
}

// =======================================================================
// Function name: OnBtnTCPDef
// Description  : Reset Serial Settings
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.03 11:40:00
// =======================================================================
void CCommSettingsDlg::OnBtnTCPDef()
{
	int nIndex = CB_ERR;

	/// 1. Dialog TCP Controls
	// IP Address: 192.168.1.1
	m_ipAddress.ClearAddress();
	m_ipAddress.SetAddress( 192, 168, 1, 1 );

	// Port: 502
	m_edtTCPPort.Clear();
	m_edtTCPPort.SetWindowText( L"502" );

	// SlaveID: 1
	nIndex = m_cboTCPSlaveID.FindStringExact( -1, L"1" );
	if( nIndex != CB_ERR )
	{
		m_cboTCPSlaveID.SetCurSel( nIndex );
	}

	///2. Reset m_pSettings's TCP/IP members
	ASSERT( CheckWritePtr( m_pSettings, sizeof(CCommSettings)));
	if( CheckWritePtr( m_pSettings, sizeof(CCommSettings)))
	{
	    m_pSettings->SetDefault( true );
	}
}

// =======================================================================
// Function name: OnBnClickedPccommCancel
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.12.13 14:50:00
// =======================================================================
void CCommSettingsDlg::OnBnClickedPccommCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

// =======================================================================
// Function name: OnBnClickedOk
// Description  : Set m_pSettings
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.01 18:50:00
// =======================================================================
void CCommSettingsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetSettings( m_pSettings );

	CDialogEx::OnOK();
}

// =======================================================================
// Function name: OnBtnTest
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.01 19:31:00
// =======================================================================
void CCommSettingsDlg::OnBtnTest()
{
	// Show a wait cursor
	CWaitCursor wait;   

    // Use the user-defined COMM settings.
    CCommSettings cs;
    GetSettings( &cs );

	try
	{
	    m_btnTest.EnableWindow( FALSE );
	    m_btnOK.EnableWindow( FALSE );
	    m_btnCancel.EnableWindow( FALSE );

	    ((CMainFrame*)AfxGetMainWnd())->TestCommSettings( &cs );

		SetTimer( 101, 100, NULL );
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occurs in CCommSettingsDlg::OnBtnTest!" );
	}
}

// =======================================================================
// Function name: OnTimer
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.01 19:31:00
// =======================================================================
void CCommSettingsDlg::OnTimer( UINT_PTR nIDEvent )
{
	if( WaitForSingleObject( g_pTestCommEvent->m_hObject, 0 ) == WAIT_OBJECT_0 )
	{
	    m_btnTest.EnableWindow( TRUE );
        m_btnOK.EnableWindow( TRUE );
	    m_btnCancel.EnableWindow( TRUE );
	}
}

//-------------------------------------------------------------------------
