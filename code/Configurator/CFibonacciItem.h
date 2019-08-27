// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CFibonacciItem 包装类

class CFibonacciItem : public COleDispatchDriver
{
public:
	CFibonacciItem(){} // 调用 COleDispatchDriver 默认构造函数
	CFibonacciItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFibonacciItem(const CFibonacciItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IFibonacciItem 方法
public:
	LPDISPATCH get_Pen()
	{
		LPDISPATCH result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	double get_Value()
	{
		double result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Value(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IFibonacciItem 属性
public:

};
