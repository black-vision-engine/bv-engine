#include "stdafx.h"

#include "VideoOutputsHandler.h"


namespace bv { namespace nrl {

// *********************************
//
void    VideoOutputsHandler::HandleOutputsData   ( const AVOutputsData & outputs )
{
	auto VCMInput = std::shared_ptr< videocards::VCMInputData >( new videocards::VCMInputData() );

	for( auto it = outputs.begin(); it != outputs.end(); ++it )
	{
		VCMInput->SetAVFrame( it->first, it->second );
	}

	videocards::VideoCardManager::Instance().Display( VCMInput );
}

// *********************************
//
void    VideoOutputsHandler::PreInitialize      ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{
    { uniqueOutputSetups; }
    { mapping; }
}

} //bv
} //nrl
