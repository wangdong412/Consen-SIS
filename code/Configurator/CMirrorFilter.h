// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CMirrorFilter 包装类

class CMirrorFilter : public COleDispatchDriver
{
public:
	CMirrorFilter(){} // 调用 COleDispatchDriver 默认构造函数
	CMirrorFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMirrorFilter(const CMirrorFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IMirrorFilter 方法
public:
	long get_Direction()
	{
		long result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Direction(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IMirrorFilter 属性
public:

};
