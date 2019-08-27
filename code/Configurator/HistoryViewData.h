#pragma once

#include "HistoryFileData.h"
#include <vector>

// 待显示的历史视图数据类：查看历史趋势时，由用户指定 或者 在右视图动态更新；
// 
// 用途：记录用户指定 或者 动态更新的结果；
class CHistoryViewData
{
public:
    CHistoryViewData(COLORREF nColor=RGB(0,0,0), UINT nWidth=2, int nIndex=-1, CHistoryFileData *pFileData=NULL)
		: m_nColor( nColor )
		, m_nWidth( nWidth )
		, m_nSeriesIndex( nIndex )
		, m_pFileData( pFileData )
	{
	}

private:
    COLORREF m_nColor;   // 序列曲线颜色，默认黑色
    UINT m_nWidth;       // 曲线宽度，默认2，单位像素
    int m_nSeriesIndex;  // 笔/序列的序号

	CHistoryFileData *m_pFileData;

public:
    // 获取参数标签名
	CString GetTagName() const
	{
		if( m_pFileData != NULL )
		{
			return m_pFileData->m_strTagName;
		}

		return L"";
	}

    // 获取参数描述信息
    CString GetDesc() const
	{
		if( m_pFileData != NULL )
		{
			return m_pFileData->m_strDesc;
		}

		return L"";
	}

    // 获取参数Modbus别名
	int GetAlias() const
	{
		if( m_pFileData != NULL )
		{
			return m_pFileData->m_nAlias;
		}

		return -1;
	}

	CString GetAlias_s() const
	{
		CString str( "-1" );
		if( m_pFileData != NULL )
		{
			str.Format( L"%d", m_pFileData->m_nAlias );
		}

		return str;
	}
   
	COLORREF GetColor()       const { return m_nColor; }              // 获取曲线颜色
	UINT GetWidth()           const { return m_nWidth; }              // 获取曲线宽度
	int GetSeriesIndex()      const { return m_nSeriesIndex; }        // 获取序列号
	CHistoryFileData* GetFileData() { return m_pFileData; }
	const CHistoryFileData* GetFileData() const { return m_pFileData; }

	void SetColor(COLORREF nColor)   { m_nColor = nColor; }       // 设置曲线颜色
	void SetWidth(UINT nWidth)       { m_nWidth = nWidth; }       // 设置曲线线宽
	void SetSeriesIndex(int nIndex)  { m_nSeriesIndex = nIndex; } // 设置序列号/笔序号
	void SetFileData( CHistoryFileData* data )
	{
		if( m_pFileData != data )
		{
			m_pFileData = data;
		}
	}

	void Reset( COLORREF color )
	{
		m_nColor = color;      // 序列曲线颜色，默认黑色
		m_nWidth = 2;          // 曲线宽度，默认2，单位像素
		m_nSeriesIndex = -1;   // 笔/序列的序号
		m_pFileData = NULL;
	}
};


// 待显示的历史视图数据集合类
//
// 用途：供History左/右视图显示；
// 使用说明：
// 1.Doc中集合（vector）初始化8个元素，均为NULL；
// 2.菜单-查看 或者 在历史右视图动态设置时，根据实际情况更新；
// 3.新建/打开文件时：不清空；
// 4.切换应用程序类型、产品类型时：仅释放资源，不清空容器；
// 5.关闭应用程序时，释放资源并清空.
class CHistoryViewDatas
{
public:
	CHistoryViewDatas();
	~CHistoryViewDatas();

private:
	CHistoryViewDatas(const CHistoryViewDatas&);
	CHistoryViewDatas& operator=(const CHistoryViewDatas&);

private:
	std::vector<CHistoryViewData*> m_vHisViewDatas;

public:
	size_t Size() const    { return m_vHisViewDatas.size(); }
	void Clear();
	const CHistoryViewData* GetHisViewData( const size_t index ) const;
	CHistoryViewData* GetHisViewData( const size_t index );
	void SetHisViewData( const size_t index, CHistoryViewData* data );

	void Reset();
};
