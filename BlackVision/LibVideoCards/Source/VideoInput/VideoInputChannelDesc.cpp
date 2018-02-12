#include "VideoInputChannelDesc.h"





namespace bv {
namespace Convert
{


// ***********************
//
std::pair< bv::videocards::VideoType, const char* > VideoInputTypeMapping[] =
{
    std::make_pair( bv::videocards::VideoType::Fill, "Fill" )
    , std::make_pair( bv::videocards::VideoType::Key, "Key" )
    , std::make_pair( bv::videocards::VideoType::Total, "" )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( bv::videocards::VideoType, VideoInputTypeMapping )



}   // Convert
}	// bv


