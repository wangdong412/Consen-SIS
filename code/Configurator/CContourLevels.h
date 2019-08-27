// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CContourLevels 包装类

class CContourLevels : public COleDispatchDriver
{
public:
	CContourLevels(){} // 调用 COleDispatchDriver 默认构造函数
	CContourLevels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CContourLevels(const CContourLevels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IContourLevels 方法
public:
	LPDISPATCH get_Items(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	long Clicked(long XCoord, long YCoord, long * SegmentIndex, long * PointIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, XCoord, YCoord, SegmentIndex, PointIndex);
		return result;
	}
	long AddLevel(double LevelValue, unsigned long Color)
	{
		long result;
		static BYTE parms[] = VTS_R8 VTS_UI4 ;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_I4, (void*)&result, parms, LevelValue, Color);
		return result;
	}

	// IContourLevels 属性
public:

};
