#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"

namespace bv { namespace model {

class HelperPixelShaderChannel
{
public:
	static void			SetTexturesDataUpdate			( DefaultPixelShaderChannelPtr psChannel );
	static void			SetRendererContextUpdate		( DefaultPixelShaderChannelPtr psChannel );
    static void		    SetRendererContextUpdate		( IPixelShaderChannelPtr psChannel );

	static bool			PropagateTexturesDataUpdate		( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin );
	static bool			PropagateRendererContextUpdate	( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin );

	static void			PropagateUpdate					( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin );

	static void			CloneRenderContext				( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin );
};

} //model
} //bv
