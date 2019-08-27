// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CCustomToolSeries 包装类

class CCustomToolSeries : public COleDispatchDriver
{
public:
	CCustomToolSeries(){} // 调用 COleDispatchDriver 默认构造函数
	CCustomToolSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCustomToolSeries(const CCustomToolSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICustomToolSeries 方法
public:
	VARIANT get_Series()
	{
		VARIANT result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Series(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}

	// ICustomToolSeries 属性
public:

};
