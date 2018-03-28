#include "stdafx.h"

#include "BVSerializeContext.h"

#include "Assets/AssetDescsWithUIDs.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// ***********************
//
AssetDescsWithUIDsPtr               BVSerializeContext::GetAssets               ()
{
    return m_assets;
}

// ***********************
//
void                                BVSerializeContext::SetAssets               ( const AssetDescsWithUIDsPtr & assets )
{
    m_assets = assets;
}

// ***********************
//
model::TimelineManager *            BVSerializeContext::GetTimelineManager      ()
{
    return m_timelineManager;
}

// ***********************
//
BVSerializeContext *                BVSerializeContext::CreateContextFromEmptiness  ()
{
    return new BVSerializeContext( model::TimelineManager::GetInstance() );
}

// ***********************
//
BVSerializeContext *              Context             ( const ISerializer & ser )
{
    auto contextTyped = Cast< BVSerializeContext * >( ser.GetSerializeContext() );
    if( contextTyped == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Serialization context doesn't exist. This can cause undefined behavior of serialization.";
        assert( !"Probably you should create serializer with context. You can remove this assert, if you are sure returning nullptr is right behavior." );
        return nullptr;
    }

    return contextTyped;
}


}
