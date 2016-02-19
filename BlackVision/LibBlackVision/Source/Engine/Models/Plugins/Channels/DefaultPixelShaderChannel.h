#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

namespace bv { namespace model {

class DefaultPixelShaderChannel;
DEFINE_PTR_TYPE(DefaultPixelShaderChannel)

class DefaultPixelShaderChannel : public ShaderChannel< IPixelShaderChannel >
{
protected:

    RendererContextPtr      m_rendererContext;
    DefaultTexturesDataPtr  m_texturesData;

	UInt64					m_texturesDataUpdateID;
	UInt64					m_rendererContexUpdateID;

public:

            DefaultPixelShaderChannel                       ( const std::string & shaderSource, IValueSetConstPtr valueSet, DefaultTexturesDataPtr txData, RendererContextPtr ctx );
    virtual ~DefaultPixelShaderChannel                      ();

    virtual RendererContextConstPtr         GetRendererContext  () const override;
    RendererContextPtr                      GetRendererContext  ();

    void                                    SetRendererContext      ( RendererContextPtr ctx );
    void                                    UpdateRendererContext   ( RendererContextConstPtr ctx );

    virtual ITexturesDataConstPtr           GetTexturesData     () const override;
    DefaultTexturesDataPtr                  GetTexturesDataImpl ();

	virtual UInt64							GetTexturesDataUpdateID		() const override;
	void									SetTexturesDataUpdateID		( UInt64 updateID );

	virtual UInt64							GetRendererContextUpdateID	() const override;
	void									SetRendererContextUpdateID	( UInt64 updateID );

    static  DefaultPixelShaderChannelPtr    Create              ();
    static  DefaultPixelShaderChannelPtr    Create              ( const std::string & shaderFile, IValueSetConstPtr values, DefaultTexturesDataPtr txData = nullptr, RendererContextPtr ctx = nullptr );
	
	static  DefaultPixelShaderChannelPtr    Create              ( IValueSetConstPtr values, DefaultTexturesDataPtr txData = nullptr, RendererContextPtr ctx = nullptr );

};

} //model
} //bv
