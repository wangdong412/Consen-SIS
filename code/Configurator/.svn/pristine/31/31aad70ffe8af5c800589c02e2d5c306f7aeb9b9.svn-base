// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CSeriesSourceFile 包装类

class CSeriesSourceFile : public COleDispatchDriver
{
public:
	CSeriesSourceFile(){} // 调用 COleDispatchDriver 默认构造函数
	CSeriesSourceFile(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSeriesSourceFile(const CSeriesSourceFile& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ISeriesSourceFile 方法
public:
	void put_Chart(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_ChartLink(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Open()
	{
		InvokeHelper(0x12d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Close()
	{
		InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_Active()
	{
		BOOL result;
		InvokeHelper(0x12f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Active(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x12f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_Series()
	{
		VARIANT result;
		InvokeHelper(0x130, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Series(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x130, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void Load()
	{
		InvokeHelper(0x191, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void LoadFromFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x192, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void LoadFromURL(LPCTSTR URL)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x193, DISPATCH_METHOD, VT_EMPTY, NULL, parms, URL);
	}
	CString get_FileName()
	{
		CString result;
		InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FileName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x194, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_LoadMode()
	{
		long result;
		InvokeHelper(0x195, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_LoadMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x195, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// ISeriesSourceFile 属性
public:

};
