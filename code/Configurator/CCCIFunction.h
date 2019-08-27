// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CCCIFunction 包装类

class CCCIFunction : public COleDispatchDriver
{
public:
	CCCIFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CCCIFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCCIFunction(const CCCIFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICCIFunction 方法
public:
	double get_Constant()
	{
		double result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Constant(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// ICCIFunction 属性
public:

};
