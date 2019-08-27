// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "D:\\Registered Active X\\TeeChart8.ocx" no_namespace
// CTeeListBoxEvents 包装类

class CTeeListBoxEvents : public COleDispatchDriver
{
public:
	CTeeListBoxEvents(){} // 调用 COleDispatchDriver 默认构造函数
	CTeeListBoxEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeeListBoxEvents(const CTeeListBoxEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ITeeListBoxEvents 方法
public:
	void OnDblClickSeries(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	STDMETHOD(OnClick)()
	{
		HRESULT result;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnChangeActive(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	void OnMouseUp(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xcb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnMouseDown(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnMouseMove(long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xcd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Shift, X, Y);
	}
	void OnChangeColor(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xce, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	void OnChangeOrder(long SeriesIndex1, long SeriesIndex2)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xcf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex1, SeriesIndex2);
	}
	STDMETHOD(OnOtherItemsChange)()
	{
		HRESULT result;
		InvokeHelper(0xd0, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnRefresh)()
	{
		HRESULT result;
		InvokeHelper(0xd1, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnRemovedSeries(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}

	// ITeeListBoxEvents 属性
public:

};
