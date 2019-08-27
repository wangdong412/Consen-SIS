// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CFibonacciLevels 包装类

class CFibonacciLevels : public COleDispatchDriver
{
public:
	CFibonacciLevels(){} // 调用 COleDispatchDriver 默认构造函数
	CFibonacciLevels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFibonacciLevels(const CFibonacciLevels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IFibonacciLevels 方法
public:
	LPDISPATCH get_Level(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}

	// IFibonacciLevels 属性
public:

};
