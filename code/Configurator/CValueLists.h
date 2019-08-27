// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CValueLists 包装类

class CValueLists : public COleDispatchDriver
{
public:
	CValueLists(){} // 调用 COleDispatchDriver 默认构造函数
	CValueLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CValueLists(const CValueLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IValueLists 方法
public:
	long get_Count()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Items(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}

	// IValueLists 属性
public:

};
