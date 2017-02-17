#include "stdafx.h"

#include "VideoOutputsHandler.h"


namespace bv { namespace nrl {

// *********************************
//
void    VideoOutputsHandler::HandleOutputsData   ( const AVOutputsData & outputs )
{
    // FIXME: nrl - can't it be created once ang simply replate the wrapped pointers?
	auto VCMInput = std::make_shared< videocards::VCMInputData >();

	for( auto it = outputs.begin(); it != outputs.end(); ++it )
	{
		VCMInput->SetAVFrame( it->first, it->second );
	}

	videocards::VideoCardManager::Instance().Display( VCMInput );
}

} //bv
} //nrl
