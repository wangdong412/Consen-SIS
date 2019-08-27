// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CVector3DSeries 包装类

class CVector3DSeries : public COleDispatchDriver
{
public:
	CVector3DSeries(){} // 调用 COleDispatchDriver 默认构造函数
	CVector3DSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVector3DSeries(const CVector3DSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVector3DSeries 方法
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
	long AddPalette(double Value, unsigned long Color)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_UI4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Value, Color);
		return result;
	}
	unsigned long get_StartColor()
	{
		unsigned long result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_StartColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_EndColor()
	{
		unsigned long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_EndColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_PaletteSteps()
	{
		long result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PaletteSteps(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_UsePalette()
	{
		BOOL result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UsePalette(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_UseColorRange()
	{
		BOOL result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UseColorRange(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void ClearPalette()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CreateDefaultPalette(long NumSteps)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NumSteps);
	}
	unsigned long GetSurfacePaletteColor(double Y)
	{
		unsigned long result;
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_UI4, (void*)&result, parms, Y);
		return result;
	}
	unsigned long get_MidColor()
	{
		unsigned long result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_MidColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void CreateRangePalette()
	{
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_PaletteStyle()
	{
		long result;
		InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PaletteStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_UsePaletteMin()
	{
		BOOL result;
		InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UsePaletteMin(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x12d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_PaletteMin()
	{
		double result;
		InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_PaletteMin(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x12e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_PaletteStep()
	{
		double result;
		InvokeHelper(0x12f, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_PaletteStep(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x12f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void InvertPalette()
	{
		InvokeHelper(0x130, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddCustomPalette(VARIANT& colorArray)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x131, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &colorArray);
	}
	long AddVector(double X0, double Y0, double Z0, double X1, double Y1, double Z1, LPCTSTR ALabel, unsigned long AColor)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_BSTR VTS_UI4 ;
		InvokeHelper(0x191, DISPATCH_METHOD, VT_I4, (void*)&result, parms, X0, Y0, Z0, X1, Y1, Z1, ALabel, AColor);
		return result;
	}
	long get_ArrowHeight()
	{
		long result;
		InvokeHelper(0x198, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ArrowHeight(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x198, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ArrowWidth()
	{
		long result;
		InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ArrowWidth(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x199, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_EndArrow()
	{
		LPDISPATCH result;
		InvokeHelper(0x19a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_EndXValues()
	{
		LPDISPATCH result;
		InvokeHelper(0x19b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_EndYValues()
	{
		LPDISPATCH result;
		InvokeHelper(0x19c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_EndZValues()
	{
		LPDISPATCH result;
		InvokeHelper(0x19d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	double MinXValueVector()
	{
		double result;
		InvokeHelper(0x192, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double MaxXValueVector()
	{
		double result;
		InvokeHelper(0x193, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double MinYValueVector()
	{
		double result;
		InvokeHelper(0x194, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double MaxYValueVector()
	{
		double result;
		InvokeHelper(0x195, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double MinZValueVector()
	{
		double result;
		InvokeHelper(0x196, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double MaxZValueVector()
	{
		double result;
		InvokeHelper(0x197, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_StartArrow()
	{
		LPDISPATCH result;
		InvokeHelper(0x19e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IVector3DSeries 属性
public:

};
