// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CPyramidSeries 包装类

class CPyramidSeries : public COleDispatchDriver
{
public:
	CPyramidSeries(){} // 调用 COleDispatchDriver 默认构造函数
	CPyramidSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPyramidSeries(const CPyramidSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IPyramidSeries 方法
public:
	long get_SizePercent()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SizePercent(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IPyramidSeries 属性
public:

};
