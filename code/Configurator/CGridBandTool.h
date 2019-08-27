// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CGridBandTool 包装类

class CGridBandTool : public COleDispatchDriver
{
public:
	CGridBandTool(){} // 调用 COleDispatchDriver 默认构造函数
	CGridBandTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGridBandTool(const CGridBandTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IGridBandTool 方法
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
	LPDISPATCH get_Band1()
	{
		LPDISPATCH result;
		InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Band2()
	{
		LPDISPATCH result;
		InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	unsigned long get_BandBackColor(long ABand)
	{
		unsigned long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12f, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, parms, ABand);
		return result;
	}

	// IGridBandTool 属性
public:

};
