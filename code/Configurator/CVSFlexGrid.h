// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\\WINDOWS\\system32\\vsflex7l.ocx" no_namespace
// CVSFlexGrid 包装类

class CVSFlexGrid : public COleDispatchDriver
{
public:
	CVSFlexGrid(){} // 调用 COleDispatchDriver 默认构造函数
	CVSFlexGrid(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVSFlexGrid(const CVSFlexGrid& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IVSFlexGrid 方法
public:
	void About()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get__ConvInfo()
	{
		long result;
		InvokeHelper(0x29a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put__ConvInfo(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x29a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FontName()
	{
		CString result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FontName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_FontSize()
	{
		float result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_FontSize(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_FontBold()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_FontBold(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_FontItalic()
	{
		BOOL result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_FontItalic(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_FontStrikethru()
	{
		BOOL result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_FontStrikethru(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_FontUnderline()
	{
		BOOL result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_FontUnderline(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Rows()
	{
		long result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Rows(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Cols()
	{
		long result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Cols(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Version()
	{
		short result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Version(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FormatString()
	{
		CString result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FormatString(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FixedRows()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FixedRows(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FixedCols()
	{
		long result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FixedCols(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TopRow()
	{
		long result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TopRow(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_LeftCol()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_LeftCol(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_RowSel()
	{
		long result;
		InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_RowSel(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ColSel()
	{
		long result;
		InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ColSel(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Row()
	{
		long result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Row(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Col()
	{
		long result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Col(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Text()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Text(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackColor()
	{
		unsigned long result;
		InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_ForeColor()
	{
		unsigned long result;
		InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_ForeColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(DISPID_FORECOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackColorFixed()
	{
		unsigned long result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackColorFixed(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_ForeColorFixed()
	{
		unsigned long result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_ForeColorFixed(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackColorSel()
	{
		unsigned long result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackColorSel(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_ForeColorSel()
	{
		unsigned long result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_ForeColorSel(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackColorBkg()
	{
		unsigned long result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackColorBkg(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_WordWrap()
	{
		BOOL result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_WordWrap(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TextStyle()
	{
		long result;
		InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TextStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TextStyleFixed()
	{
		long result;
		InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TextStyleFixed(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ScrollTrack()
	{
		BOOL result;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ScrollTrack(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FocusRect()
	{
		long result;
		InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FocusRect(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HighLight()
	{
		long result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_HighLight(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Redraw()
	{
		long result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Redraw(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ScrollBars()
	{
		long result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ScrollBars(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MouseRow()
	{
		long result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_MouseCol()
	{
		long result;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_CellLeft()
	{
		long result;
		InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_CellTop()
	{
		long result;
		InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_CellWidth()
	{
		long result;
		InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_CellHeight()
	{
		long result;
		InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_RowHeightMin()
	{
		long result;
		InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_RowHeightMin(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FillStyle()
	{
		long result;
		InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FillStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_GridLines()
	{
		long result;
		InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_GridLines(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_GridLinesFixed()
	{
		long result;
		InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_GridLinesFixed(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_GridColor()
	{
		unsigned long result;
		InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_GridColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_GridColorFixed()
	{
		unsigned long result;
		InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_GridColorFixed(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_CellBackColor()
	{
		unsigned long result;
		InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_CellBackColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_CellForeColor()
	{
		unsigned long result;
		InvokeHelper(0x2e, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_CellForeColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CellAlignment()
	{
		long result;
		InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CellAlignment(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CellTextStyle()
	{
		long result;
		InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CellTextStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_CellPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_CellPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x31, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_CellPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x31, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CellPictureAlignment()
	{
		long result;
		InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CellPictureAlignment(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_CellFontName()
	{
		CString result;
		InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_CellFontName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x33, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_CellFontSize()
	{
		float result;
		InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_CellFontSize(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x34, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_CellFontBold()
	{
		BOOL result;
		InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CellFontBold(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x35, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_CellFontItalic()
	{
		BOOL result;
		InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CellFontItalic(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_CellFontWidth()
	{
		float result;
		InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_CellFontWidth(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_CellFontUnderline()
	{
		BOOL result;
		InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CellFontUnderline(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x38, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_CellFontStrikethru()
	{
		BOOL result;
		InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CellFontStrikethru(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x39, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	float get_FontWidth()
	{
		float result;
		InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_FontWidth(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0x3a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Clip()
	{
		CString result;
		InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Clip(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_Sort(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SelectionMode()
	{
		long result;
		InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SelectionMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MergeCells()
	{
		long result;
		InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MergeCells(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Picture()
	{
		LPDISPATCH result;
		InvokeHelper(0xfffffdf5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_PictureType()
	{
		long result;
		InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PictureType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AllowBigSelection()
	{
		BOOL result;
		InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AllowBigSelection(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AllowUserResizing()
	{
		long result;
		InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AllowUserResizing(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MousePointer()
	{
		long result;
		InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MousePointer(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_MouseIcon()
	{
		LPDISPATCH result;
		InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_MouseIcon(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_MouseIcon(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_VirtualData()
	{
		BOOL result;
		InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_VirtualData(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Editable()
	{
		long result;
		InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Editable(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ComboList()
	{
		CString result;
		InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ComboList(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Value()
	{
		double result;
		InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	unsigned long get_FloodColor()
	{
		unsigned long result;
		InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_FloodColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_CellFloodPercent()
	{
		short result;
		InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_CellFloodPercent(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_CellFloodColor()
	{
		unsigned long result;
		InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_CellFloodColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x4c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SubtotalPosition()
	{
		long result;
		InvokeHelper(0x4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_SubtotalPosition(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BorderStyle()
	{
		long result;
		InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BorderStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_Font(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_Font(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Appearance()
	{
		long result;
		InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Appearance(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(DISPID_APPEARANCE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OutlineBar()
	{
		long result;
		InvokeHelper(0x52, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OutlineBar(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x52, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_TreeColor()
	{
		unsigned long result;
		InvokeHelper(0x53, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_TreeColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x53, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_GridLineWidth()
	{
		short result;
		InvokeHelper(0x54, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_GridLineWidth(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x54, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoResize()
	{
		BOOL result;
		InvokeHelper(0x55, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoResize(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x55, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BottomRow()
	{
		long result;
		InvokeHelper(0x56, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_RightCol()
	{
		long result;
		InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL get_ExtendLastCol()
	{
		BOOL result;
		InvokeHelper(0x58, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ExtendLastCol(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x58, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ClientWidth()
	{
		long result;
		InvokeHelper(0x59, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_ClientHeight()
	{
		long result;
		InvokeHelper(0x5a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_EditText()
	{
		CString result;
		InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_EditText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_hWnd()
	{
		long result;
		InvokeHelper(DISPID_HWND, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_AutoSizeMode()
	{
		long result;
		InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AutoSizeMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_RightToLeft()
	{
		BOOL result;
		InvokeHelper(0x5e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_RightToLeft(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_MultiTotals()
	{
		BOOL result;
		InvokeHelper(0x5f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_MultiTotals(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackColorAlternate()
	{
		unsigned long result;
		InvokeHelper(0x60, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackColorAlternate(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OwnerDraw()
	{
		long result;
		InvokeHelper(0x61, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OwnerDraw(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x61, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DataMode()
	{
		long result;
		InvokeHelper(0x62, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DataMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x62, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OLEDragMode()
	{
		long result;
		InvokeHelper(0x63, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OLEDragMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x63, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OLEDropMode()
	{
		long result;
		InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OLEDropMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_TabBehavior()
	{
		long result;
		InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_TabBehavior(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_SheetBorder()
	{
		unsigned long result;
		InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_SheetBorder(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AllowSelection()
	{
		BOOL result;
		InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AllowSelection(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_PicturesOver()
	{
		BOOL result;
		InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PicturesOver(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CellChecked()
	{
		long result;
		InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CellChecked(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MergeCompare()
	{
		long result;
		InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MergeCompare(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Ellipsis()
	{
		long result;
		InvokeHelper(0x6b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Ellipsis(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_OutlineCol()
	{
		long result;
		InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_OutlineCol(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_RowHeightMax()
	{
		long result;
		InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_RowHeightMax(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AutoSearch()
	{
		long result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AutoSearch(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ExplorerBar()
	{
		long result;
		InvokeHelper(0x6f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ExplorerBar(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_EditMask()
	{
		CString result;
		InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_EditMask(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x70, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EditSelStart()
	{
		long result;
		InvokeHelper(0x71, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EditSelStart(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x71, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EditSelLength()
	{
		long result;
		InvokeHelper(0x72, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EditSelLength(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x72, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_EditSelText()
	{
		CString result;
		InvokeHelper(0x73, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_EditSelText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x73, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EditMaxLength()
	{
		long result;
		InvokeHelper(0x74, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EditMaxLength(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x74, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ComboIndex()
	{
		long result;
		InvokeHelper(0x75, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ComboIndex(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x75, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ComboCount()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_ColAlignment(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x77, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	void put_ColAlignment(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x77, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	long get_ColWidth(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x78, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	void put_ColWidth(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x78, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	long get_RowHeight(long Row)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Row);
		return result;
	}
	void put_RowHeight(long Row, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x79, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	BOOL get_MergeRow(long Row)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Row);
		return result;
	}
	void put_MergeRow(long Row, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x7a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	BOOL get_MergeCol(long Col)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Col);
		return result;
	}
	void put_MergeCol(long Col, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	void put_RowPosition(long Row, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	void put_ColPosition(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x7d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	VARIANT get_RowData(long Row)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, Row);
		return result;
	}
	void put_RowData(long Row, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x7e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, &newValue);
	}
	VARIANT get_ColData(long Col)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, Col);
		return result;
	}
	void put_ColData(long Col, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x7f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, &newValue);
	}
	void AddItem(LPCTSTR Item, VARIANT& Row)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x80, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Item, &Row);
	}
	void RemoveItem(VARIANT& Row)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x81, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Row);
	}
	CString get_TextMatrix(long Row, long Col)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x82, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Row, Col);
		return result;
	}
	void put_TextMatrix(long Row, long Col, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x82, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, Col, newValue);
	}
	void Clear(VARIANT& Where, VARIANT& What)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x83, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Where, &What);
	}
	void EditCell()
	{
		InvokeHelper(0x84, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	double get_ValueMatrix(long Row, long Col)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x85, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Row, Col);
		return result;
	}
	BOOL get_IsSubtotal(long Row)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Row);
		return result;
	}
	void put_IsSubtotal(long Row, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	void Subtotal(long Function, VARIANT& GroupOn, VARIANT& TotalOn, VARIANT& Format, VARIANT& BackColor, VARIANT& ForeColor, VARIANT& FontBold, VARIANT& Caption, VARIANT& MatchFrom, VARIANT& TotalOnly)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x87, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Function, &GroupOn, &TotalOn, &Format, &BackColor, &ForeColor, &FontBold, &Caption, &MatchFrom, &TotalOnly);
	}
	void Refresh()
	{
		InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Outline(short Level)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x89, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Level);
	}
	long get_FixedAlignment(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	void put_FixedAlignment(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	BOOL get_RowIsVisible(long Row)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Row);
		return result;
	}
	BOOL get_ColIsVisible(long Col)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Col);
		return result;
	}
	long get_RowPos(long Row)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Row);
		return result;
	}
	long get_ColPos(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	BOOL get_IsSelected(long Row)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Row);
		return result;
	}
	void put_IsSelected(long Row, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x8f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	CString get_TextArray(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x90, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	void put_TextArray(long Index, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x90, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Index, newValue);
	}
	long get_IsCollapsed(long Row)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x91, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Row);
		return result;
	}
	void put_IsCollapsed(long Row, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x91, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	void Select(long Row, long Col, VARIANT& RowSel, VARIANT& ColSel)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x92, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Col, &RowSel, &ColSel);
	}
	void AutoSize(long Col1, VARIANT& Col2, VARIANT& Equal, VARIANT& ExtraSpace)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x93, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Col1, &Col2, &Equal, &ExtraSpace);
	}
	void CellBorder(unsigned long Color, short Left, short Top, short Right, short Bottom, short Vertical, short Horizontal)
	{
		static BYTE parms[] = VTS_UI4 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0x95, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Color, Left, Top, Right, Bottom, Vertical, Horizontal);
	}
	void OLEDrag()
	{
		InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SaveGrid(LPCTSTR FileName, long SaveWhat, VARIANT& FixedCells)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, SaveWhat, &FixedCells);
	}
	void LoadGrid(LPCTSTR FileName, long LoadWhat, VARIANT& FixedCells)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x98, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, LoadWhat, &FixedCells);
	}
	void Archive(LPCTSTR ArcFileName, LPCTSTR FileName, long Action)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0x99, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ArcFileName, FileName, Action);
	}
	VARIANT get_ArchiveInfo(LPCTSTR ArcFileName, long InfoType, VARIANT& Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x9a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, ArcFileName, InfoType, &Index);
		return result;
	}
	long get_ColSort(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	void put_ColSort(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	CString get_ColFormat(long Col)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Col);
		return result;
	}
	void put_ColFormat(long Col, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x9c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	long get_ColDataType(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	void put_ColDataType(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	VARIANT get_Cell(long Setting, VARIANT& Row1, VARIANT& Col1, VARIANT& Row2, VARIANT& Col2)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x9e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, Setting, &Row1, &Col1, &Row2, &Col2);
		return result;
	}
	void put_Cell(long Setting, VARIANT& Row1, VARIANT& Col1, VARIANT& Row2, VARIANT& Col2, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x9e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Setting, &Row1, &Col1, &Row2, &Col2, &newValue);
	}
	void putref_Cell(long Setting, VARIANT& Row1, VARIANT& Col1, VARIANT& Row2, VARIANT& Col2, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x9e, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, Setting, &Row1, &Col1, &Row2, &Col2, &newValue);
	}
	long get_RowStatus(long Row)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Row);
		return result;
	}
	void put_RowStatus(long Row, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	long get_FindRow(VARIANT& Item, VARIANT& Row, VARIANT& Col, VARIANT& CaseSensitive, VARIANT& FullMatch)
	{
		long result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xa0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, &Item, &Row, &Col, &CaseSensitive, &FullMatch);
		return result;
	}
	CString get_ComboItem(VARIANT& Index)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xa1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, &Index);
		return result;
	}
	CString get_ComboData(VARIANT& Index)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xa2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, &Index);
		return result;
	}
	void BindToArray(VARIANT& VariantArray, VARIANT& RowDim, VARIANT& ColDim, VARIANT& PageDim, VARIANT& CurrentPage)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xa3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &VariantArray, &RowDim, &ColDim, &PageDim, &CurrentPage);
	}
	CString get_ColEditMask(long Col)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Col);
		return result;
	}
	void put_ColEditMask(long Col, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0xa4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	CString get_ColComboList(long Col)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Col);
		return result;
	}
	void put_ColComboList(long Col, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0xa5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	short get_RowOutlineLevel(long Row)
	{
		short result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms, Row);
		return result;
	}
	void put_RowOutlineLevel(long Row, short newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0xa6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	long get_SelectedRows()
	{
		long result;
		InvokeHelper(0xa7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_SelectedRow(long Index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Index);
		return result;
	}
	BOOL get_ScrollTips()
	{
		BOOL result;
		InvokeHelper(0xa9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ScrollTips(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xa9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ScrollTipText()
	{
		CString result;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ScrollTipText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_RowHidden(long Row)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xab, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Row);
		return result;
	}
	void put_RowHidden(long Row, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0xab, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Row, newValue);
	}
	BOOL get_ColHidden(long Col)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xac, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Col);
		return result;
	}
	void put_ColHidden(long Col, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0xac, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	long get_ColWidthMin()
	{
		long result;
		InvokeHelper(0xad, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ColWidthMin(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xad, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ColWidthMax()
	{
		long result;
		InvokeHelper(0xae, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ColWidthMax(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xae, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void GetMergedRange(long Row, long Col, long * R1, long * C1, long * R2, long * C2)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0xaf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Col, R1, C1, R2, C2);
	}
	long get_ShowComboButton()
	{
		long result;
		InvokeHelper(0xb0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ShowComboButton(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_CellButtonPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0xb1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_CellButtonPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xb1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_CellButtonPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xb1, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ComboSearch()
	{
		long result;
		InvokeHelper(0xb2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ComboSearch(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void LoadArray(VARIANT& VariantArray, VARIANT& RowDim, VARIANT& ColDim, VARIANT& PageDim, VARIANT& CurrentPage)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xb3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &VariantArray, &RowDim, &ColDim, &PageDim, &CurrentPage);
	}
	BOOL get_AutoSizeMouse()
	{
		BOOL result;
		InvokeHelper(0xb4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoSizeMouse(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xb4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void GetSelection(long * Row1, long * Col1, long * Row2, long * Col2)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0xb5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row1, Col1, Row2, Col2);
	}
	CString get_ClipSeparators()
	{
		CString result;
		InvokeHelper(0xb6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ClipSeparators(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xb6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void PrintGrid(VARIANT& DocName, VARIANT& ShowDialog, VARIANT& Orientation, VARIANT& MarginLR, VARIANT& MarginTB)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xb7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &DocName, &ShowDialog, &Orientation, &MarginLR, &MarginTB);
	}
	long get_ColImageList(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	void put_ColImageList(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xb8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	CString get_ColKey(long Col)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Col);
		return result;
	}
	void put_ColKey(long Col, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0xb9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	long get_ColIndex(LPCTSTR Key)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xba, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Key);
		return result;
	}
	long get_FrozenRows()
	{
		long result;
		InvokeHelper(0xbc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FrozenRows(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xbc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FrozenCols()
	{
		long result;
		InvokeHelper(0xbd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FrozenCols(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xbd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AllowUserFreezing()
	{
		long result;
		InvokeHelper(0xbe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AllowUserFreezing(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xbe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackColorFrozen()
	{
		unsigned long result;
		InvokeHelper(0xbf, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackColorFrozen(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xbf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_ForeColorFrozen()
	{
		unsigned long result;
		InvokeHelper(0xc0, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_ForeColorFrozen(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0xc0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_FlexDataSource()
	{
		LPDISPATCH result;
		InvokeHelper(0xc1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_FlexDataSource(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xc1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long GetNodeRow(long Row, long Which)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xc2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Row, Which);
		return result;
	}
	CString BuildComboList(LPDISPATCH rs, LPCTSTR FieldList, VARIANT& KeyField, VARIANT& BackColor)
	{
		CString result;
		static BYTE parms[] = VTS_DISPATCH VTS_BSTR VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xc3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, rs, FieldList, &KeyField, &BackColor);
		return result;
	}
	LPDISPATCH get_NodeOpenPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0xc4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_NodeOpenPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xc4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_NodeOpenPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xc4, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_NodeClosedPicture()
	{
		LPDISPATCH result;
		InvokeHelper(0xc5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_NodeClosedPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xc5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_NodeClosedPicture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xc5, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	void ShowCell(long Row, long Col)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xc6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Row, Col);
	}
	float get_AutoSearchDelay()
	{
		float result;
		InvokeHelper(0xc7, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
		return result;
	}
	void put_AutoSearchDelay(float newValue)
	{
		static BYTE parms[] = VTS_R4 ;
		InvokeHelper(0xc7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EditWindow()
	{
		long result;
		InvokeHelper(0xc8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_WallPaper()
	{
		LPDISPATCH result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_WallPaper(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_WallPaper(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	long get_WallPaperAlignment()
	{
		long result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_WallPaperAlignment(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_Aggregate(long Aggregate, long Row1, long Col1, long Row2, long Col2)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Aggregate, Row1, Col1, Row2, Col2);
		return result;
	}
	long DragRow(long Row)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Row);
		return result;
	}
	LPDISPATCH GetNode(VARIANT& Row)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xcd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Row);
		return result;
	}
	long get_ColIndent(long Col)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Col);
		return result;
	}
	void put_ColIndent(long Col, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Col, newValue);
	}
	void LoadGridURL(LPCTSTR URL, long LoadWhat, VARIANT& FixedCells)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_VARIANT ;
		InvokeHelper(0xd2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, URL, LoadWhat, &FixedCells);
	}
	void FinishEditing(BOOL Cancel)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xd3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Cancel);
	}

	// IVSFlexGrid 属性
public:

};
