// MySplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"            // Added by wangdong

#include "MySplitterWnd.h"


// CMySplitterWnd

CMySplitterWnd::CMySplitterWnd(void)
	           :m_cx( -1 ),
			    m_cy( -1 ),
				m_cxSplitterBorder( -1 ),
				m_cySplitterBorder( -1 )
{
}


CMySplitterWnd::~CMySplitterWnd(void)
{
}

// 隐藏分割条
// m_cxSplitterGap，m_cySplitterGap：分割条的宽度
// m_cxBorder，m_cyBorder：分割条的边框
void CMySplitterWnd::HideSplitter()
{
	// Save previous size
	m_cx = m_cxSplitterGap;   
	m_cy = m_cySplitterGap;   

	m_cxSplitterBorder = m_cxBorder;
	m_cySplitterBorder = m_cyBorder;

	m_cxSplitterGap = 1;      // Must > 0
	m_cySplitterGap = 1;

	m_cxBorder = 0;
	m_cyBorder = 0;
}

// 显示分割条
void CMySplitterWnd::ShowSplitter()
{
    m_cxSplitterGap = m_cx;             
	m_cySplitterGap = m_cy;

    m_cxBorder = m_cxSplitterBorder;
	m_cyBorder = m_cySplitterBorder;
}

BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/// 鼠标移动到任何一个分割条时，均能触发WM_MOUSEMOVE消息
void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 注释掉此行，分割条被固定，不能移动；
	// 不注释，则分割条能被移动
	//CSplitterWnd::OnMouseMove(nFlags, point);

	// 限制切分条的运动范围
	if( AfxGetMainWnd() )
	{
		CRect rect;
		AfxGetMainWnd()->GetClientRect( &rect );       // 获取客户区尺寸

		int nColCount = this->GetColumnCount();
	    if((nColCount != 1) && (nColCount != 2))
	    {
		    return;
	    }

	    if( 2 == nColCount )      // 两列，垂直分割条
	    {
			if((point.x < 200) || (point.x > rect.Width()/2))
            {
                CWnd::OnMouseMove(nFlags, point);    // 限定移动范围，第一列宽度：200 ～ 主框架客户区宽度/2
            }
            else
            {  
                CSplitterWnd::OnMouseMove(nFlags, point);
            }
	    }
	    else //if( 1 == nColCount )  // 1列，水平分割条
	    {
			if((point.y < rect.Height()/2) || (point.y > rect.Height() - 100))
		    {
			    CWnd::OnMouseMove(nFlags, point);    // 限定移动范围，第一行：主框架客户区高度/2 ～ 主框架客户区高度-200
		    }
		    else
		    {
                CSplitterWnd::OnMouseMove(nFlags, point);
		    }
	    }
	} //if( AfxGetMainWnd() )	
}
