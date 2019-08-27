// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CSeriesMarksPositions 包装类

class CSeriesMarksPositions : public COleDispatchDriver
{
public:
	CSeriesMarksPositions(){} // 调用 COleDispatchDriver 默认构造函数
	CSeriesMarksPositions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSeriesMarksPositions(const CSeriesMarksPositions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ISeriesMarksPositions 方法
public:
	LPDISPATCH get_Position(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	void Automatic(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}

	// ISeriesMarksPositions 属性
public:

};
