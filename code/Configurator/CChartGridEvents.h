// �����Ϳ������á������ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CChartGridEvents ��װ��

class CChartGridEvents : public COleDispatchDriver
{
public:
	CChartGridEvents(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CChartGridEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartGridEvents(const CChartGridEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IChartGridEvents ����
public:
	STDMETHOD(OnClick)()
	{
		HRESULT result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_HRESULT, (void*)&result, NULL);
		return result;
	}

	// IChartGridEvents ����
public:

};