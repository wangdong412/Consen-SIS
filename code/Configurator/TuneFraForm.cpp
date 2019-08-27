// TuneFraForm.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "TuneFraForm.h"


// CTuneFraForm

IMPLEMENT_DYNCREATE(CTuneFraForm, CFormView)

CTuneFraForm::CTuneFraForm()
	: CFormView(CTuneFraForm::IDD)
{

}

CTuneFraForm::~CTuneFraForm()
{
}

void CTuneFraForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTuneFraForm, CFormView)
	ON_EN_CHANGE(IDC_EDIT1, &CTuneFraForm::OnEnChangeEdit1)
	ON_WM_CHAR()
END_MESSAGE_MAP()


// CTuneFraForm 诊断

#ifdef _DEBUG
void CTuneFraForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTuneFraForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTuneFraForm 消息处理程序


void CTuneFraForm::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CTuneFraForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFormView::OnChar(nChar, nRepCnt, nFlags);
}
