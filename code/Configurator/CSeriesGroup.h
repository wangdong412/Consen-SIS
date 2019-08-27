// �����Ϳ������á������ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CSeriesGroup ��װ��

class CSeriesGroup : public COleDispatchDriver
{
public:
	CSeriesGroup(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CSeriesGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSeriesGroup(const CSeriesGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// ISeriesGroup ����
public:
	void Add(long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, SeriesIndex);
	}
	void Hide()
	{
		InvokeHelper(0xca, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Show()
	{
		InvokeHelper(0xcb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long get_Active()
	{
		long result;
		InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Active(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xcd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_SeriesCount()
	{
		VARIANT result;
		InvokeHelper(0xce, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_GroupLink()
	{
		long result;
		InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Delete(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void Insert(long IndexInGroup, long SeriesIndex)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xd1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, IndexInGroup, SeriesIndex);
	}
	LPDISPATCH get_Series()
	{
		LPDISPATCH result;
		InvokeHelper(0xd2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// ISeriesGroup ����
public:

};