// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CSubsetTeeFunction 包装类

class CSubsetTeeFunction : public COleDispatchDriver
{
public:
	CSubsetTeeFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CSubsetTeeFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSubsetTeeFunction(const CSubsetTeeFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ISubsetTeeFunction 方法
public:
	double get_StartValue()
	{
		double result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_StartValue(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_EndValue()
	{
		double result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_EndValue(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// ISubsetTeeFunction 属性
public:

};
