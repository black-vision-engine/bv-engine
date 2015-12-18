#include "AssetHandlers.h"

#include "../../BVAppLogic.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"
#include "Engine/Events/EventHelpers.h"             // wstring to string conversions and vice versa


namespace bv
{

// ***********************
//
AssetHandlers::AssetHandlers( BVAppLogic* appLogic )
    :   m_appLogic( appLogic )
{
}

// ***********************
//
AssetHandlers::~AssetHandlers()
{
}

// ***********************
//
void		AssetHandlers::CacheHandler			( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::AssetEvent::Type() )
    {
        bv::AssetEventPtr assetEvent = std::static_pointer_cast< bv::AssetEvent >( evt );
        AssetEvent::Command command = assetEvent->AssetCommand;
		
        if( command == AssetEvent::Command::ClearUnusedCachedAssets )
		{
			GTexture2DCache.ClearUnused();
		}
    }
}

} //bv
