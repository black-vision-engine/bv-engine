#include "stdafx.h"

#include "RenderComponentState.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv { 

// **************************
//
bool                RenderComponentState::SyncTexturesReq () const
{
    return m_syncTextures;
}

// **************************
//
bool                RenderComponentState::SyncValuesReq   () const
{
    return m_syncValues;
}

// **************************
//
void                RenderComponentState::SetSyncTextures ( bool syncRequested )
{
    m_syncTextures = syncRequested;
}

// **************************
//
void                RenderComponentState::SetSyncValues   ( bool syncRequested )
{
    m_syncValues = syncRequested;
}

// **************************
//
void                RenderComponentState::AppendValue      ( IValuePtr val )
{
    m_values.push_back( val );
    
    SetSyncValues();
}

// **************************
//
void                RenderComponentState::AppendTexture    ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    m_textures.push_back( Texture2DEntry( tex, samplerName, wrapX, wrapY, filteringMode ) );
    
    SetSyncTextures();
}

// **************************
//
void                RenderComponentState::SetIValue        ( unsigned int i, IValuePtr val )
{
    assert( i < m_values.size() );

    if( val != m_values[ i ] )
    {
        m_values[ i ] = val;

        SetSyncValues();
    }
}

// **************************
//
void                RenderComponentState::SetTexture       ( unsigned int i, Texture2DPtr tex )
{
    assert( m_textures.size() < i );

    if ( m_textures[ i ].GetTexture() != tex )
    {
        m_textures[ i ].SetTexture( tex );
        
        SetSyncTextures();
    }
}

// **************************
//
void                RenderComponentState::SetSamplerName   ( unsigned int i, const std::string & samplerName )
{
    assert( m_textures.size() < i );

    if( m_textures[ i ].GetSamplerName() != samplerName )
    {
        m_textures[ i ].SetSamplerName( samplerName );        
        
        SetSyncTextures();
    }
}

// **************************
//
void                RenderComponentState::SetWrappinMode  ( unsigned int i, TextureWrappingMode wrapX, TextureWrappingMode wrapY )
{
    assert( m_textures.size() < i );

    if( m_textures[ i ].GetWrappingModeX() != wrapX || m_textures[ i ].GetWrappingModeY() != wrapY )
    {
        m_textures[ i ].SetWrappingModeX( wrapX );
        m_textures[ i ].SetWrappingModeY( wrapY );

        SetSyncTextures();
    }
}

// **************************
//
void                RenderComponentState::SetFilteringMode( unsigned int i, TextureFilteringMode filteringMode )
{
    assert( m_textures.size() < i );

    if( m_textures[ i ].GetFilteringMode() != filteringMode )
    {
        m_textures[ i ].SetFilteringMode( filteringMode );

        SetSyncTextures();
    }

}

// **************************
//
void                RenderComponentState::SetTexture       ( unsigned int i, Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    SetTexture( i, tex );
    SetSamplerName( i, samplerName );
    SetWrappinMode( i, wrapX, wrapY );
    SetFilteringMode( i, filteringMode );
}

// **************************
//
IValuePtr           RenderComponentState::GetValueAt      ( unsigned int i ) const
{
    assert( i < m_values.size() );

    return m_values[ i ];
}

// **************************
//
IValuePtr           RenderComponentState::GetValue        ( const std::string & name ) const
{
    for( auto val : m_values )
    {
        if ( val->GetName() == name )
        {
            return val;
        }
    }

    return nullptr;
}

// **************************
//
Texture2DPtr        RenderComponentState::GetTextureAt    ( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetTexture();
}

// **************************
//
const std::string & RenderComponentState::GetSamplerNameAt( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetSamplerName();
}

// **************************
//
TextureWrappingMode     RenderComponentState::GetWrappinModeX ( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetWrappingModeX();
}

// **************************
//
TextureWrappingMode     RenderComponentState::GetWrappinModeY ( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetWrappingModeY();
}

// **************************
//
TextureFilteringMode    RenderComponentState::GetFilteringMode( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetFilteringMode();
}

// **************************
//
Texture2DPtr        RenderComponentState::GetTexture      ( const std::string & samplerName ) const
{
    for( auto & entry : m_textures )
    {
        if ( entry.GetSamplerName() == samplerName )
        {
            return entry.GetTexture();
        }
    }

    return nullptr;
}

// **************************
//
unsigned int        RenderComponentState::GetNumValues    () const
{
    return (unsigned int) m_values.size();
}

// **************************
//
unsigned int        RenderComponentState::GetNumTextures  () const
{
    return (unsigned int) m_textures.size();
}

// **************************
//
const Texture2DEntryVec &  RenderComponentState::GetAllTextures  () const
{
    return m_textures;
}

// **************************
//
const IValuePtrVec &        RenderComponentState::GetAllValues    () const
{
    return m_values;
}

// **************************
//
bool                RenderComponentState::SetValue        ( const std::string & name, float val )
{
    auto ival = GetValue( name );

    if ( val )
    {
        auto floatVal = QueryTypedValue< ValueFloatPtr >( ival );

        floatVal->SetValue( val );

        return true;
    }

    return false;
}

// **************************
//
bool                RenderComponentState::SetValue        ( const std::string & name, int val )
{
    auto ival = GetValue( name );

    if ( val )
    {
        auto floatVal = QueryTypedValue< ValueIntPtr  >( ival );

        floatVal->SetValue( val );

        return true;
    }

    return false;
}

// **************************
//
void                RenderComponentState::SetSyncTextures  ()
{
    SetSyncTextures( true );
}

// **************************
//
void                RenderComponentState::SetSyncValues    ()
{
    SetSyncValues( true );
}


} // bv
