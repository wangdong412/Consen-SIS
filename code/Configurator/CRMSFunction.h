// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CRMSFunction 包装类

class CRMSFunction : public COleDispatchDriver
{
public:
	CRMSFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CRMSFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRMSFunction(const CRMSFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IRMSFunction 方法
public:
	BOOL get_Complete()
	{
		BOOL result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Complete(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IRMSFunction 属性
public:

};
