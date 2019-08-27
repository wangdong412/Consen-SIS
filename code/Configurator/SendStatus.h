#pragma once


class CSettingVector;

class CSendStatus
{
public:
	CSendStatus(void);
	~CSendStatus(void);

private:
	int m_nDefChanged;
	int m_nDefSame;
    int m_nTuningChanged;
	int m_nTuningSame;
	int m_nCfgChanged;
	int m_nCfgSame;

	CString m_strMode;
	float m_fSpeed;
	float m_fProgress;

public:
	// Def Changed
	void SetDefChanged( int value )        { m_nDefChanged = value; }
	int  GetDefChanged()                   const { return m_nDefChanged; }

	// Def Same
	void SetDefSame( int value )           { m_nDefSame = value; }
	int  GetDefSame()                      const { return m_nDefSame; }

	// Tuning Changed
	void SetTuningChanged( int value )     { m_nTuningChanged = value; }
	int  GetTuningChanged()                const { return m_nTuningChanged; }

	// Tuning Same
	void SetTuningSame( int value )        { m_nTuningSame = value; }
	int  GetTuningSame()                   const { return m_nTuningSame; }

	// Cfg Changed
	void SetCfgChanged( int value )        { m_nCfgChanged = value; }
	int  GetCfgChanged()                   const { return m_nCfgChanged; }

	// Cfg Same
	void SetCfgSame( int value )           { m_nCfgSame = value; }
	int  GetCfgSame()                      const { return m_nCfgSame; }

	// Mode
	void SetMode( CString strMode )        { m_strMode = strMode; }
	CString GetMode()                      const {return m_strMode; }             

	// Speed
	void SetSpeed( float value )           { m_fSpeed = value; }
	float GetSpeed()                       const { return m_fSpeed; }

	// Progress
	void SetProgress( float value )        { m_fProgress = value; }
	float GetProgress()                    const { return m_fProgress; }

	// Update SendDlg status
	void UpdateSendDlgStatus( const CSettingVector* pSettings );
};

