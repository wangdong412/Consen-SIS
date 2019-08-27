// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CMedianFunction 包装类

class CMedianFunction : public COleDispatchDriver
{
public:
	CMedianFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CMedianFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMedianFunction(const CMedianFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IMedianFunction 方法
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

	// IMedianFunction 属性
public:

};
