#pragma once

#include <memory>

// Calibrate base class which includes common calibrate operations.
class CMainFrame;
class CCalSheet;
class CCalibrateBase
{
public:
	CCalibrateBase()
		:m_bOrigMonitoring( false )
	{
	}

	virtual ~CCalibrateBase(void)
	{
	}

// avoid copy
private:
	CCalibrateBase(const CCalibrateBase&);
	CCalibrateBase& operator=(const CCalibrateBase&);

public:
	virtual bool CanCalibrate() = 0;
	virtual void Calibrate( std::auto_ptr<CCalSheet> &CalSheet ) = 0;
	virtual void Run();  // Do nothind

protected:
	virtual bool IsShutdown() = 0;

protected:
	bool m_bOrigMonitoring;

public:
	bool IsOrigMonitoring() const { return m_bOrigMonitoring; }
};


// DSC100 calibrate control
class CDSC100Calibrate : public CCalibrateBase
{
public:
	CDSC100Calibrate( CMainFrame *pMainFrame = NULL )
		:m_pMainFrame( pMainFrame )
	{
	}

private:
	CMainFrame *m_pMainFrame;

public:
	bool CanCalibrate();
	void Calibrate( std::auto_ptr<CCalSheet> &CalSheet );
	void Run();

protected:
	bool IsShutdown();

private:
	bool IsCheckShutdown();
	void ResetCalibrationDatas();

public:
	void SetAliasFlag( bool bInfoFlag,  bool bMinFlag, bool bMaxFlag, bool bVerFlag,
                       bool bSaveFlag,  bool bIsCal,   bool bCancel,  bool bRun, bool bChooseNo );
	void SendCalibrationFlags( bool bSave );
};


// DSC110 calibrate control
class CDSC110Calibrate : public CCalibrateBase
{
public:
	CDSC110Calibrate()
	{
	}

public:
	bool CanCalibrate();
	void Calibrate( std::auto_ptr<CCalSheet> &CalSheet );

protected:
	bool IsShutdown();

public:
	void SetAliasFlag( bool bAngleFlag, bool bVerFlag, bool bSaveRunFlag,  bool bCancel );
};

