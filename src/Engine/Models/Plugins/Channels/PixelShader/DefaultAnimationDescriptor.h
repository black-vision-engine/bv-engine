#pragma once

#include <vector>

#include "Engine/Models/Interfaces/IAnimationDescriptor.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"


namespace bv { namespace model {

class DefaultAnimationDescriptor : public IAnimationDescriptor
{
private:

    std::vector< char * >   m_frames;

    DefaultTextureParams    m_params;

public:

    DefaultAnimationDescriptor      ();
    DefaultAnimationDescriptor      ( const std::string & name, unsigned int w, unsigned int h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc );
    ~DefaultAnimationDescriptor     ();

    virtual unsigned int            NumTextures     () const override;
    virtual const char *            GetBits         ( unsigned int idx ) const override;

    virtual const std::string       GetName         () const override;
    virtual unsigned int            GetWidth        () const override;
    virtual unsigned int            GetHeight       () const override;
    virtual TextureFormat           GetFormat       () const override;
    virtual TextureWrappingMode     GetWrappingModeX() const override;
    virtual TextureWrappingMode     GetWrappingModeY() const override;
    virtual TextureFilteringMode    GetFilteringMode() const override;
    virtual glm::vec4               BorderColor     () const override;

    void                            SetBits         ( unsigned int idx, const char * data, TextureFormat fmt, unsigned int w, unsigned int h );
    void                            AddBits         ( const char * data, TextureFormat fmt, unsigned int w, unsigned int h );
    void                            SetName         ( const std::string & name );
    void                            SetWidth        ( unsigned int w );
    void                            SetHeight       ( unsigned int h );
    void                            SetFormat       ( TextureFormat fmt );
    void                            SetWrappingModeX( TextureWrappingMode wm );
    void                            SetWrappingModeY( TextureWrappingMode wm );
    void                            SetFilteringMode( TextureFilteringMode fm );
    void                            SetBorderColor  ( const glm::vec4 & bc );

    static DefaultAnimationDescriptor * LoadAnimation   ( const std::vector< std::string > & frames, const std::string & name );

private:

    static unsigned int                 LoadFrame       ( const std::string & frame, TextureFormat * fmt, unsigned int * w, unsigned int * h, const char ** ppData );

};

} //model
} //bv
