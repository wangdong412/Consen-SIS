// �����Ϳ������á������ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CSeriesTextFields ��װ��

class CSeriesTextFields : public COleDispatchDriver
{
public:
	CSeriesTextFields(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CSeriesTextFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSeriesTextFields(const CSeriesTextFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// ISeriesTextFields ����
public:
	LPDISPATCH get_Items(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}
	void Clear()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Delete(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// ISeriesTextFields ����
public:

};