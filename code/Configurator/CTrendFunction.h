// �����Ϳ������á������ࡱ�����ļ�������ɵ� IDispatch ��װ��

#import "C:\Windows\\System32\\TeeChart8.ocx" no_namespace
// CTrendFunction ��װ��

class CTrendFunction : public COleDispatchDriver
{
public:
	CTrendFunction(){} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CTrendFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTrendFunction(const CTrendFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// ITrendFunction ����
public:
	long CalculateTrend(double * m, double * b, long SourceSeriesIndex, long FirstIndex, long LastIndex)
	{
		long result;
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, m, b, SourceSeriesIndex, FirstIndex, LastIndex);
		return result;
	}

	// ITrendFunction ����
public:

};