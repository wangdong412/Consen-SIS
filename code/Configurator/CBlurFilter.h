// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CBlurFilter 包装类

class CBlurFilter : public COleDispatchDriver
{
public:
	CBlurFilter(){} // 调用 COleDispatchDriver 默认构造函数
	CBlurFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBlurFilter(const CBlurFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IBlurFilter 方法
public:
	long get_Amount()
	{
		long result;
		InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Amount(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Steps()
	{
		long result;
		InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Steps(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IBlurFilter 属性
public:

};
