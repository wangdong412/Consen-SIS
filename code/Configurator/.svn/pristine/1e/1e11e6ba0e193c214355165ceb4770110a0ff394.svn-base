// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CFilterItems 包装类

class CFilterItems : public COleDispatchDriver
{
public:
	CFilterItems(){} // 调用 COleDispatchDriver 默认构造函数
	CFilterItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFilterItems(const CFilterItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IFilterItems 方法
public:
	long Add(long FilterClass)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FilterClass);
		return result;
	}
	void LoadImage(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	LPDISPATCH get_Item(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	void AssignImage(long ImageHandle)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ImageHandle);
	}

	// IFilterItems 属性
public:

};
