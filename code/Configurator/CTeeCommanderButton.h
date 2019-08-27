// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "D:\\Registered Active X\\TeeChart8.ocx" no_namespace
// CTeeCommanderButton 包装类

class CTeeCommanderButton : public COleDispatchDriver
{
public:
	CTeeCommanderButton(){} // 调用 COleDispatchDriver 默认构造函数
	CTeeCommanderButton(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeeCommanderButton(const CTeeCommanderButton& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ITeeCommanderButton 方法
public:
	void Hide()
	{
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// ITeeCommanderButton 属性
public:

};
