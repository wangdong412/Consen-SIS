// CtlCommSettingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "CtlCommSettingsDlg.h"
#include "afxdialogex.h"

#include "ConfiguratorDoc.h"
#include "MainFrm.h"
#include "CTRLDateTimeDlg.h"
#include "ModbusThread.h"

extern std::auto_ptr<modbus::CModbusThread> ModbusThread;

// CCtlCommSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CCtlCommSettingsDlg, CDialogEx)

CCtlCommSettingsDlg::CCtlCommSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCtlCommSettingsDlg::IDD, pParent)
{
    // GetDocument
	m_pMainWnd = (CMainFrame*)theApp.GetMainWnd();
	ASSERT( m_pMainWnd );
	m_pDoc = (CConfiguratorDoc*)m_pMainWnd->GetActiveDocument();
	ASSERT( m_pDoc );
}

CCtlCommSettingsDlg::~CCtlCommSettingsDlg()
{
}

void CCtlCommSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBO_SER1TYPE, m_cboType1);
	DDX_Control(pDX, IDC_CBO_SER1BAUD, m_cboBaudRate1);
	DDX_Control(pDX, IDC_CBO_SER1STOP, m_cboSB1);
	DDX_Control(pDX, IDC_CBO_SER1DATA, m_cboDB1);
	DDX_Control(pDX, IDC_CBO_SER1PAR, m_cboPB1);
	DDX_Control(pDX, IDC_CBO_SER1ID, m_cboSer1ID);
	DDX_Control(pDX, IDC_CBO_PROSER1, m_cboPT1);
	DDX_Control(pDX, IDC_CBO_SER2TYPE, m_cboType2);
	DDX_Control(pDX, IDC_CBO_SER2BAUD, m_cboBaudRate2);
	DDX_Control(pDX, IDC_CBO_SER2STOP, m_cboSB2);
	DDX_Control(pDX, IDC_CBO_SER2DATA, m_cboDB2);
	DDX_Control(pDX, IDC_CBO_SER2PAR, m_cboPB2);
	DDX_Control(pDX, IDC_CBO_SER2ID, m_cboSer2ID);
	DDX_Control(pDX, IDC_CBO_PROSER2, m_cboPT2);
	DDX_Control(pDX, IDC_ETH_IP, m_ctlIP);
	DDX_Control(pDX, IDC_ETH_MASK, m_ctlMask);
	DDX_Control(pDX, IDC_ETH_GATE, m_ctlGate);
	DDX_Control(pDX, IDC_CBO_TCPID, m_cboTCPID);
	DDX_Control(pDX, IDC_EDT_CTLTCPPN, m_edtPN);
	DDX_Control(pDX, IDC_CHK_SER1, m_chkSer1);
	DDX_Control(pDX, IDC_CHK_SER2, m_chkSer2);
}

// =========================================================================
// Function name: EnableCtrls
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.11.21 09:00:00
// =========================================================================
void CCtlCommSettingsDlg::EnableCtrls( BOOL bSerial1, BOOL bSerial2 )
{
	// Serial Port 1
	m_chkSer1.SetCheck( bSerial1 ?  BST_CHECKED : BST_UNCHECKED );  // Check
	m_cboType1.EnableWindow( bSerial1 );                            // COM1 Type
    m_cboBaudRate1.EnableWindow( bSerial1 );                        // Baud Rate
	m_cboSB1.EnableWindow( bSerial1 );                              // Stop Bits
	m_cboDB1.EnableWindow( bSerial1 );                              // Data Bits
	m_cboPB1.EnableWindow( bSerial1 );                              // Parity Bits
	m_cboSer1ID.EnableWindow( bSerial1 );                           // Slave ID
	m_cboPT1.EnableWindow( bSerial1 );                              // Protocol Type

	// Serial Port 2
	m_chkSer2.SetCheck( bSerial2 ?  BST_CHECKED : BST_UNCHECKED );  // Check
	m_cboType2.EnableWindow( bSerial2 );                            // COM2 Type
    m_cboBaudRate2.EnableWindow( bSerial2 );                        // Baud Rate
	m_cboSB2.EnableWindow( bSerial2 );                              // Stop Bits
	m_cboDB2.EnableWindow( bSerial2 );                              // Data Bits
	m_cboPB2.EnableWindow( bSerial2 );                              // Parity Bits
	m_cboSer2ID.EnableWindow( bSerial2 );                           // Slave ID
	m_cboPT2.EnableWindow( bSerial2 );                              // Protocol Type
}


BEGIN_MESSAGE_MAP(CCtlCommSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHK_SER1, &CCtlCommSettingsDlg::OnBnClickedChkSer1)
	ON_BN_CLICKED(IDC_CHK_SER2, &CCtlCommSettingsDlg::OnBnClickedChkSer2)
	ON_BN_CLICKED(IDC_BTN_DATETIME, &CCtlCommSettingsDlg::OnBnClickedBtnDatetime)
END_MESSAGE_MAP()


// CCtlCommSettingsDlg 消息处理程序


BOOL CCtlCommSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// Serial Port 1
	m_cboType1.AddString( L"RS-422" );   
	m_cboType1.AddString( L"RS-485" );
	m_cboType1.AddString( L"RS-232" );

	m_cboBaudRate1.AddString( L"1200" );
	m_cboBaudRate1.AddString( L"4800" );
	m_cboBaudRate1.AddString( L"9600" );
    m_cboBaudRate1.AddString( L"19200" );
	m_cboBaudRate1.AddString( L"38400" );
	m_cboBaudRate1.AddString( L"57600" );
	m_cboBaudRate1.AddString( L"115200" );

	m_cboSB1.AddString( L"1" );
	m_cboSB1.AddString( L"2" );

	m_cboDB1.AddString( L"8" );

	m_cboPB1.AddString( L"None" );
	m_cboPB1.AddString( L"Even" );
	m_cboPB1.AddString( L"Odd" );

	m_cboPT1.AddString( L"RTU" );
	m_cboPT1.AddString( L"ASCII" );

	// Serial Port 2
	m_cboType2.AddString( L"RS-422" );   
	m_cboType2.AddString( L"RS-485" );

	m_cboBaudRate2.AddString( L"1200" );
	m_cboBaudRate2.AddString( L"4800" );
	m_cboBaudRate2.AddString( L"9600" );
    m_cboBaudRate2.AddString( L"19200" );
	m_cboBaudRate2.AddString( L"38400" );
	m_cboBaudRate2.AddString( L"57600" );
	m_cboBaudRate2.AddString( L"115200" );

	m_cboSB2.AddString( L"1" );
	m_cboSB2.AddString( L"2" );

	m_cboDB2.AddString( L"8" );

	m_cboPB2.AddString( L"None" );
	m_cboPB2.AddString( L"Even" );
	m_cboPB2.AddString( L"Odd" );
	
	m_cboPT2.AddString( L"RTU" );
	m_cboPT2.AddString( L"ASCII" );

	for( int i = 1; i < 248; i++ )
	{
		CString str;
		str.Format( L"%d", i );
		m_cboSer1ID.AddString( str );
		m_cboSer2ID.AddString( str );
		m_cboTCPID.AddString( str );
	}

	/// Set ctrl value with corresponding alias
	/// Serial Port
	//1.1 Serial Port 1
	CAliasItem *pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1Type" ); // COM1 Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 2 )
		{
			value = 2;
		}

		m_cboType1.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1Baud" );  // Baud Rate
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 6 )
		{
			value = 6;
		}

		m_cboBaudRate1.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1SB" );     // Stop Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 1 )
		{
			value = 1;
		}

		m_cboSB1.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1DB" );     // Data Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 0 )
		{
			value = 0;
		}

		m_cboDB1.SetCurSel( value );
	}
	
	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1Parity" );  // Parity Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 2 )
		{
			value = 2;
		}

		m_cboPB1.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1ID" );     // Slave ID
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value < 1 )
		{
			value = 1;
		}
		else if( value > 247 )
		{
			value = 247;
		}

		m_cboSer1ID.SetCurSel( value - 1 );  // Zero-Based Index
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1ProType" );     // Protocol Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 1 )
		{
			value = 0;     // 0: RTU; 1: ASCII
		}

		m_cboPT1.SetCurSel( value );
	}

	//1.2 Serial Port 2
	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2Type" ); // COM2 Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 1 )
		{
			value = 1;
		}

		m_cboType2.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2Baud" );  // Baud Rate
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 6 )
		{
			value = 6;
		}

		m_cboBaudRate2.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2SB" );     // Stop Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 1 )
		{
			value = 1;
		}

		m_cboSB2.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2DB" );     // Data Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 0 )
		{
			value = 0;
		}

		m_cboDB2.SetCurSel( value );
	}
	
	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2Parity" );  // Parity Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 2 )
		{
			value = 2;
		}

		m_cboPB2.SetCurSel( value );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2ID" );     // Slave ID
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value < 1 )
		{
			value = 1;
		}
		else if( value > 247 )
		{
			value = 247;
		}

		m_cboSer2ID.SetCurSel( value - 1 );     // Zero-Based Index
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2ProType" );     // Protocol Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value > 1 )
		{
			value = 0;     // 0: RTU; 1: ASCII
		}

		m_cboPT2.SetCurSel( value );
	}

	//1.3 Serial Port Enable
	BOOL bSer1 = FALSE, bSer2 = FALSE;
	pAlias = m_pDoc->m_aAliases->find_name( L"eCom1En" );   // Port 1 Enable
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		bSer1 = pAlias->remBool() ? TRUE : FALSE;
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"eCom2En" );   // Port 2 Enable
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		bSer2 = pAlias->remBool() ? TRUE : FALSE;
	}

	EnableCtrls( bSer1, bSer2 );

	///2. Ethernet Port Settings
	//2.1 IP Address
	UINT uIP01 = 0, uIP02 = 0, uIP03 = 0, uIP04 = 0;
	pAlias = m_pDoc->m_aAliases->find_name( L"iIP01" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uIP01 = pAlias->remI2();
		if( uIP01 > 255 )
		{
			uIP01 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iIP02" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uIP02 = pAlias->remI2();
		if( uIP02 > 255 )
		{
			uIP02 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iIP03" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uIP03 = pAlias->remI2();
		if( uIP03 > 255 )
		{
			uIP03 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iIP04" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uIP04 = pAlias->remI2();
		if( uIP04 > 255 )
		{
			uIP04 = 255;     
		}
	}

	m_ctlIP.SetAddress( uIP01, uIP02, uIP03, uIP04 );

	//2.2 Subnet Mask
	UINT uMask01 = 0, uMask02 = 0, uMask03 = 0, uMask04 = 0;
	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask01" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uMask01 = pAlias->remI2();
		if( uMask01 > 255 )
		{
			uMask01 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask02" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uMask02 = pAlias->remI2();
		if( uMask02 > 255 )
		{
			uMask02 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask03" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uMask03 = pAlias->remI2();
		if( uMask03 > 255 )
		{
			uMask03 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask04" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uMask04 = pAlias->remI2();
		if( uMask04 > 255 )
		{
			uMask04 = 255;     
		}
	}

	m_ctlMask.SetAddress( uMask01, uMask02, uMask03, uMask04 );

	//2.3 Gate way
	UINT uGate01 = 0, uGate02 = 0, uGate03 = 0, uGate04 = 0;
	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway01" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uGate01 = pAlias->remI2();
		if( uGate01 > 255 )
		{
			uGate01 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway02" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uGate02 = pAlias->remI2();
		if( uGate02 > 255 )
		{
			uGate02 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway03" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uGate03 = pAlias->remI2();
		if( uGate03 > 255 )
		{
			uGate03 = 255;     
		}
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway04" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		uGate04 = pAlias->remI2();
		if( uGate04 > 255 )
		{
			uGate04 = 255;     
		}
	}

	m_ctlGate.SetAddress( uGate01, uGate02, uGate03, uGate04 );

	//2.4 Modbus TCP Slave ID
	pAlias = m_pDoc->m_aAliases->find_name( L"iTCPID" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		UINT value = pAlias->remI2();
		if( value < 1 )
		{
			value = 1;
		}
		else if( value > 247 )
		{
			value = 247;
		}

		m_cboTCPID.SetCurSel( value - 1 );  // Zero-Based Index
	}

	//2.5 Modbus TCP Port Number
	pAlias = m_pDoc->m_aAliases->find_name( L"iPortNum" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		CString str;
		str.Format( L"%d", pAlias->remI2());
		m_edtPN.SetWindowTextW( str );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCtlCommSettingsDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	///1. Serial Port
	//1.1 Serial Port 1
	CAliasItem *pAlias = m_pDoc->m_aAliases->find_name( L"eCom1En" );      // Ser1 Enable
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valBool( BST_CHECKED == m_chkSer1.GetCheck()); 
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1Type" );    // COM1 Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboType1.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1Baud" );    // Baud Rate
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboBaudRate1.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1SB" );      // Stop Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboSB1.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1DB" );      // Data Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboDB1.GetCurSel());
	}
	
	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1Parity" );  // Parity Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboPB1.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1ID" );      // Slave ID
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboSer1ID.GetCurSel() + 1 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM1ProType" ); // Protocol Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboPT1.GetCurSel());  // 0: RTU; 1: ASCII
	}

	//1.2 Serial Port 2
	pAlias = m_pDoc->m_aAliases->find_name( L"eCom2En" );      // Ser1 Enable
	if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
		pAlias->valBool( BST_CHECKED == m_chkSer2.GetCheck()); 
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2Type" );    // COM2 Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboType2.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2Baud" );    // Baud Rate
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboBaudRate2.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2SB" );      // Stop Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboSB2.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2DB" );      // Data Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboDB2.GetCurSel());
	}
	
	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2Parity" );  // Parity Bits
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboPB2.GetCurSel());
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2ID" );      // Slave ID
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboSer2ID.GetCurSel() + 1 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iCOM2ProType" ); // Protocol Type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboPT2.GetCurSel());  // 0: RTU; 1: ASCII
	}

	///2. Ethernet Port
	//2.1 IP
	BYTE byIP01 = 0, byIP02 = 0, byIP03 = 0, byIP04 = 0;
	m_ctlIP.GetAddress( byIP01, byIP02, byIP03, byIP04 );

	pAlias = m_pDoc->m_aAliases->find_name( L"iIP01" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byIP01 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iIP02" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byIP02 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iIP03" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byIP03 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iIP04" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byIP04 );
	}

	//2.2 Subnet Mask
	BYTE byMask01 = 0, byMask02 = 0, byMask03 = 0, byMask04 = 0;
	m_ctlMask.GetAddress( byMask01, byMask02, byMask03, byMask04 );

	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask01" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byMask01 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask02" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byMask02 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask03" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byMask03 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iNetMask04" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byMask04 );
	}

	//2.3 Gate way
	BYTE byGate01 = 0, byGate02 = 0, byGate03 = 0, byGate04 = 0;
	m_ctlGate.GetAddress( byGate01, byGate02, byGate03, byGate04 );

	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway01" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byGate01 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway02" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byGate02 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway03" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byGate03 );
	}

	pAlias = m_pDoc->m_aAliases->find_name( L"iGateway04" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( byGate04 );
	}

	//2.4 Modbus TCP Slave ID
	pAlias = m_pDoc->m_aAliases->find_name( L"iTCPID" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		pAlias->valI2( m_cboTCPID.GetCurSel() + 1 );  // Zero-Based Index 
	}

	//2.5 Modbus TCP Port Number
	pAlias = m_pDoc->m_aAliases->find_name( L"iPortNum" );     
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		CString strValue;
		m_edtPN.GetWindowTextW( strValue );
		int value = 0;
		::swscanf_s( strValue, L"%d", &value );
		pAlias->valI2( value );
	}

	CDialogEx::OnOK();
}

// =========================================================================
// Function name: OnBnClickedChkSer1
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.11.21 10:00:00
// =========================================================================
void CCtlCommSettingsDlg::OnBnClickedChkSer1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnable = (BST_CHECKED == m_chkSer1.GetCheck());

	m_cboType1.EnableWindow( bEnable );        // COM1 Type
    m_cboBaudRate1.EnableWindow( bEnable );    // Baud Rate
	m_cboSB1.EnableWindow( bEnable );          // Stop Bits
	m_cboDB1.EnableWindow( bEnable );          // Data Bits
	m_cboPB1.EnableWindow( bEnable );          // Parity Bits
	m_cboSer1ID.EnableWindow( bEnable );       // Slave ID
	m_cboPT1.EnableWindow( bEnable );          // Protocol Type
}

// =========================================================================
// Function name: OnBnClickedChkSer2
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.11.21 10:00:00
// =========================================================================
void CCtlCommSettingsDlg::OnBnClickedChkSer2()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnable = (BST_CHECKED == m_chkSer2.GetCheck());

	m_cboType2.EnableWindow( bEnable );        // COM2 Type
    m_cboBaudRate2.EnableWindow( bEnable );    // Baud Rate
	m_cboSB2.EnableWindow( bEnable );          // Stop Bits
	m_cboDB2.EnableWindow( bEnable );          // Data Bits
	m_cboPB2.EnableWindow( bEnable );          // Parity Bits
	m_cboSer2ID.EnableWindow( bEnable );       // Slave ID
	m_cboPT2.EnableWindow( bEnable );          // Protocol Type
}

// =========================================================================
// Function name: OnBnClickedBtnDatetime
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2012.01.12 15:00:00
// =========================================================================
void CCtlCommSettingsDlg::OnBnClickedBtnDatetime()
{
	// TODO: 在此添加控件通知处理程序代码
	//1. Start monitoring to update corresponding parameters if TS350 and not monitoring
	bool bPreMonitor = false;
	if( ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR )
	{
        bPreMonitor = true;
	}
	else
	{
		m_pMainWnd->DoMonitor();
	}

	// Popup 'Controller Date And Time' Dialog
	std::auto_ptr<CCTRLDateTimeDlg> dlg( new CCTRLDateTimeDlg());
	if((dlg.get() != NULL) && (IDOK == dlg->DoModal()))
	{
	    m_pMainWnd->SendCTRLComSettings( true );
	}

	if( !bPreMonitor )
	{
		if( ModbusThread->HasEvent(2000))
		{
			m_pMainWnd->DoStopMonitor();
		}
	}
}
