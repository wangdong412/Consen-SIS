// -----------------------------------------------------------------------
// $Workfile: CfgTrace.h $
// Author/Date  : rjvs 2002-11-13 07:29:24 (UTC-6:00)
// Description  : Trace array for the document
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Configurator/CfgTrace.h  3     2/06/03 3:11p Rvanslooten $
//
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2002-11-13  Ryan Van Slooten  Added Comment Block
// 2011-02-09  Wang Dong         Changed from BCB5 to Visual Studio 2010
// -----------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include "AliasItem.h"

//---------------------------------------------------------------------------
// { CCfgTrace }

class CCfgTrace
{
public:
    enum { clTraceDefault = 0x20000000 };

    CCfgTrace()
        : m_strName( L"" )
         ,m_width( 2 )
		 ,m_color( clTraceDefault )
         //,m_color(static_cast<Graphics::TColor>(clTraceDefault) )
         ,m_axis( 0 )
         ,m_scaled( false )
         ,m_scaleMin( 0.0 )
         ,m_scaleMax( 0.0 )
         ,m_scaleFactor( 0.0 )
         ,m_value( 0.0 )
         ,m_pAlias( NULL )
    {
    }

    CCfgTrace( CString strName, int nAxis )
        : m_strName( strName )
         ,m_width(2)
		 ,m_color(clTraceDefault)
         //,m_color(static_cast<Graphics::TColor>(clTraceDefault) )
         ,m_axis( nAxis )
         ,m_scaled( false )
         ,m_scaleMin( 0.0 )
         ,m_scaleMax( 0.0 )
         ,m_scaleFactor( 0.0 )
         ,m_value( 0.0 )
         ,m_pAlias( NULL )
    {
    }

private:
	//Graphics::TColor m_color;

	CString m_strName;
    int m_width;
	long m_color;
    int m_axis;
    bool m_scaled;
    float m_scaleMin;
    float m_scaleMax;
    float m_scaleFactor;
    float m_value;
    CAliasItem* m_pAlias;

public:
	// Name
	CString GetName()                       const { return m_strName; }
	LPCTSTR GetName_C()                     const { return (LPCTSTR)m_strName; }

	void SetName( CString strName )         { m_strName = strName; }
	void SetName( LPCTSTR pcStrName )       { m_strName = pcStrName; }

	// Alias
    CAliasItem* GetAlias()                  { return m_pAlias; }
    const CAliasItem* GetAlias()            const { return m_pAlias; }

    void SetAlias( CAliasItem* Value )      { m_pAlias = Value; }

	// Color
    long GetColor()                         const { return m_color; }
    void SetColor( long Value)              { m_color = Value; }

	// Width
    int GetWidth() const { return m_width; }
    void SetWidth(int Value) { m_width = Value; }

	// Axis
    int GetAxis() const { return m_axis; }
    void SetAxis(int Value)
    {
        if (Value == 0)
            m_axis = 0;
        else
            m_axis = 1;
    }

	// Scaled
    bool IsScaled() const { return m_scaled; }
    void CheckScaling()
    {
        m_scaled = (m_scaleMin != m_scaleMax);
        if (m_scaled)
            m_scaleFactor = 100.0f / (m_scaleMax - m_scaleMin);
        else
            m_scaleFactor = 0.0;
    }

	// ScaleMin, ScaleMax
    float GetScaleMin() const { return m_scaleMin; }
    void SetScaleMin(float Value) { m_scaleMin = Value; CheckScaling(); }

    float GetScaleMax() const { return m_scaleMax; }
    void SetScaleMax(float Value) { m_scaleMax = Value; CheckScaling(); }

    float GetScaleValue()
    {
        if (IsScaled() && (m_pAlias != NULL))
        {
            if (m_pAlias->GetDataType() == CAliasItem::ALIAS_BOOLEAN)
            {
                if (m_pAlias->valBool())
                    m_value = m_scaleMax;
                else
                    m_value = m_scaleMin;
                return m_value;
            }

            if (m_pAlias->GetDataType() == CAliasItem::ALIAS_INTEGER)
            {
                m_value = static_cast<float>(m_pAlias->valI2());
            }
            else if (m_pAlias->GetDataType() == CAliasItem::ALIAS_UINTEGER)
            {
                m_value = static_cast<float>(m_pAlias->valUI2());
            }
            else if ((m_pAlias->GetDataType() == CAliasItem::ALIAS_LONG) ||
                     (m_pAlias->GetDataType() == CAliasItem::ALIAS_LONGSWAP))
            {
                m_value = static_cast<float>(m_pAlias->valI4());
            }
            else if ((m_pAlias->GetDataType() == CAliasItem::ALIAS_FLOAT) ||
                     (m_pAlias->GetDataType() == CAliasItem::ALIAS_FLOATSWAP))
            {
                m_value = static_cast<float>(m_pAlias->valR4());
            }
            else
            {
                m_value = 0.0;
            }

            if (m_value < m_scaleMin)
            {
                m_value = m_scaleMin;
            }
            else if (m_value > m_scaleMax)
            {
                m_value = m_scaleMax;
            }
            else
            {
                m_value -= m_scaleMin;
                m_value *= m_scaleFactor;
            }
        }
        else
        {
            m_value = 0.0;
        }

        return m_value;
    }
};

//---------------------------------------------------------------------------
// { TCfgTraceFind }

struct CCfgTraceFind :  public std::unary_function<CCfgTrace, bool>
{
    LPCTSTR s_;

    CCfgTraceFind( LPCTSTR s ) : s_( s )  { }

    bool operator ()(const CCfgTrace& v) const
    {
        return ::lstrcmpi(s_, v.GetName_C()) == 0;
    }
};

//---------------------------------------------------------------------------
// { CCfgTraceArray }

class CCfgTraceArray : public std::vector<CCfgTrace>
{
private:
    typedef std::vector<CCfgTrace> inherited;

public:
    CCfgTraceArray()
        :inherited()
    {
    }

	CCriticalSection m_cirsec;

public:
    virtual void bind( CAliasMap* pAliasMap)
    {
        // Clear existing alias bindings and bind to current
        iterator  it = begin();
        iterator eit = end();

        if( !CheckReadPtr( pAliasMap, sizeof(CAliasMap)))
        {
            while (it != eit)
            {
                it->SetAlias( NULL );
                ++it;
            }
        }
        else
        {
            while (it != eit)
            {
				it->SetAlias( pAliasMap->find_name( it->GetName_C()));
                ++it;
            }
        }
    }
};

//---------------------------------------------------------------------------

