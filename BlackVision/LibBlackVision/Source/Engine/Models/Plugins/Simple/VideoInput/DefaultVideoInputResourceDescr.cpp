#include "stdafx.h"
#include "DefaultVideoInputResourceDescr.h"

namespace bv { namespace model {

std::string DefaultVideoInputResourceDescr::uid = "VIDEO_INPUT_DESC";
    
// *******************************
//
//model::IVideoInputResourceDescrConstPtr   QueryVideoInputResourceDescr   ( model::IPluginResourceDescrConstPtr resDescr )
//{
//    if ( resDescr->GetResourceType() == PluginResourceType::PRT_VIDEOINPUT )
//    {
//        return std::static_pointer_cast< const model::IVideoInputResourceDescr >( resDescr );
//    }
//    
//    return nullptr;
//}

// ***********************
//
SizeType        DefaultVideoInputResourceDescr::EstimateMemoryUsage () const
{
    return 0;
}


} }