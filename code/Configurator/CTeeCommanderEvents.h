// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "D:\\Registered Active X\\TeeChart8.ocx" no_namespace
// CTeeCommanderEvents 包装类

class CTeeCommanderEvents : public COleDispatchDriver
{
public:
	CTeeCommanderEvents(){} // 调用 COleDispatchDriver 默认构造函数
	CTeeCommanderEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeeCommanderEvents(const CTeeCommanderEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ITeeCommanderEvents 方法
public:
	void OnEditedChart()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// ITeeCommanderEvents 属性
public:

};
