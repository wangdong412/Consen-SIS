// �����Ϳ������á������ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CExpMovAvgFunction ��װ��

class CExpMovAvgFunction : public COleDispatchDriver
{
public:
	CExpMovAvgFunction(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CExpMovAvgFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CExpMovAvgFunction(const CExpMovAvgFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IExpMovAvgFunction ����
public:
	BOOL get_IgnoreNulls()
	{
		BOOL result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IgnoreNulls(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

	// IExpMovAvgFunction ����
public:

};