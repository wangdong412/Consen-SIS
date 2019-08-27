// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CTeePreviewPanelEvents 包装类

class CTeePreviewPanelEvents : public COleDispatchDriver
{
public:
	CTeePreviewPanelEvents(){} // 调用 COleDispatchDriver 默认构造函数
	CTeePreviewPanelEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeePreviewPanelEvents(const CTeePreviewPanelEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ITeePreviewPanelEvents 方法
public:
	STDMETHOD(OnChangeMargins)(BOOL DisableProportional, long Left, long Top, long Right, long Bottom)
	{
		HRESULT result;
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, DisableProportional, Left, Top, Right, Bottom);
		return result;
	}
	STDMETHOD(OnAfterDraw)()
	{
		HRESULT result;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}

	// ITeePreviewPanelEvents 属性
public:

};
