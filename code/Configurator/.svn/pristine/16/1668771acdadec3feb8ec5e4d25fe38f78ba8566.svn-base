#include "StdAfx.h"
#include "HistoryServer.h"

#include "Configurator.h"
#include "ConfiguratorDoc.h"
#include "ModbusThread.h"

const ULONGLONG MINFREESPACE = 10; // 磁盘最小可用空间为10M
extern std::auto_ptr<modbus::CModbusThread> ModbusThread;

//Hitory Record Thread Function
UINT CHistoryServer::RecordThreadFunc( LPVOID pParam )
{
	theApp.LogInfo( L"Record Thread run" );
	CConfiguratorDoc *pDoc = (CConfiguratorDoc*)pParam;
	CHistoryServer *pHisServer = pDoc->GetHistoryServer();
	CWinThread *pHisThread = pDoc->GetHistoryThread();
	CRecordHistoryCtrl* pRecordCtrl = pDoc->GetHisRecordCtrl();
	ASSERT( pDoc && pHisServer && pHisThread && pRecordCtrl );

	while( !pHisServer->m_bExit )
	{
		if( pHisServer->m_bSuspend )
		{
			pDoc->RecordHistoryFile();
			pHisServer->m_dwStartTime = 0;
			pHisServer->SetRecording( false );
			theApp.LogInfo( L"Record Thread Suspended!" );
			pDoc->GetHisRecordSusEvent()->SetEvent();
			pHisThread->SuspendThread();
		}

		// Disk Space
		CString strDisk( pRecordCtrl->GetFilePathName());
		strDisk = strDisk.Left( 3 );  // such as "D:\\"
		ULARGE_INTEGER freeBytes;
		memset( &freeBytes, 0, sizeof(ULARGE_INTEGER));
		if( GetDiskFreeSpaceExW( strDisk, NULL, NULL, &freeBytes ))
		{
			if( freeBytes.QuadPart/(1024*1024) < MINFREESPACE )
			{
				CString str;
				str.Format( IDS_DISK_NOSPACE, strDisk, freeBytes.QuadPart/(1024*1024), MINFREESPACE );
				AfxMessageBox( str );
				pHisServer->m_bSuspend = true;
			}
		}
		else
		{
			CString str;
			str.Format( L" GetDiskFreeSpaceEx error:%d-%s", HrGetMessage(::GetLastError()));
			AfxMessageBox( str );
			pHisServer->m_bSuspend = true;
		}

		if( !pHisServer->m_bSuspend )
		{
			// Cache local: Sampling Points
			DWORD dwTime = GetTickCount();
			if( ModbusThread->HasEvent(2000))
			{
				pDoc->m_aHisRecordDatas->CacheRecordDatas();

				// Sleep
				DWORD dwSleep = pRecordCtrl->GetSamplingRate()*1000 - ( GetTickCount()-dwTime );
				if( dwSleep > 0 )
				{
					::Sleep( dwSleep );  // ms
				}
			}

			// Record to File
			dwTime = GetTickCount();
			if((dwTime - pHisServer->m_dwStartTime) > pRecordCtrl->GetInterval()*1000)
			{
				pDoc->RecordHistoryFile();   // Record Local datas to History File
				pDoc->CheckCurRecordFile();  // If maximum, new create a record file
			    
				pHisServer->m_dwStartTime = GetTickCount();
			
				/*CString str;
				str.Format( L"Record File used %dms", pHisServer->m_dwStartTime - dwTime );
				theApp.LogInfo( str );*/
			}
		}
	}//while

	return 0;
}

