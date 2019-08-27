#include "StdAfx.h"
#include "CalibrateBase.h"

#include "Configurator.h"
#include "MainFrm.h"
#include "ConfiguratorDoc.h"
#include "ModbusThread.h"

#include "CalSheet.h"              // Calibration 


extern std::auto_ptr<modbus::CModbusThread> ModbusThread;



// CCalibrateBase
void CCalibrateBase::Run()
{
	// do nothing
}



// DSC100 Calibrate control
/*******************************************************/
// Check if we can calibrate DSC100
/*******************************************************/
bool CDSC100Calibrate::CanCalibrate()
{
	bool bCanCalibrate = true;
    if( IsCheckShutdown())    // need to check shutdown status
	{
		// when shutdown, we can calibrate 
		bCanCalibrate = IsShutdown();
	}

	return bCanCalibrate;
}


/*******************************************************/
// Send DSC100 calibration flags
/*******************************************************/
void CDSC100Calibrate::SendCalibrationFlags( bool bSave )
{
	ASSERT( m_pMainFrame->GetSafeHwnd());
	m_pMainFrame->SendCalibrationFlags( CConfiguratorApp::DSC100, bSave );
}

/*******************************************************************/
// Check if we need to check DSC100's shutdown status
// 12065(Can not Run?): false: can run, need to check shutdown status
/*******************************************************************/
bool CDSC100Calibrate::IsCheckShutdown()
{
	ASSERT( m_pMainFrame->GetSafeHwnd());
	bool bCheckShutdown = true;

	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)(m_pMainFrame->GetActiveDocument());
    if( pDoc && m_pMainFrame->IsConnected())
    {
		std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
	    CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"dCanRun" );  // 12065: Can not Run?
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			pAliases->Add( pAlias );
		}

		m_pMainFrame->ReadAliasesFromDevice( pAliases.get(), true );

		pAliases->clear();
		pAliases.reset();

		// Wait read done and then check if shutdown
		if( ModbusThread->HasEvent( 2000 ))   
		{
			// false: can run, need to check shutdown status
			bCheckShutdown = (pAlias->remBool()==VARIANT_FALSE);
		}
	}

	return bCheckShutdown;
}

/*******************************************************************/
// Reset Null Current Offset(42161) and Demand (%) 42123
/*******************************************************************/
void CDSC100Calibrate::ResetCalibrationDatas()
{
	ASSERT( m_pMainFrame->GetSafeHwnd());

	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)(m_pMainFrame->GetActiveDocument());
    if( pDoc && m_pMainFrame->IsConnected())
    {
		std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
		if( NULL == pAliases.get())
		{
			return ;
		}

		CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"kNull1" );  // 42161: Null Current Offset
		if( CheckWritePtr(pAlias, sizeof(CAliasItem)))
		{
			pAlias->valR4(0.0);
			pAliases->Add( pAlias );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kDemand" );             // 42123: Demand (%)
		if( CheckWritePtr(pAlias, sizeof(CAliasItem)))
		{
			pAlias->valR4(0.0);
			pAliases->Add( pAlias );
		}

		m_pMainFrame->WriteAliasesToDevice( pAliases.get(), true );

		pAliases->clear();
		pAliases.reset();
	}
}

/*******************************************************************/
// Check DSC100's shutdown status
// 12062(Can not Run?): false: shutdown
/*******************************************************************/
bool CDSC100Calibrate::IsShutdown()
{
	ASSERT( m_pMainFrame->GetSafeHwnd());
	bool bShutdown = false;

	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)(m_pMainFrame->GetActiveDocument());
    if( pDoc && m_pMainFrame->IsConnected())
    {
		// Read shutdown alias
		std::auto_ptr<CAliasMap> pAliases( new CAliasMap());

		CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"dDO2V" );  // 12062: Shutdown
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			pAliases->Add( pAlias );
		}

		pAlias = pDoc->m_aAliases->find_name( L"nPVSel" );             // 40251: PV Select Algorithm
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			pAliases->Add( pAlias );
		}

		m_pMainFrame->ReadAliasesFromDevice( pAliases.get(), true );

		pAliases->clear();
		pAliases.reset();
	
		// Wait read done and then check if shutdown
		if( ModbusThread->HasEvent( 2000 ))   
		{
			CAliasItem* pShutdown = pDoc->m_aAliases->find_name( L"dDO2V" );   // 12062: Shutdown status
			if( CheckReadPtr( pShutdown, sizeof(CAliasItem)) )
			{
				bShutdown = (pShutdown->remBool()==VARIANT_FALSE);    // false: shutdown
				if( !bShutdown )  //not shutdown, can not calibrate
				{
					CString str;
					str.LoadString( IDS_CAL_UNSHUTDOWN );
					AfxMessageBox( str );
				}
			}
		}
	}

	return bShutdown;
}

/*******************************************************************/
// Set DSC100's calibration flag
/*******************************************************************/
void CDSC100Calibrate::SetAliasFlag( bool bInfoFlag,  bool bMinFlag, bool bMaxFlag, bool bVerFlag,
                                     bool bSaveFlag,  bool bIsCal,   bool bCancel,  bool bRun, bool bChooseNo )
{
	ASSERT( m_pMainFrame->GetSafeHwnd());
    CConfiguratorDoc* pDoc = (CConfiguratorDoc*)(m_pMainFrame->GetActiveDocument());
    if( pDoc )
    {
		CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"kFlag_Inf" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bInfoFlag );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_Min" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bMinFlag );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_Max" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bMaxFlag );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_Ver" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bVerFlag );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_Save" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bSaveFlag );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_Cal" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bIsCal );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_Cancel" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bCancel );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_Run" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bRun );
		}

		pAlias = pDoc->m_aAliases->find_name( L"kFlag_NotCal" );
		if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
		{
			pAlias->valBool( bChooseNo );
		}
	}//if( pDoc )
}

/*******************************************************************/
// when user can calibrate
/*******************************************************************/
void CDSC100Calibrate::Calibrate( std::auto_ptr<CCalSheet> &CalSheet )
{
	ASSERT( CalSheet.get() && m_pMainFrame->GetSafeHwnd());

	/* Debug
	// Show calibrate dialog
	CalSheet->SetWizardMode();
	CalSheet->DoModal();*/

	CWaitCursor wait;
	SetAliasFlag( false, false, false, false,
                  false, true, false, false, false );
	SendCalibrationFlags( false );

	if( ModbusThread->HasEvent( 2000 ))   // wait send done 
	{
		ResetCalibrationDatas();
		if( ModbusThread->HasEvent( 2000 ))   // wait send done 
		{
			// Start monitor if not monitoring
			m_bOrigMonitoring = (ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR);
			if( !m_bOrigMonitoring )
			{
				m_pMainFrame->DoMonitor(); 
			}

			// Show calibrate dialog
			CalSheet->SetWizardMode();
			CalSheet->DoModal();
		}
	}
}

/*******************************************************************/
// when user click run
/*******************************************************************/
void CDSC100Calibrate::Run()
{
	ASSERT( m_pMainFrame->GetSafeHwnd());
	if( m_pMainFrame->GetSafeHwnd() && m_pMainFrame->IsConnected())
    {
		CConfiguratorDoc* pDoc = (CConfiguratorDoc*)(m_pMainFrame->GetActiveDocument());
		if((!CheckWritePtr( pDoc, sizeof(CConfiguratorDoc))) ||
		   (!CheckWritePtr( ModbusThread.get(), sizeof(modbus::CModbusThread))))
		{
			return;
		}

		// Read 12065: Can not Run?
		std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
	    CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"dCanRun" );  // 12065: Can not Run?
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			pAliases->Add( pAlias );
		}

		m_pMainFrame->ReadAliasesFromDevice( pAliases.get(), true );

		pAliases->clear();
		pAliases.reset();

		// Wait read done
		if( ModbusThread->HasEvent( 2000 ))   
		{
			if( pAlias->remBool())  // true: Can't Run, need calibrate first.
			{
				pAlias = pDoc->m_aAliases->find_name( L"kFlag_NotCal" );    // 2209
				if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
				{  
					CString str;
					str.LoadStringW( IDS_CAL_CHOSE );
					if( AfxMessageBox( str, MB_YESNO | MB_ICONQUESTION ) == IDNO )  // Choose not Calibrate
					{
						pDoc->SetModifiedFlag(true);

						// Set true only when user choose no
						// Reset all the other Flags  2201 бл 2207, Set Run Flag(2210) true, Set user choose no flag(2209) true.
						SetAliasFlag( false, false, false, false,
									  false, false, false, true, true );

						// Send all the new Flags.    
						SendCalibrationFlags( true );
					}
				}
			}
			else // false: Can Run
			{
				pDoc->SetModifiedFlag(true);

				// Reset all the other Flags  2201 бл 2208, Set Run Flag true.
				SetAliasFlag( false, false, false, false,
							  false, false, false, true, false );

				// Send all the new Flags.    bool bSave = false, Unsave calibrate parameters to eeprom
				SendCalibrationFlags( false );
			}
		}
	}
}











//=======================================================
// DSC110 Calibrate control
/*******************************************************/
// Check if we can calibrate DSC100
/*******************************************************/
bool CDSC110Calibrate::CanCalibrate()
{
	//return true;       // Debug
	return IsShutdown();
}

/*******************************************************************/
// Check DSC100's shutdown status
// 12062(Can not Run?): false: shutdown
/*******************************************************************/
bool CDSC110Calibrate::IsShutdown()
{
	bool bShutdown = false;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if( pFrame->GetSafeHwnd())
	{
		CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pFrame->GetActiveDocument();
		if( pDoc && pFrame->IsConnected())
		{
			// Read shutdown alias
			std::auto_ptr<CAliasMap> pAliases( new CAliasMap());

			CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"dDO2V" );   // 10004: DO Shutdown
			if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
			{
				pAliases->Add( pAlias );
			}

			pFrame->ReadAliasesFromDevice( pAliases.get(), true );

			pAliases->clear();
			pAliases.reset();
	
			// Wait read done and then check if shutdown
			if( ModbusThread->HasEvent( 2000 ))   
			{
				bShutdown = (pAlias->remBool() != VARIANT_FALSE);    // 1: shutdown
				if( !bShutdown )  //not shutdown, can not calibrate
				{
					CString str;
					str.LoadString( IDS_CAL_UNSHUTDOWN );
					AfxMessageBox( str );
				}
			}
		}
	}

	return bShutdown;
}

/*******************************************************************/
// Check DSC100's shutdown status
// 12062(Can not Run?): false: shutdown
/*******************************************************************/
void CDSC110Calibrate::SetAliasFlag( bool bAngleFlag, bool bVerFlag, bool bSaveRunFlag,  bool bCancel )
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if( pFrame->GetSafeHwnd())
	{
		CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pFrame->GetActiveDocument();
		if( pDoc )
		{
			CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"kFlag_Cal" );
			if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
			{
				pAlias->valBool( bAngleFlag );
			}

			pAlias = pDoc->m_aAliases->find_name( L"kFlag_Ver" );
			if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
			{
				pAlias->valBool( bVerFlag );
			}

			pAlias = pDoc->m_aAliases->find_name( L"kFlag_SaveRun" );
			if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
			{
				pAlias->valBool( bSaveRunFlag );
			}

			pAlias = pDoc->m_aAliases->find_name( L"kFlag_Cancel" );
			if( CheckWritePtr( pAlias, sizeof( CAliasItem )))
			{
				pAlias->valBool( bCancel );
			}

			pFrame->SendCalibrationFlags( CConfiguratorApp::DSC110, bSaveRunFlag );
		}//if( pDoc )
	}
}


/*******************************************************************/
// when user can calibrate
/*******************************************************************/
void CDSC110Calibrate::Calibrate( std::auto_ptr<CCalSheet> &CalSheet )
{
	ASSERT( CalSheet.get());

	/* Debug
	// Show calibrate dialog
	CalSheet->SetWizardMode();
	CalSheet->DoModal();
	
	// Modaless
	//CalSheet->Create();
	//CalSheet->ShowWindow( SW_SHOW );*/
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if( pFrame->GetSafeHwnd())
	{
		CWaitCursor wait;
		SetAliasFlag( true, false, false, false );

		if( ModbusThread->HasEvent( 2000 ))   // wait send done 
		{
			// Start monitor if not monitoring
			m_bOrigMonitoring = (ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR);
			if( !m_bOrigMonitoring )
			{
				pFrame->DoMonitor(); 
			}

			// Show calibrate dialog
			CalSheet->SetWizardMode();
			CalSheet->DoModal();
		}
	}
}