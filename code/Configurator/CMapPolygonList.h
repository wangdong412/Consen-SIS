// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CMapPolygonList 包装类

class CMapPolygonList : public COleDispatchDriver
{
public:
	CMapPolygonList(){} // 调用 COleDispatchDriver 默认构造函数
	CMapPolygonList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMapPolygonList(const CMapPolygonList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IMapPolygonList 方法
public:
	long Add()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Polygon(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// IMapPolygonList 属性
public:

};
