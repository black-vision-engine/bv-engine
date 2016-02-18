#include "stdafx.h"
#include "BlendingModeHelper.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"


namespace bv {
namespace model {

const std::string        BlendingModeHelper::PARAM_BLEND_MODE     = "blend mode";


// ***********************
//
void         BlendingModeHelper::SetBlendRendererContext   ( DefaultPixelShaderChannelPtr psc, IParameterPtr param )
{
    assert( param != nullptr );
    assert( psc != nullptr );

    auto ctx = psc->GetRendererContext();
    auto blendModeParam = QueryTypedParam< std::shared_ptr< ParamEnum< BlendingModeHelper::BlendMode > > >( param );

}


}	// model

}	// bv