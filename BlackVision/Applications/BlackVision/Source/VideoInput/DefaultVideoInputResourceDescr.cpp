#include "Engine\Models\Resources\IPluginResourceDescr.h"

namespace bv { namespace model {

// *******************************
//
model::IVideoInputResourceDescrConstPtr   QueryVideoInputResourceDescr   ( model::IPluginResourceDescrConstPtr resDescr )
{
    if ( resDescr->GetResourceType() == PluginResourceType::PRT_VIDEOINPUT )
    {
        return std::static_pointer_cast< const model::IVideoInputResourceDescr >( resDescr );
    }
    
    return nullptr;
}

} }