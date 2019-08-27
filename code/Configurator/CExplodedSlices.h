// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CExplodedSlices 包装类

class CExplodedSlices : public COleDispatchDriver
{
public:
	CExplodedSlices(){} // 调用 COleDispatchDriver 默认构造函数
	CExplodedSlices(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CExplodedSlices(const CExplodedSlices& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IExplodedSlices 方法
public:
	long get_Value(long Index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Index);
		return result;
	}
	void put_Value(long Index, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Index, newValue);
	}

	// IExplodedSlices 属性
public:

};
