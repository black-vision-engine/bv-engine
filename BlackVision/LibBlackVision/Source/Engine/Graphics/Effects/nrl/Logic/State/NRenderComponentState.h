#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NTexture2DEntry.h"


namespace bv { namespace nrl {

class NRenderComponentState
{
private:

    bool                m_syncTextures;
    bool                m_syncValues;

    IValuePtrVec        m_values;
    NTexture2DEntryVec  m_textures;

public:

    bool                    SyncTexturesReq () const;
    bool                    SyncValuesReq   () const;

    void                    SetSyncTextures ( bool syncRequested );
    void                    SetSyncValues   ( bool syncRequested );

    void                    AppendValue     ( IValuePtr val );
    void                    AppendTexture   ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

    void                    SetIValue       ( unsigned int i, IValuePtr val );

    void                    SetTexture      ( unsigned int i, Texture2DPtr tex );
    void                    SetSamplerName  ( unsigned int i, const std::string & samplerName );
    void                    SetWrappinMode  ( unsigned int i, TextureWrappingMode wrapX, TextureWrappingMode wrapY );
    void                    SetFilteringMode( unsigned int i, TextureFilteringMode filteringMode );
    void                    SetTexture      ( unsigned int i, Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

    IValuePtr               GetValueAt      ( unsigned int i ) const;
    IValuePtr               GetValue        ( const std::string & name ) const;

    Texture2DPtr            GetTextureAt    ( unsigned int i ) const;
    const std::string &     GetSamplerNameAt( unsigned int i ) const;
    TextureWrappingMode     GetWrappinModeX ( unsigned int i ) const;
    TextureWrappingMode     GetWrappinModeY ( unsigned int i ) const;
    TextureFilteringMode    GetFilteringMode( unsigned int i ) const;
    Texture2DPtr            GetTexture      ( const std::string & samplerName ) const;

    unsigned int            GetNumValues    () const;
    unsigned int            GetNumTextures  () const;

    const NTexture2DEntryVec &  GetAllTextures  () const;
    const IValuePtrVec &        GetAllValues    () const;

    // Overloaded set vals
    bool                    SetValue        ( const std::string & name, float val );
    bool                    SetValue        ( const std::string & name, int val );

private:

    void                    SetSyncTextures ();
    void                    SetSyncValues   ();

};

DEFINE_PTR_TYPE(NRenderComponentState)
DEFINE_CONST_PTR_TYPE(NRenderComponentState)

// ****************************
//
inline IValuePtr   get_value( NRenderComponentStatePtr state, const std::string & name )
{
    return state->GetValue( name );
}

} // nrl
} // bv
