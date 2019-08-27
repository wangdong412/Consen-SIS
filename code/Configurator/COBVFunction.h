// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// COBVFunction 包装类

class COBVFunction : public COleDispatchDriver
{
public:
	COBVFunction(){} // 调用 COleDispatchDriver 默认构造函数
	COBVFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COBVFunction(const COBVFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IOBVFunction 方法
public:
	VARIANT get_Volume()
	{
		VARIANT result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Volume(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}

	// IOBVFunction 属性
public:

};
