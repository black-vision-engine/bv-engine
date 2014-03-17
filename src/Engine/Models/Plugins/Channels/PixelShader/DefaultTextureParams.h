#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"


namespace bv { namespace model {

class DefaultTexturesData : public ITextureParams
{
private:

    std::string             m_name;
    unsigned int            m_width;
    unsigned int            m_height;
    TextureFormat           m_format;
    TextureWrappingMode     m_wrappingModeY;
    TextureWrappingMode     m_wrappingModeX;
    TextureFilteringMode    m_filteringMode;
    glm::vec4               m_borderColor;

public:

            DefaultTexturesData ();
            DefaultTexturesData ( const std::string & name, unsigned int w, unsigned int h, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc );
            ~DefaultTexturesData();

    virtual const std::string       GetName         () const override;

    virtual unsigned int            GetWidth        () const override;
    virtual unsigned int            GetHeight       () const override;
    virtual TextureFormat           GetFormat       () const override;
    virtual TextureWrappingMode     GetWrappingModeX() const override;
    virtual TextureWrappingMode     GetWrappingModeY() const override;
    virtual TextureFilteringMode    GetFilteringMode() const override;
    virtual glm::vec4               BorderColor     () const override;

    void                            SetName         ( const std::string & name );
    void                            SetWidth        ( unsigned int w );
    void                            SetHeight       ( unsigned int h );
    void                            SetFormat       ( TextureFormat format );
    void                            SetWrappingModeX( TextureWrappingMode wm );
    void                            SetWrappingModeY( TextureWrappingMode wm );
    void                            SetFilteringMode( TextureFilteringMode fm );
    void                            SetBorderColor  ( const glm::vec4 & bc );

};

} //model
} //bv
