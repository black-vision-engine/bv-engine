#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"

namespace bv { namespace model {

class DefaultTextureParams : public ITextureParams
{
private:

    std::string             m_name;

    UInt32				    m_width;
    UInt32				    m_height;
	UInt32				    m_depth;
   
    TextureFormat           m_format;
    
    SamplerStateModelPtr	m_samplerState;

public:

            DefaultTextureParams ();
            DefaultTextureParams ( const std::string & name, UInt32 w, UInt32 h, UInt32 d, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc );
            ~DefaultTextureParams();

    virtual const std::string       GetName         () const override;

    virtual UInt32				    GetWidth        ( UInt32 level = 0 ) const override;
    virtual UInt32				    GetHeight       ( UInt32 level = 0 ) const override;
	virtual UInt32				    GetDepth		( UInt32 level = 0 ) const override;

    virtual TextureFormat           GetFormat       () const override;

	virtual model::SamplerStateModelPtr	GetSamplerState () const override;

 //   virtual TextureWrappingMode     GetWrappingModeX() const override;
 //   virtual TextureWrappingMode     GetWrappingModeY() const override;
	//virtual TextureWrappingMode     GetWrappingModeZ() const override;

 //   virtual TextureFilteringMode    GetFilteringMode() const override;

 //   virtual glm::vec4               BorderColor     () const override;

    void                            SetName         ( const std::string & name );
    void                            SetWidth        ( UInt32 w );
    void                            SetHeight       ( UInt32 h );
	void                            SetDepth		( UInt32 z );
    void                            SetFormat       ( TextureFormat fmt );

	void							SetSamplerState ( SamplerStateModelPtr samplerState );

 //   void                            SetWrappingModeX( TextureWrappingMode wm );
 //   void                            SetWrappingModeY( TextureWrappingMode wm );
	//void                            SetWrappingModeZ( TextureWrappingMode wm );

 //   void                            SetFilteringMode( TextureFilteringMode fm );

 //   void                            SetBorderColor  ( const glm::vec4 & bc );

};

} //model
} //bv
