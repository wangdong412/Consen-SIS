// SelectTNDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "SelectTNDlg.h"
#include "afxdialogex.h"

#include "HistoryFileData.h"


// CSelectTNDlg 对话框

IMPLEMENT_DYNAMIC(CSelectTNDlg, CDialogEx)

CSelectTNDlg::CSelectTNDlg( CHistoryFileDatas* pHisFileDatas, CString strTagname, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectTNDlg::IDD, pParent)
	, m_pHisFileDatas( pHisFileDatas )
	, m_strSelTagname( strTagname )
{
	ASSERT( m_pHisFileDatas != NULL );
}

CSelectTNDlg::~CSelectTNDlg()
{
}

void CSelectTNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBO_TNF, m_cboFilter);
	DDX_Control(pDX, IDC_LIST_TAGNAME, m_listTag);
}


BEGIN_MESSAGE_MAP(CSelectTNDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSelectTNDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CBO_TNF, &CSelectTNDlg::OnCbnSelchangeCboTnf)
END_MESSAGE_MAP()


// CSelectTNDlg 消息处理程序

// =======================================================================
// Function name  : OnInitDialog
// Description    :
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.18 16:00:00
// =======================================================================
BOOL CSelectTNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Filter
	m_cboFilter.AddString( L"All" );
	m_cboFilter.AddString( L"Coil" );              // 1 - 9999
	m_cboFilter.AddString( L"Discrete Input" );    // 10001 - 19999
	m_cboFilter.AddString( L"Holding Register" );  // 40001 - 49999
	m_cboFilter.AddString( L"Input Register" );    // 30001 - 39999
	m_cboFilter.SetCurSel( FILTER_ALL );

	CRect rect;
	m_listTag.GetWindowRect( &rect );
	m_listTag.InsertColumn( COL_ALIAS,   L"Alias",   LVCFMT_LEFT, rect.Width()/4, 0 );
	m_listTag.InsertColumn( COL_TAGNAME, L"Tagname", LVCFMT_LEFT, rect.Width()/4, 1 );
	m_listTag.InsertColumn( COL_DESC,    L"Desc",    LVCFMT_LEFT, rect.Width()/2, 2 );

	AddFilterItems( m_cboFilter.GetCurSel());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// ----------------------------------------------------------------------
// Function name  : AddFilterItems
// Description    :
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.19 09:30:00
// ----------------------------------------------------------------------
void CSelectTNDlg::AddFilterItems( int nFilterType )
{
	int nMinAlias = 0, nMaxAlias = 0;
    switch( nFilterType )
	{
		case FILTER_ALL:
			nMinAlias = 1;
			nMaxAlias = 50000;
		break;
		case FILTER_COIL:
			nMinAlias = 1;
			nMaxAlias = 10000;
		break;
		case FILTER_IN:
			nMinAlias = 10001;
			nMaxAlias = 20000;
		break;
		case FILTER_HOLDREG:
			nMinAlias = 40001;
			nMaxAlias = 50000;
		break;
		case FILTER_INREG:
			nMinAlias = 30001;
			nMaxAlias = 40000;
		break;
		default:
           return;
	}

	m_listTag.SetFocus();
	m_listTag.DeleteAllItems();
	int index = 0;
	if( FILTER_ALL == nFilterType )
	{
		m_listTag.InsertItem(  0, L"none" );        // 供清除当前选择的参数
		m_listTag.SetItemData( 0, NULL );
		m_listTag.SetItemText( 0, 1, L"" );
		m_listTag.SetItemText( 0, 2, L"" );

		if( m_strSelTagname.IsEmpty())
		{
			m_listTag.SetItemState( index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		}

		++index;
	}

	const HFileDatas& datas =  m_pHisFileDatas->GetHFileDatas();
	HFileDatas::const_iterator it = datas.begin();
	while( it != datas.end())
	{
		if( it->second != NULL )
		{
			int nAlias = it->first;
			if((nAlias>=nMinAlias) && (nAlias<=nMaxAlias))
			{
				m_listTag.InsertItem( index, it->second->m_strAlias );
				m_listTag.SetItemData( index, (DWORD)it->second );
				m_listTag.SetItemText( index, 1, it->second->m_strTagName );
				m_listTag.SetItemText( index, 2, it->second->m_strDesc );

				if( 0 == m_strSelTagname.Compare( it->second->m_strTagName ))
				{
					//m_listTag.SetItemState( index, LVIS_SELECTED, LVIS_SELECTED );
					//m_listTag.SetItemState( index, LVIS_FOCUSED, LVIS_FOCUSED );
					m_listTag.SetItemState( index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
				}

				++index;
			}
		}

		++it;
	}
}

// =======================================================================
// Function name  : OnCbnSelchangeCboTnf
// Description    :
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.18 16:00:00
// =======================================================================
void CSelectTNDlg::OnCbnSelchangeCboTnf()
{
	AddFilterItems( m_cboFilter.GetCurSel());
}

// =======================================================================
// Function name  : OnBnClickedOk
// Description    :
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.18 16:00:00
// =======================================================================
void CSelectTNDlg::OnBnClickedOk()
{
	UINT uSelectedCount = m_listTag.GetSelectedCount();
	int index = -1;
	if( 1 == uSelectedCount )
	{
		 index = m_listTag.GetNextItem( -1, LVNI_SELECTED );
		 ASSERT( index != -1 );

		 m_strSelTagname = m_listTag.GetItemText( index, COL_TAGNAME );
	}
	else
	{
		m_strSelTagname = L"";
	}

	CDialogEx::OnOK();
}
