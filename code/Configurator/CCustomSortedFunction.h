// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CCustomSortedFunction 包装类

class CCustomSortedFunction : public COleDispatchDriver
{
public:
	CCustomSortedFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CCustomSortedFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCustomSortedFunction(const CCustomSortedFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICustomSortedFunction 方法
public:
	BOOL get_IncludeNulls()
	{
		BOOL result;
		InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IncludeNulls(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x12d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// ICustomSortedFunction 属性
public:

};
