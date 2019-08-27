// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CPerformanceFunction 包装类

class CPerformanceFunction : public COleDispatchDriver
{
public:
	CPerformanceFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CPerformanceFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPerformanceFunction(const CPerformanceFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IPerformanceFunction 方法
public:
	double Calculate(long SourceSeriesIndex, long FirstIndex, long LastIndex)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_R8, (void*)&result, parms, SourceSeriesIndex, FirstIndex, LastIndex);
		return result;
	}

	// IPerformanceFunction 属性
public:

};
