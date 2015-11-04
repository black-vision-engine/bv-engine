#include "HelperPixelShaderChannel.h"

namespace bv { namespace model {

// ******************************
//
void			HelperPixelShaderChannel::SetTexturesDataUpdate			( DefaultPixelShaderChannelPtr psChannel )
{
	psChannel->SetTexturesDataUpdateID( psChannel->GetTexturesDataUpdateID() + 1 );
}

// ******************************
//
void			HelperPixelShaderChannel::SetRendererContextUpdate		( DefaultPixelShaderChannelPtr psChannel )
{
	psChannel->SetRendererContextUpdateID( psChannel->GetRendererContextUpdateID() + 1 );
}

// ******************************
//
bool			HelperPixelShaderChannel::PropagateTexturesDataUpdate	( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin )
{
	if( prevPlugin && prevPlugin->GetPixelShaderChannel() )
	{
		auto prevUpdateId = prevPlugin->GetPixelShaderChannel()->GetTexturesDataUpdateID();
		if( psChannel->GetTexturesDataUpdateID() < prevUpdateId )
		{
			psChannel->SetTexturesDataUpdateID( prevUpdateId );
			return true;
		}
	}
	return false;
}

// ******************************
//
bool			HelperPixelShaderChannel::PropagateRendererContextUpdate( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin )
{
	if( prevPlugin && prevPlugin->GetPixelShaderChannel() )
	{
		auto prevUpdateId = prevPlugin->GetPixelShaderChannel()->GetRendererContextUpdateID();
		if( psChannel->GetRendererContextUpdateID() < prevUpdateId )
		{
			psChannel->SetRendererContextUpdateID( prevUpdateId );
			return true;
		}
	}
	return false;
}

// ******************************
//
void			HelperPixelShaderChannel::PropagateUpdate			( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin )
{
	if( prevPlugin && prevPlugin->GetPixelShaderChannel() )
	{
		auto psc = prevPlugin->GetPixelShaderChannel();
		auto txDataUpdateId = psc->GetTexturesDataUpdateID();
		if( psChannel->GetTexturesDataUpdateID() < txDataUpdateId )
		{
			psChannel->SetTexturesDataUpdateID( txDataUpdateId );
		}

		auto rendererCtxUpdateId = psc->GetRendererContextUpdateID();
		if( psChannel->GetRendererContextUpdateID() < rendererCtxUpdateId )
		{
			psChannel->SetRendererContextUpdateID( rendererCtxUpdateId );
		}
	}
}

} //model
} //bv
