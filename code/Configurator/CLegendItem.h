// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CLegendItem 包装类

class CLegendItem : public COleDispatchDriver
{
public:
	CLegendItem(){} // 调用 COleDispatchDriver 默认构造函数
	CLegendItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CLegendItem(const CLegendItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ILegendItem 方法
public:
	TeeRect get_SymbolRect()
	{
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_EMPTY, NULL, NULL);
	}
	void put_SymbolRect(TeeRect newValue)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Align()
	{
		long result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Left()
	{
		long result;
		InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_Text()
	{
		CString result;
		InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Text(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xcc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Top()
	{
		long result;
		InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// ILegendItem 属性
public:

};
