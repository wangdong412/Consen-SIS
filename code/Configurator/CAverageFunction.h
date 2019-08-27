// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CAverageFunction 包装类

class CAverageFunction : public COleDispatchDriver
{
public:
	CAverageFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CAverageFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAverageFunction(const CAverageFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IAverageFunction 方法
public:
	BOOL get_IncludeNulls()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IncludeNulls(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IAverageFunction 属性
public:

};
