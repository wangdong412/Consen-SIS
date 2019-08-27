// BaseGridView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "BaseGridView.h"

// Added by wangdong
#include "SettingItem.h"           
#include "AliasItem.h"
#include "ConfiguratorDoc.h"

// CBaseGridView

IMPLEMENT_DYNCREATE(CBaseGridView, CConfiguratorView)                 

CBaseGridView::CBaseGridView()
{

}

CBaseGridView::~CBaseGridView()
{
}

BEGIN_MESSAGE_MAP(CBaseGridView, CConfiguratorView)          
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CBaseGridView 绘图

void CBaseGridView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CBaseGridView 诊断

#ifdef _DEBUG
void CBaseGridView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBaseGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBaseGridView 消息处理程序

int CBaseGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CConfiguratorView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	try
	{
		if( !m_sGrid.Create( lpCreateStruct->lpszClass,
		                 _T(""), 
						 lpCreateStruct->dwExStyle,
						 CRect( lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx, lpCreateStruct->cy),
						 this, 
						 IDC_BASEGRID
						 ))
	    {
			theApp.LogInfo( L"m_sGrid.Create in CBaseGridView is failed!" );
		}
	}
	catch( COleException &e )
	{
		e.ReportError();
		e.Delete();

		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception was throwed in CBaseGridView::OnCreate!");

		return -1;
	}

	return 0;
}

// =======================================================================
// Function name  : ApplyFont
// Description    : Set Font
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:30:00
// =======================================================================
void CBaseGridView::ApplyFont( void )
{ 
	m_sGrid.put_FontName( (LPCTSTR)global::FontName );    
	m_sGrid.put_FontSize( global::FontSize ); 

	// Font Name
	m_sGrid.put_Cell( CMygridn::flexcpFontName, _variant_t( 0L ), _variant_t( 0L ), 
				      _variant_t( m_sGrid.get_Rows()-1 ), _variant_t( m_sGrid.get_Cols()-1 ), _variant_t(global::FontName));

	// Font Size
	m_sGrid.put_Cell( CMygridn::flexcpFontSize, _variant_t( 0L ), _variant_t( 0L ), 
				      _variant_t( m_sGrid.get_Rows()-1 ), _variant_t(m_sGrid.get_Cols()-1), _variant_t(global::FontSize));
}

// =======================================================================
// Function name  : GetRowSetting
// Description    : 
// Return type    : CSettingItem*
// Parameters     : lRow
// Author         : WangDong 2011.01.06 15:25:00
// =======================================================================
CSettingItem* CBaseGridView::GetRowSetting( long lRow )
{
	CSettingItem* pSetting = NULL;

	VARIANT v = m_sGrid.get_RowData( lRow );
	if( V_VT(&v) != VT_EMPTY )
	{
        pSetting = reinterpret_cast<CSettingItem*>( V_I4(&v));
		if( !CheckWritePtr( pSetting, sizeof(CSettingItem)))
		{
            pSetting = NULL;
		}
	}

	return pSetting;
}

// =======================================================================
// Function name  : GotoSetting
// Description    : Attempts to find the specified setting item in the grid.
// Return type    : Returns S_OK if the setting is found, otherwise S_FALSE.
// Parameters     : pSetting: The specified setting item.
// Author         :  rjvs 2003-03-17 11:55:33 -06:00
// =======================================================================
HRESULT CBaseGridView::GotoSetting(const CSettingItem* pSetting)
{
	 long lRowIndex = -1;
	 if( m_sGrid.GetSafeHwnd())
	 {
		 for( long lRow = 1; lRow < m_sGrid.get_Rows(); lRow++ )
		 {
              CSettingItem* pSettingItem = GetRowSetting( lRow );
			  if( pSettingItem == pSetting )
			  {
                  lRowIndex = lRow;
				  break;
			  }
		 }
	 }

    if( lRowIndex >= 0 )
    {
		m_sGrid.put_Row( lRowIndex );
		m_sGrid.ShowCell( lRowIndex, 0 );

		long lTopRow = m_sGrid.get_TopRow();
		long lBottomRow = m_sGrid.get_BottomRow();

		if((lRowIndex - lTopRow) > ((lBottomRow - lTopRow) / 2))
        {
            m_sGrid.put_TopRow( lRowIndex );
        }

        return S_OK;
	}

	return S_FALSE;

	/*
    OLECHECK( m_spGrid->get_FindRow(
        TVariant((long) pSetting),   // Item
        TNoParam(), // Row
        TNoParam(), // Col
        TNoParam(), // CaseSensitive
        TNoParam(), // FullMatch
        &lRowIndex  // Result
    ) );*/
}

// =======================================================================
// Function name  : GetSelectionSettings
// Description    : 
// Return type    :
// Parameters     : 
// Author         : rjvs 2003-03-17 11:23:28 -06:00
// =======================================================================
CAliasMap* CBaseGridView::GetSelectionSettings()
{
	if( !m_sGrid.GetSafeHwnd())
	{
       return NULL; 
	}

	std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
    long r1, r2, c1, c2;
	m_sGrid.GetSelection( &r1, &c1, &r2, &c2 );

    for( int i = r1; i <= r2; ++i )
    {
        CSettingItem *pSetting = GetRowSetting(i);
        if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
        {
            CAliasItem* pAlias = pSetting->GetAlias();
            if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
            {
				if( pAlias->GetProtectType() != CAliasItem::PROTECT_CALIBRATE )
				{
                    pAliases->Add(pAlias);
				}
            }
        }
    }

   // Release the auto_ptr
   return pAliases.release();
}

// =======================================================================
// Function name  : SetStringSettings
// Description    : m_avRemote: int --> string
//                  MAC:     40132 - 40137,  Display: 40201    // DSC100, DSC110
//                  IP:      40120 - 40123,  Display: 40141    // TS312
//                  NetMask: 40124 - 40127,  Display: 40161    // TS312
//                  GateWay: 40128 - 40131,  Display: 40181    // TS312
// Return type    :
// Parameters     : nBegin:  Start alias address         
//                  nEnd：   End   alias address
//                  nString：String address
//                  nUpdateType: true: m_avRemote ==> m_avLocal
// Author         : wangdong 2011-04-19 18:50:00
// =======================================================================
void CBaseGridView::SetStringSettings( int nBegin, int nEnd, int nString, bool bLocal )
{
    CAliasItem* pAlias;
    UINTVector  SettingsVector;

    /// 1. Get corresponding int value
    for( int i=nBegin; i<=nEnd; i++ )
    {
        pAlias = GetDocument()->m_aAliases->find_num(i);                 
        if( CheckReadPtr(pAlias, sizeof(CAliasItem)) )          
        {
			if( bLocal )
			{
                SettingsVector.push_back( pAlias->valUI2());  
			}
			else
			{
				SettingsVector.push_back( pAlias->remUI2()); 
			}
        }
    }
    
    /// 2. Combined int values into a string
    CString strString;
    int nValue = 0;
    UINTVector::iterator it = SettingsVector.begin();
    UINTVector::iterator eit = SettingsVector.end();
    while( it != eit )
    {
        UINTVector::iterator nextit = it + 1;
        if( nextit != eit )
        {
            if( nString == 40201 )                   // MAC: "-"; Hex:16
            {
                nValue = *it;
				
                if( nValue < 16 )
                {
					strString.AppendFormat( L"0%x-", nValue ); 
                }
                else
                {
					strString.AppendFormat( L"%x-", nValue ); 
                }
            }
            else                                     // IP, NetMask, GateWay: ".";  Hex:10
            {
				strString.AppendFormat( L"%d.", nValue );
            }
        }
        else       // Last valid int value, not include "-" or "."
        {
            if( nString == 40201 )                   // MAC
            {
                nValue = *it;
                if( nValue < 16 )
                {
					strString.AppendFormat( L"0%x", nValue ); 
                }
                else
                {
					strString.AppendFormat( L"%x", nValue ); 
                }
            }
            else
            {
                strString.AppendFormat( L"%d", nValue );
            }
        }
        it++;
    }

    /// 3. Set string settings
    pAlias = GetDocument()->m_aAliases->find_num( nString );
    if( CheckWritePtr(pAlias, sizeof(CAliasItem)) )          
    {
		if( bLocal )
		{
            pAlias->valString( CW2A(strString));           // m_avLocal, m_bDirtyRemote
		}
		else
		{
			pAlias->remString( CW2A(strString));           // m_avRemote, m_bDirtyRemote
		}
    }

    SettingsVector.clear();
}

// =======================================================================
// Function name  : SplitStringSettings
// Description    : m_avLocal: string -> int
//                  MAC:     40132 - 40137,  Display: 40201    // DSC100, DSC110
//                  IP:      40120 - 40123,  Display: 40141    // TS312
//                  NetMask: 40124 - 40127,  Display: 40161    // TS312
//                  GateWay: 40128 - 40131,  Display: 40181    // TS312
// Return type    :
// Parameters     : nBegin:  Start alias address         
//                  nEnd：   End   alias address
//                  nString：String address
// Author         : wangdong 2011-04-19 18:40:00
// =======================================================================
void CBaseGridView::SplitStringSettings( int nBegin, int nEnd, int nString )
{
    CAliasItem* pAlias = NULL;
    CString  strString;
    CString  strSub;
    int nTagCount = 0, nValidCount = 0;
    int nIndex = -1;
    int nValue = 0;

    /// 1. Get Setting String
    pAlias = GetDocument()->m_aAliases->find_num( nString );
    if( CheckWritePtr(pAlias, sizeof(CAliasItem)) )          
    {
        strString = pAlias->valStr();
		if( strString.IsEmpty())
        {
            return;
        } 
    }

    /// 2. Split setting string, and transfer it to int, assign to corresponding alias
	WCHAR wchar = '.';
	CString strFormat( L"%d" );                   // Hex: 10
	if( nString == 40201 )                        // MAC: XX-XX-XX-XX-XX-XX
	{
		wchar = '-';
		strFormat = L"%x";                        // Hex: 16

		nValidCount = 5;                          // "-" Count: 5
	}
	else
	{
		nValidCount = 3;                          // "." Count: 3
	}

	do
	{
		nIndex = strString.Find( wchar );       // Zero-based
		if( nIndex == -1 )                    
		{
			if( nTagCount != nValidCount )
			{
				break;
			}

			// Last 
			pAlias = GetDocument()->m_aAliases->find_num( nBegin + nTagCount );
			if( pAlias )
			{
				::swscanf_s( strString, strFormat, &nValue );         
				pAlias->valUI2( nValue );
			}

			break;
		}
			
		strSub = strString.Left( nIndex );
		strString = strString.Right( strString.GetLength() - (nIndex+1));
		pAlias = GetDocument()->m_aAliases->find_num( nBegin + nTagCount );
		if( pAlias )
		{
			::swscanf_s( strSub, strFormat, &nValue );         
			pAlias->valUI2( nValue );
		}

		nTagCount++;
	}while( nTagCount <= nValidCount );


	/*
	for( int i = 1; i<=sSettings.Length(); i++ )
    {
        sSub = sSettings.SubString( i, 1);
        if( nString == 40201 )
        {
            nValidCount = 5;                          // "-"的数量应该为5
            if( sSub.AnsiCompareIC("-") == 0 )        // MAC: XX-XX-XX-XX-XX-XX
            {
                nTagCount++;
                nCount = 2;
                nIndex = (nTagCount-1)*3+1;
                
                sSub = sSettings.SubString( nIndex, nCount);
                pAlias = GetDocument()->m_aAliases->find_num( nBegin + nTagCount -1 );
                ::sscanf( sSub.c_str(), "%x", &nValue );         // %x: 11->17， 既认为字符串代表的是16进制数；
                pAlias->valUI2( nValue );

                // 完成上述操作后，还要设置最后一个相关的整型数
                if( nTagCount == 5)
                {
                    sSub = sSettings.SubString( i+1, nCount);
                    nValue = 0;
                    ::sscanf( sSub.c_str(), "%x", &nValue );       // %x: 11->17;   %d: 1a->1
                    pAlias = GetDocument()->m_aAliases->find_num( nBegin + nTagCount );
                    pAlias->valUI2( nValue);
                }
            }
        }    //  if( nString == 40201 )
        else
        {
            nValidCount = 3;                          // "."的数量应该为3
            if( sSub.AnsiCompareIC(".") == 0 )
            {
                nTagCount++;

                if( nTagCount == 1 )
                {
                    nIndex = 1;                // AnsiString: 序号从1开始 
                }
                else
                {
                    nIndex = nPrev + 1;
                }

                nCount = i - nIndex;
                nPrev = i;
                sSub = sSettings.SubString( nIndex, nCount);
                pAlias = GetDocument()->m_aAliases->find_num( nBegin + nTagCount -1 );
                pAlias->valUI2( sSub.ToInt());

                if( nTagCount == 3 )
                {
                    sSub = sSettings.SubString( i+1, sSettings.Length() - i );
                    pAlias = GetDocument()->m_aAliases->find_num( nBegin + nTagCount );
                    pAlias->valUI2( sSub.ToInt());
                }
            }
        }  // else
    }  //for*/
}
//------------------------------------------------------------------------
