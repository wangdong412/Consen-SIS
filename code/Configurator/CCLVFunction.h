// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CCLVFunction 包装类

class CCLVFunction : public COleDispatchDriver
{
public:
	CCLVFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CCLVFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCLVFunction(const CCLVFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICLVFunction 方法
public:
	BOOL get_Accumulate()
	{
		BOOL result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Accumulate(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_Volume()
	{
		VARIANT result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Volume(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}

	// ICLVFunction 属性
public:

};
