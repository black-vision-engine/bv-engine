#include "HelperPixelShaderChannel.h"

#include "Application/ApplicationContext.h"

namespace bv { namespace model {

// ******************************
//
void			HelperPixelShaderChannel::SetTexturesDataUpdate			( DefaultPixelShaderChannelPtr psChannel )
{
	psChannel->SetTexturesDataUpdateID( ApplicationContext::Instance().GetTimestamp() + 1 );
}

// ******************************
//
void			HelperPixelShaderChannel::SetRendererContextUpdate		( DefaultPixelShaderChannelPtr psChannel )
{
	psChannel->SetRendererContextUpdateID( ApplicationContext::Instance().GetTimestamp() + 1 );
}

// ******************************
//
void		    HelperPixelShaderChannel::SetRendererContextUpdate		( IPixelShaderChannelPtr psChannel )
{
    assert( std::dynamic_pointer_cast< DefaultPixelShaderChannel >( psChannel ) );
    SetRendererContextUpdate( std::static_pointer_cast< DefaultPixelShaderChannel >( psChannel ) );
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


// ******************************
//
void			HelperPixelShaderChannel::CloneRenderContext		( DefaultPixelShaderChannelPtr psChannel, IPluginPtr prevPlugin )
{
	if( prevPlugin && prevPlugin->GetRendererContext() )
	{
		psChannel->SetRendererContext( prevPlugin->GetRendererContext()->Clone() );
	}
}

} //model
} //bv
