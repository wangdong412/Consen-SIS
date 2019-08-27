// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "D:\\Registered Active X\\TeeChart8.ocx" no_namespace
// CTChartEvents 包装类

class CTChartEvents : public COleDispatchDriver
{
public:
	CTChartEvents(){} // 调用 COleDispatchDriver 默认构造函数
	CTChartEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTChartEvents(const CTChartEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ITChartEvents 方法
public:
	void OnAfterDraw()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnBeforeDrawSeries()
	{
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnBeforeDrawAxes()
	{
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnClick()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnClickAxis(long Axis, long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Axis, Button, Shift, X, Y);
	}
	void OnClickBackground(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnClickLegend(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnClickSeries(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex, Button, Shift, X, Y);
	}
	void OnDblClick()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnDragDrop(long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, X, Y);
	}
	void OnDragOver(long X, long Y, long State, BOOL * Accept)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, X, Y, State, Accept);
	}
	void OnEndDrag(long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, X, Y);
	}
	void OnEnter()
	{
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnExit()
	{
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnGetAxisLabel(long Axis, long SeriesIndex, long ValueIndex, BSTR * LabelText)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBSTR ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Axis, SeriesIndex, ValueIndex, LabelText);
	}
	void OnGetLegendRect(long * Left, long * Top, long * Right, long * Bottom)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top, Right, Bottom);
	}
	void OnGetNextAxisLabel(long Axis, long LabelIndex, double * LabelValue, BOOL * MoreLabels)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 VTS_PBOOL ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Axis, LabelIndex, LabelValue, MoreLabels);
	}
	void OnMouseDown(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnMouseMove(long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Shift, X, Y);
	}
	void OnMouseUp(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnPageChange()
	{
		InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnResize()
	{
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnScroll()
	{
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnStartDrag()
	{
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnUndoZoom()
	{
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnZoom()
	{
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnAllowScroll(long Axis, double * AMin, double * AMax, BOOL * AllowScroll)
	{
		static BYTE parms[] = VTS_I4 VTS_PR8 VTS_PR8 VTS_PBOOL ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Axis, AMin, AMax, AllowScroll);
	}
	void OnGetLegendPos(long ItemIndex, long * X, long * Y, long * XColor)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ItemIndex, X, Y, XColor);
	}
	void OnGetLegendText(long LegendStyle, long ValueIndex, BSTR * LegendText)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LegendStyle, ValueIndex, LegendText);
	}
	void OnDatabaseRecord(long SeriesIndex, long Index, BOOL * StopRetrieve)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, Index, StopRetrieve);
	}
	void OnTimer()
	{
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnGetSeriesMark(long SeriesIndex, long ValueIndex, BSTR * MarkText)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex, MarkText);
	}
	void OnGetSeriesPointerStyle(long SeriesIndex, long ValueIndex, long * AStyle)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex, AStyle);
	}
	void OnGetSeriesContourLevel(long SeriesIndex, long LevelIndex, double * LevelValue, unsigned long * LevelColor)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 VTS_PUI4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, LevelIndex, LevelValue, LevelColor);
	}
	void OnBeforeDrawChart()
	{
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnSeriesBeforeDrawValues(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	void OnSeriesAfterDrawValues(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	void OnMarkTipToolGetText(long Tool, BSTR * Text)
	{
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Tool, Text);
	}
	void OnCursorToolChange(long Tool, long X, long Y, double XVal, double YVal, long Series, long ValueIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_R8 VTS_I4 VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Tool, X, Y, XVal, YVal, Series, ValueIndex);
	}
	void OnDrawLineToolLineDrag()
	{
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnDrawLineToolNewLine()
	{
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnDrawLineToolDraggedLine()
	{
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnDrawLineToolSelectLine()
	{
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnCalendarSeriesChange(long Series, double Value)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, Value);
	}
	void OnSeriesBeforeAdd(long SeriesIndex, BOOL * MoreValues)
	{
		static BYTE parms[] = VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, MoreValues);
	}
	STDMETHOD(OnMouseEnterSeries)(long SeriesIndex)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, SeriesIndex);
		return result;
	}
	STDMETHOD(OnMouseLeaveSeries)(long SeriesIndex)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, SeriesIndex);
		return result;
	}
	void OnSeriesContourBeforeDrawLevel(long SeriesIndex, long LevelIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xcb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, LevelIndex);
	}
	STDMETHOD(OnSeriesAnimationToolStep)(long Tool, long Step)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Tool, Step);
		return result;
	}
	STDMETHOD(OnGanttToolDragBar)(long Tool, long GanttBar)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xcd, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Tool, GanttBar);
		return result;
	}
	STDMETHOD(OnGanttToolResizeBar)(long Tool, long GanttBar, long BarPart)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xce, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, Tool, GanttBar, BarPart);
		return result;
	}
	STDMETHOD(OnFunctionCalculate)(long SeriesIndex, double X, double * Y)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_PR8 ;
		InvokeHelper(0xcf, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, SeriesIndex, X, Y);
		return result;
	}
	STDMETHOD(OnCompressFunctionGetDate)(long FunctionSeriesIndex, long SourceSeriesIndex, long ValueIndex, DATE * D)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PDATE ;
		InvokeHelper(0xd0, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, FunctionSeriesIndex, SourceSeriesIndex, ValueIndex, D);
		return result;
	}
	STDMETHOD(OnGaugeChange)(long SeriesIndex)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd1, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, SeriesIndex);
		return result;
	}
	STDMETHOD(OnNearestToolChange)()
	{
		HRESULT result;
		InvokeHelper(0xd2, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnBeforePrint)()
	{
		HRESULT result;
		InvokeHelper(0xd3, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnKeyDown)(long KeyCode, long Shift)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xd4, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, KeyCode, Shift);
		return result;
	}
	STDMETHOD(OnKeyUp)(long KeyCode, long Shift)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xd5, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, KeyCode, Shift);
		return result;
	}
	STDMETHOD(OnLegendScrollBarToolScrolled)()
	{
		HRESULT result;
		InvokeHelper(0xd6, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnSurfaceNearestToolSelectCell)()
	{
		HRESULT result;
		InvokeHelper(0xd7, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnDrawAxisLabel(long Axis, long X, long Y, BSTR * LabelText)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBSTR ;
		InvokeHelper(0xd8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Axis, X, Y, LabelText);
	}
	void OnLegendDrawSymbol(long Series, long ValueIndex, long Left, long Top, long Right, long Bottom)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xd9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Series, ValueIndex, Left, Top, Right, Bottom);
	}
	void OnColorBandToolClick(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xda, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	STDMETHOD(OnRotateToolRotate)()
	{
		HRESULT result;
		InvokeHelper(0xdb, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnColorLineToolBeginDragLine()
	{
		InvokeHelper(0xdc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnColorLineToolDragLine()
	{
		InvokeHelper(0xdd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnColorLineToolEndDragLine()
	{
		InvokeHelper(0xde, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void OnDragPointToolDragPoint(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xdf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void OnDrawLineToolSelecting(long Index, BOOL AllowSelect)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0xe0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index, AllowSelect);
	}
	STDMETHOD(OnSelectorToolDragged)()
	{
		HRESULT result;
		InvokeHelper(0xe1, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnSelectorToolDragging)()
	{
		HRESULT result;
		InvokeHelper(0xe2, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnSelectorToolResized)()
	{
		HRESULT result;
		InvokeHelper(0xe3, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnSelectorToolResizing)()
	{
		HRESULT result;
		InvokeHelper(0xe4, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnSelectorToolSelected)()
	{
		HRESULT result;
		InvokeHelper(0xe5, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnAnnotationToolClick(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xe6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	STDMETHOD(OnRectangleToolDragging)()
	{
		HRESULT result;
		InvokeHelper(0xe7, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnRectangleToolResizing)()
	{
		HRESULT result;
		InvokeHelper(0xe8, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnRectangleToolClick(long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xe9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Button, Shift, X, Y);
	}
	void OnSeriesAfterAdd(long SeriesIndex, long ValueIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xea, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex);
	}
	void OnGetSeriesBarStyle(long SeriesIndex, long ValueIndex, long * BarStyle)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0xeb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex, BarStyle);
	}
	STDMETHOD(OnVideoToolNewFrame)()
	{
		HRESULT result;
		InvokeHelper(0xec, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnVideoToolStart)()
	{
		HRESULT result;
		InvokeHelper(0xed, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnVideoToolStop)()
	{
		HRESULT result;
		InvokeHelper(0xee, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnVideoPlayerToolFrame)()
	{
		HRESULT result;
		InvokeHelper(0xef, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnOpenGLInit)()
	{
		HRESULT result;
		InvokeHelper(0xf0, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnOpenGLRepaint)()
	{
		HRESULT result;
		InvokeHelper(0xf1, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnCursorToolSnapChange(long Tool, long X, long Y, double XVal, double YVal, long Series, long ValueIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_R8 VTS_I4 VTS_I4 ;
		InvokeHelper(0xf2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Tool, X, Y, XVal, YVal, Series, ValueIndex);
	}
	void OnDragMarksToolDraggedMark(long Index, long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xf3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index, Button, Shift, X, Y);
	}
	void OnDragMarksToolDraggingMark(long Index, long Button, long Shift, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xf4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index, Button, Shift, X, Y);
	}
	STDMETHOD(OnRectangleToolDragged)()
	{
		HRESULT result;
		InvokeHelper(0xf5, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnRectangleToolResized)()
	{
		HRESULT result;
		InvokeHelper(0xf6, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnMarkTipToolCancelHint)()
	{
		HRESULT result;
		InvokeHelper(0xf7, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnSeriesClickPointer(long SeriesIndex, long ValueIndex, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xf8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex, X, Y);
	}
	void OnSeriesClickPointer3D(long SeriesIndex, long ValueIndex, long X, long Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xf9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex, X, Y);
	}
	STDMETHOD(OnFaderToolStop)()
	{
		HRESULT result;
		InvokeHelper(0xfa, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	STDMETHOD(OnAxisArrowToolClick)(long ToolIndex, BOOL AtStart)
	{
		HRESULT result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0xfb, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, ToolIndex, AtStart);
		return result;
	}
	STDMETHOD(OnColorBandToolResized)()
	{
		HRESULT result;
		InvokeHelper(0xfc, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnAddSeries(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xfd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	void OnRemoveSeries(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xfe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	STDMETHOD(OnLegendScrollBarToolChangeSize)()
	{
		HRESULT result;
		InvokeHelper(0xff, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}
	void OnLegendScrollBarToolSetPosition(long Value)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x100, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Value);
	}
	void OnImageSeriesGetImage(long SeriesIndex, long ValueIndex, LPDISPATCH * Picture)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x101, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex, Picture);
	}
	void OnGetSeriesCircleLabel(long SeriesIndex, double Angle, long Index, BSTR * Text)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x102, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, Angle, Index, Text);
	}
	void OnTagCloudSeriesDrawTag(long SeriesIndex, long ValueIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x103, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, ValueIndex);
	}
	void OnSurfaceSeriesGetYValue(long SeriesIndex, long X, long Z, double * Y)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PR8 ;
		InvokeHelper(0x104, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, X, Z, Y);
	}
	void OnDataTableToolFilterSeries(long SeriesIndex, BOOL * Accept)
	{
		static BYTE parms[] = VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x105, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex, Accept);
	}

	// ITChartEvents 属性
public:

};
