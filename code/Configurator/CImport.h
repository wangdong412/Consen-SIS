// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CImport 包装类

class CImport : public COleDispatchDriver
{
public:
	CImport(){} // 调用 COleDispatchDriver 默认构造函数
	CImport(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CImport(const CImport& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IImport 方法
public:
	void LoadFromFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void LoadFromURL(LPCTSTR URL)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, URL);
	}
	void LoadFromStream(VARIANT& AStream)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &AStream);
	}
	void LoadFromBase64Stream(VARIANT& AStream)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &AStream);
	}
	BOOL get_V5TeeCompatible()
	{
		BOOL result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_V5TeeCompatible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IImport 属性
public:

};
