// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CADXFunction 包装类

class CADXFunction : public COleDispatchDriver
{
public:
	CADXFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CADXFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CADXFunction(const CADXFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IADXFunction 方法
public:
	LPDISPATCH get_DMDown()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DMUp()
	{
		LPDISPATCH result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IADXFunction 属性
public:

};
