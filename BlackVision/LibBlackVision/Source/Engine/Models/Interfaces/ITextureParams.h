#pragma once

#include "Engine/Models/Plugins/Channels/PixelShader/SamplerStateModel.h"
#include "Engine/Types/Enums.h"
#include "CoreDEF.h"


namespace bv {

class ITextureParams
{
public:

    virtual const std::string       GetName         () const = 0;

    virtual UInt32				    GetWidth        ( UInt32 level = 0  ) const = 0;
    virtual UInt32				    GetHeight       ( UInt32 level = 0  ) const = 0;
	virtual UInt32				    GetDepth		( UInt32 level = 0  ) const = 0;

	virtual model::SamplerStateModelPtr	GetSamplerState () const = 0;

    virtual TextureFormat           GetFormat       () const = 0;

    virtual ~ITextureParams () {}

};

DEFINE_PTR_TYPE(ITextureParams)
DEFINE_CONST_PTR_TYPE(ITextureParams)

} //bv
