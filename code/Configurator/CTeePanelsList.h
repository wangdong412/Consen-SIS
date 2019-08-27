// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装类

#import "D:\\Registered Active X\\TeeChart8.ocx" no_namespace
// CTeePanelsList 包装类

class CTeePanelsList : public COleDispatchDriver
{
public:
	CTeePanelsList(){} // 调用 COleDispatchDriver 默认构造函数
	CTeePanelsList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeePanelsList(const CTeePanelsList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ITeePanelsList 方法
public:
	void Delete(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void Clear()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// ITeePanelsList 属性
public:

};
