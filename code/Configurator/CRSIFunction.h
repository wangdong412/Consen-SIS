// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CRSIFunction 包装类

class CRSIFunction : public COleDispatchDriver
{
public:
	CRSIFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CRSIFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRSIFunction(const CRSIFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IRSIFunction 方法
public:
	long get_Style()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Style(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IRSIFunction 属性
public:

};
