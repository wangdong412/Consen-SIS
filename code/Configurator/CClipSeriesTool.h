// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "D:\\ActiveX\\TeeChart8.oc" no_namespace
// CClipSeriesTool 包装类

class CClipSeriesTool : public COleDispatchDriver
{
public:
	CClipSeriesTool(){} // 调用 COleDispatchDriver 默认构造函数
	CClipSeriesTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CClipSeriesTool(const CClipSeriesTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IClipSeriesTool 方法
public:
	VARIANT get_Series()
	{
		VARIANT result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Series(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}

	// IClipSeriesTool 属性
public:

};
