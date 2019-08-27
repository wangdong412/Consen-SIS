#include "StdAfx.h"
#include "SendStatus.h"

#include "SettingItem.h"
#include "AliasItem.h"


CSendStatus::CSendStatus(void)
	: m_nDefChanged( 0 )
	, m_nDefSame( 0 )
	, m_nTuningChanged( 0 )
	, m_nTuningSame( 0 )
	, m_nCfgChanged( 0 )
	, m_nCfgSame( 0 )
	, m_fSpeed( 0.0 )
	, m_fProgress( 0.0 )
{
}


CSendStatus::~CSendStatus(void)
{
}

// =======================================================================
// Function name  : UpdateSendDlgStatus
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong  2011.03.17  16:10:00
// =======================================================================
void CSendStatus::UpdateSendDlgStatus( const CSettingVector* pSettings )
{
	CSettingVector::const_iterator it = pSettings->begin();
	CSettingVector::const_iterator eit = pSettings->end();

	for( ; it != eit; it++ )
	{
		const CSettingItem* pSetting = *it;
		if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
		{
		    const CAliasItem* pAlias = pSetting->GetAlias();
			if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
			{
				bool bDirty = pAlias->IsDirtyRemote() || pAlias->IsDirtyLocal();  
				switch( pAlias->GetProtectType())
				{
				    case CAliasItem::PROTECT_CONFIGURE:
						if( bDirty )
						{
							m_nCfgChanged++;
						}
						else
						{
							m_nCfgSame++;
						}
					break;

					case CAliasItem::PROTECT_TUNE:
						if( bDirty )
						{
							m_nTuningChanged++;
						}
						else
						{
							m_nTuningSame++;
						}
					break;

					default:
						if( bDirty )
						{
							m_nDefChanged++;
						}
						else
						{
							m_nDefSame++;
						}
				}//switch
			}//if pAlias

			if( pSetting->GetSubItemCount() > 0 )
			{
				UpdateSendDlgStatus( pSetting->GetChildVector());
			}
		}//if pSetting
	}//for

	/* return *this;*/
}
//------------------------------------------------------------------------
