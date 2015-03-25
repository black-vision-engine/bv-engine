#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"


namespace bv { namespace model {

class DefaultTextureParams : public ITextureParams
{
private:

    std::string             m_name;
    SizeType				m_width;
    SizeType				m_height;
    TextureFormat           m_format;
    TextureWrappingMode     m_wrappingModeY;
    TextureWrappingMode     m_wrappingModeX;
    TextureFilteringMode    m_filteringMode;
    glm::vec4               m_borderColor;

public:

            DefaultTextureParams ();
            DefaultTextureParams ( const std::string & name, unsigned int w, unsigned int h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc );
            ~DefaultTextureParams();

    virtual const std::string       GetName         () const override;
    virtual SizeType				GetWidth        ( UInt32 level = 0 ) const override;
    virtual SizeType				GetHeight       ( UInt32 level = 0 ) const override;
    virtual TextureFormat           GetFormat       () const override;
    virtual TextureWrappingMode     GetWrappingModeX() const override;
    virtual TextureWrappingMode     GetWrappingModeY() const override;
    virtual TextureFilteringMode    GetFilteringMode() const override;
    virtual glm::vec4               BorderColor     () const override;

    void                            SetName         ( const std::string & name );
    void                            SetWidth        ( SizeType w );
    void                            SetHeight       ( SizeType h );
    void                            SetFormat       ( TextureFormat fmt );
    void                            SetWrappingModeX( TextureWrappingMode wm );
    void                            SetWrappingModeY( TextureWrappingMode wm );
    void                            SetFilteringMode( TextureFilteringMode fm );
    void                            SetBorderColor  ( const glm::vec4 & bc );

};

} //model
} //bv
