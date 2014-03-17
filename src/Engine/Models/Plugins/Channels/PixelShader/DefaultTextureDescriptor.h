#pragma once

#include "Engine/Models/Interfaces/ITextureDescriptor.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"


namespace bv { namespace model {

class DefaultTextureDescriptor : public ITextureDescriptor
{
private:

    char *                  m_data;

    DefaultTextureParams    m_params;

public:

    DefaultTextureDescriptor        ();
    DefaultTextureDescriptor        ( const char * data, const std::string & name, unsigned int w, unsigned int h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc );
    ~DefaultTextureDescriptor       ();

    virtual const char *            GetBits         () const override;

    virtual const std::string       GetName         () const override;
    virtual unsigned int            GetWidth        () const override;
    virtual unsigned int            GetHeight       () const override;
    virtual TextureFormat           GetFormat       () const override;
    virtual TextureWrappingMode     GetWrappingModeX() const override;
    virtual TextureWrappingMode     GetWrappingModeY() const override;
    virtual TextureFilteringMode    GetFilteringMode() const override;
    virtual glm::vec4               BorderColor     () const override;

    void                            SetBits         ( const char * data, TextureFormat fmt, unsigned int w, unsigned int h );
    void                            SetName         ( const std::string & name );
    void                            SetWidth        ( unsigned int w );
    void                            SetHeight       ( unsigned int h );
    void                            SetFormat       ( TextureFormat fmt );
    void                            SetWrappingModeX( TextureWrappingMode wm );
    void                            SetWrappingModeY( TextureWrappingMode wm );
    void                            SetFilteringMode( TextureFilteringMode fm );
    void                            SetBorderColor  ( const glm::vec4 & bc );
    
    static void                     SetDefaults     ( DefaultTextureDescriptor * desc );

    static DefaultTextureDescriptor * LoadTexture   ( const std::string & textureFile, const std::string & name );

};

} //model
} //bv
