// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CExpAvgFunction 包装类

class CExpAvgFunction : public COleDispatchDriver
{
public:
	CExpAvgFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CExpAvgFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CExpAvgFunction(const CExpAvgFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IExpAvgFunction 方法
public:
	double get_Weight()
	{
		double result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_Weight(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IExpAvgFunction 属性
public:

};
