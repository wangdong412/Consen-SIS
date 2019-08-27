// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CChartGridNavigator 包装类

class CChartGridNavigator : public COleDispatchDriver
{
public:
	CChartGridNavigator(){} // 调用 COleDispatchDriver 默认构造函数
	CChartGridNavigator(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartGridNavigator(const CChartGridNavigator& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IChartGridNavigator 方法
public:
	void put_Grid(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_GridLink(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void EnableButtons()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	unsigned long get_Color()
	{
		unsigned long result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_Color(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IChartGridNavigator 属性
public:

};
