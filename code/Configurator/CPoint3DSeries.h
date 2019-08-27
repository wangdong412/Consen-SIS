// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CPoint3DSeries 包装类

class CPoint3DSeries : public COleDispatchDriver
{
public:
	CPoint3DSeries(){} // 调用 COleDispatchDriver 默认构造函数
	CPoint3DSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPoint3DSeries(const CPoint3DSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IPoint3DSeries 方法
public:
	long AddXYZ(double AX, double AY, double AZ, LPCTSTR AXLabel, unsigned long Value)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_BSTR VTS_UI4 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, AX, AY, AZ, AXLabel, Value);
		return result;
	}
	double MaxZValue()
	{
		double result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double MinZValue()
	{
		double result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	long get_TimesZOrder()
	{
		long result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TimesZOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_ZValues()
	{
		LPDISPATCH result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	double get_ZValue(long Index)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Index);
		return result;
	}
	void put_ZValue(long Index, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Index, newValue);
	}
	LPDISPATCH get_Brush()
	{
		LPDISPATCH result;
		InvokeHelper(0x83, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Pen()
	{
		LPDISPATCH result;
		InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void AddArrayXYZ(VARIANT& XArray, VARIANT& YArray, VARIANT& ZArray)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x87, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &XArray, &YArray, &ZArray);
	}
	void AddArrayGrid(long NumX, long NumZ, VARIANT& XZArray)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NumX, NumZ, &XZArray);
	}
	long CalcZPos(long ValueIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ValueIndex);
		return result;
	}
	double get_DepthSize()
	{
		double result;
		InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_DepthSize(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Pointer()
	{
		LPDISPATCH result;
		InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_LinePen()
	{
		LPDISPATCH result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_BaseLine()
	{
		LPDISPATCH result;
		InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IPoint3DSeries 属性
public:

};
