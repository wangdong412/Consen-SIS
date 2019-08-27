// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CCustomSeriesList 包装类

class CCustomSeriesList : public COleDispatchDriver
{
public:
	CCustomSeriesList(){} // 调用 COleDispatchDriver 默认构造函数
	CCustomSeriesList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCustomSeriesList(const CCustomSeriesList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICustomSeriesList 方法
public:
	void ClearValues()
	{
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long First()
	{
		long result;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long Last()
	{
		long result;
		InvokeHelper(0xcb, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Items(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}

	// ICustomSeriesList 属性
public:

};
