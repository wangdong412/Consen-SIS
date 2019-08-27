// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// COrgItem 包装类

class COrgItem : public COleDispatchDriver
{
public:
	COrgItem(){} // 调用 COleDispatchDriver 默认构造函数
	COrgItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COrgItem(const COrgItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IOrgItem 方法
public:
	long AddChild(LPCTSTR Text)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Text);
		return result;
	}
	long AddBrother(LPCTSTR Text)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Text);
		return result;
	}
	VARIANT get_Series()
	{
		VARIANT result;
		InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_Superior()
	{
		long result;
		InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Superior(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Text()
	{
		CString result;
		InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Text(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xcd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Format()
	{
		LPDISPATCH result;
		InvokeHelper(0xce, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IOrgItem 属性
public:

};
