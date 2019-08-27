
#include "stdafx.h"
#include "HistoryFileLoadCtrl.h"

#include "Configurator.h"
#include "HistoryFileData.h"
#include "ConfiguratorDoc.h"

CHistoryFileLoadCtrl::CHistoryFileLoadCtrl( CConfiguratorDoc* pDoc, CString strViewFileName )
					: m_pDoc( pDoc )
                    , m_bDlgLoad( false )
					, m_strLoadFileName( L"" )                    // 被加载的文件名
					, m_strViewFileName( strViewFileName )        // 之前被使用的文件名
					, m_aHisFileDatas( new CHistoryFileDatas())
{
	ASSERT( m_pDoc != NULL );
}

CHistoryFileLoadCtrl::~CHistoryFileLoadCtrl()
{
	SAFE_DELETE( m_aHisFileDatas );
}

// =======================================================================
// Function name  : Load
// Description    : 
// Return type    : 
// Parameters     : strSelName: 用户选择加载的历史数据文件
// Author         : WangDong 2013.04.17 16:00:00
// =======================================================================
bool CHistoryFileLoadCtrl::Load(const CString strSelName)
{
	bool bResult = true;

	// Check if strSelName is same array with loaded file
	if( m_bDlgLoad )
	{
		if( IsHisFileSameArray( m_strLoadFileName, strSelName ))
		{
			return bResult;
		}
	}

	// Check if strSelName is same array with ViewCtrlFileName
	if( IsHisFileSameArray( m_strViewFileName, strSelName ))
	{
		m_aHisFileDatas->Clear();
		m_bDlgLoad = false;
	}
	else
	{
		// Load user selected history file
		bResult = m_pDoc->LoadHisFile( strSelName, m_aHisFileDatas );
		if( bResult )
		{
			// Update
			m_strLoadFileName = strSelName;
			m_bDlgLoad = true;
		}
	}

	return bResult;
}
