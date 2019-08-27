// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CTransposeSeriesTool 包装类

class CTransposeSeriesTool : public COleDispatchDriver
{
public:
	CTransposeSeriesTool(){} // 调用 COleDispatchDriver 默认构造函数
	CTransposeSeriesTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTransposeSeriesTool(const CTransposeSeriesTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ITransposeSeriesTool 方法
public:
	void Transpose()
	{
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// ITransposeSeriesTool 属性
public:

};
