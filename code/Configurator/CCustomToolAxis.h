// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CCustomToolAxis 包装类

class CCustomToolAxis : public COleDispatchDriver
{
public:
	CCustomToolAxis(){} // 调用 COleDispatchDriver 默认构造函数
	CCustomToolAxis(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCustomToolAxis(const CCustomToolAxis& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICustomToolAxis 方法
public:
	VARIANT get_Axis()
	{
		VARIANT result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Axis(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}

	// ICustomToolAxis 属性
public:

};
