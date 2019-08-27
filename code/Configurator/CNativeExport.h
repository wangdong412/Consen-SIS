// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CNativeExport 包装类

class CNativeExport : public COleDispatchDriver
{
public:
	CNativeExport(){} // 调用 COleDispatchDriver 默认构造函数
	CNativeExport(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CNativeExport(const CNativeExport& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// INativeExport 方法
public:
	void SaveToFile(LPCTSTR FileName, BOOL IncludeData)
	{
		static BYTE parms[] = VTS_BSTR VTS_BOOL ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, IncludeData);
	}
	VARIANT SaveToStream(BOOL IncludeData)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, IncludeData);
		return result;
	}
	VARIANT SaveToBase64Stream(BOOL IncludeData)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, IncludeData);
		return result;
	}

	// INativeExport 属性
public:

};
