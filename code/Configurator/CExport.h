// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CExport 包装类

class CExport : public COleDispatchDriver
{
public:
	CExport(){} // 调用 COleDispatchDriver 默认构造函数
	CExport(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CExport(const CExport& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IExport 方法
public:
	void CopyToClipboardBitmap()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CopyToClipboardMetafile(BOOL Enhanced)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enhanced);
	}
	void SaveToBitmapFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveToFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveToJPEGFile(LPCTSTR FileName, BOOL Gray, long Performance, long Quality, long Width, long Height)
	{
		static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, Gray, Performance, Quality, Width, Height);
	}
	void SaveToMetafile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveToMetafileEnh(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void ShowExport()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString SaveChartDialog()
	{
		CString result;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void ConvertTeeFileToText(LPCTSTR InputFile, LPCTSTR OutputFile)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, InputFile, OutputFile);
	}
	VARIANT Stream(long StreamType)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, StreamType);
		return result;
	}
	LPDISPATCH get_asJPEG()
	{
		LPDISPATCH result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asPNG()
	{
		LPDISPATCH result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asGIF()
	{
		LPDISPATCH result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asBMP()
	{
		LPDISPATCH result;
		InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asPCX()
	{
		LPDISPATCH result;
		InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asMetafile()
	{
		LPDISPATCH result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asNative()
	{
		LPDISPATCH result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asXML()
	{
		LPDISPATCH result;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asXLS()
	{
		LPDISPATCH result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asHTML()
	{
		LPDISPATCH result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asText()
	{
		LPDISPATCH result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void EmailFile(LPCTSTR FileName, LPCTSTR Subject)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, Subject);
	}
	LPDISPATCH get_asPDF()
	{
		LPDISPATCH result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asEPS()
	{
		LPDISPATCH result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_asSVG()
	{
		LPDISPATCH result;
		InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_LabelHeader()
	{
		CString result;
		InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_LabelHeader(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xcc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_asXAML()
	{
		LPDISPATCH result;
		InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SaveToXMLFile(LPCTSTR FileName, BOOL IncludeData, BOOL XMLHeader)
	{
		static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL ;
		InvokeHelper(0xce, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, IncludeData, XMLHeader);
	}
	VARIANT SaveToXMLStream(BOOL IncludeData, BOOL XMLHeader)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BOOL VTS_BOOL ;
		InvokeHelper(0xcf, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, IncludeData, XMLHeader);
		return result;
	}

	// IExport 属性
public:

};
