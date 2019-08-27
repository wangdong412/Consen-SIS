// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CGridTransposeTool 包装类

class CGridTransposeTool : public COleDispatchDriver
{
public:
	CGridTransposeTool(){} // 调用 COleDispatchDriver 默认构造函数
	CGridTransposeTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGridTransposeTool(const CGridTransposeTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IGridTransposeTool 方法
public:
	void Transpose()
	{
		InvokeHelper(0x12d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	VARIANT get_Series()
	{
		VARIANT result;
		InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Series(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x12e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}

	// IGridTransposeTool 属性
public:

};
