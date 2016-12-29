#include "NRenderComponentState.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv { namespace nrl {

// **************************
//
bool                NRenderComponentState::SyncTexturesReq () const
{
    return m_syncTextures;
}

// **************************
//
bool                NRenderComponentState::SyncValuesReq   () const
{
    return m_syncValues;
}

// **************************
//
void                NRenderComponentState::SetSyncTextures ( bool syncRequested )
{
    m_syncTextures = syncRequested;
}

// **************************
//
void                NRenderComponentState::SetSyncValues   ( bool syncRequested )
{
    m_syncValues = syncRequested;
}

// **************************
//
void                NRenderComponentState::AppendValue      ( IValuePtr val )
{
    m_values.push_back( val );
    
    SetSyncValues();
}

// **************************
//
void                NRenderComponentState::AppendTexture    ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    m_textures.push_back( NTexture2DEntry( tex, samplerName, wrapX, wrapY, filteringMode ) );
    
    SetSyncTextures();
}

// **************************
//
void                NRenderComponentState::SetIValue        ( unsigned int i, IValuePtr val )
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
void                NRenderComponentState::SetTexture       ( unsigned int i, Texture2DPtr tex )
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
void                NRenderComponentState::SetSamplerName   ( unsigned int i, const std::string & samplerName )
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
void                NRenderComponentState::SetWrappinMode  ( unsigned int i, TextureWrappingMode wrapX, TextureWrappingMode wrapY )
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
void                NRenderComponentState::SetFilteringMode( unsigned int i, TextureFilteringMode filteringMode )
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
void                NRenderComponentState::SetTexture       ( unsigned int i, Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode )
{
    SetTexture( i, tex );
    SetSamplerName( i, samplerName );
    SetWrappinMode( i, wrapX, wrapY );
    SetFilteringMode( i, filteringMode );
}

// **************************
//
IValuePtr           NRenderComponentState::GetValueAt      ( unsigned int i ) const
{
    assert( i < m_values.size() );

    return m_values[ i ];
}

// **************************
//
IValuePtr           NRenderComponentState::GetValue        ( const std::string & name ) const
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
Texture2DPtr        NRenderComponentState::GetTextureAt    ( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetTexture();
}

// **************************
//
const std::string & NRenderComponentState::GetSamplerNameAt( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetSamplerName();
}

// **************************
//
TextureWrappingMode     NRenderComponentState::GetWrappinModeX ( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetWrappingModeX();
}

// **************************
//
TextureWrappingMode     NRenderComponentState::GetWrappinModeY ( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetWrappingModeY();
}

// **************************
//
TextureFilteringMode    NRenderComponentState::GetFilteringMode( unsigned int i ) const
{
    assert( m_textures.size() < i );

    return m_textures[ i ].GetFilteringMode();
}

// **************************
//
Texture2DPtr        NRenderComponentState::GetTexture      ( const std::string & samplerName ) const
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
unsigned int        NRenderComponentState::GetNumValues    () const
{
    return (unsigned int) m_values.size();
}

// **************************
//
unsigned int        NRenderComponentState::GetNumTextures  () const
{
    return (unsigned int) m_textures.size();
}

// **************************
//
const NTexture2DEntryVec &  NRenderComponentState::GetAllTextures  () const
{
    return m_textures;
}

// **************************
//
const IValuePtrVec &        NRenderComponentState::GetAllValues    () const
{
    return m_values;
}

// **************************
//
bool                NRenderComponentState::SetValue        ( const std::string & name, float val )
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
bool                NRenderComponentState::SetValue        ( const std::string & name, int val )
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
void                NRenderComponentState::SetSyncTextures  ()
{
    SetSyncTextures( true );
}

// **************************
//
void                NRenderComponentState::SetSyncValues    ()
{
    SetSyncValues( true );
}

} // nrl
} // bv
