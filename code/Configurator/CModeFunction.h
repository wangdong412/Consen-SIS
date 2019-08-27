// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CModeFunction 包装类

class CModeFunction : public COleDispatchDriver
{
public:
	CModeFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CModeFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CModeFunction(const CModeFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IModeFunction 方法
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

	// IModeFunction 属性
public:

};
