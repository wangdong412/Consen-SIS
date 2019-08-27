// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CCompressFunction 包装类

class CCompressFunction : public COleDispatchDriver
{
public:
	CCompressFunction(){} // 调用 COleDispatchDriver 默认构造函数
	CCompressFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCompressFunction(const CCompressFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICompressFunction 方法
public:
	long get_Compress()
	{
		long result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Compress(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// ICompressFunction 属性
public:

};
