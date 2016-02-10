#pragma once

#include "Engine/Events/Events.h"


namespace bv
{

class BVAppLogic;

class AssetHandlers
{
private:

    BVAppLogic*         m_appLogic;

public:

				AssetHandlers			( BVAppLogic* appLogic );
				~AssetHandlers			();

    void        CacheHandler			( bv::IEventPtr evt );
    void        LoadAsset               ( bv::IEventPtr eventPtr );
};

} //bv
