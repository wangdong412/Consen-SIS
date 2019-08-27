#pragma once

class CHistoryFileDatas;
class CConfiguratorDoc;
class CHistoryFileLoadCtrl
{
public:
    CHistoryFileLoadCtrl( CConfiguratorDoc* pDoc, CString strViewFileName = L"" );
    ~CHistoryFileLoadCtrl();

private:
    CHistoryFileLoadCtrl( const CHistoryFileLoadCtrl& orig );
    CHistoryFileLoadCtrl& operator=( const CHistoryFileLoadCtrl& rhs );

private:
	CConfiguratorDoc *m_pDoc;
    bool m_bDlgLoad;            // 历史文件数据被真正加载
    CString m_strLoadFileName;  // 被加载的文件名
    CString m_strViewFileName;  // 之前被使用的文件名
	CHistoryFileDatas *m_aHisFileDatas;

public:
    bool Load( const CString strSelName );

    friend class CHistoryTrendDlg;
};
