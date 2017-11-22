#include "FakeVideoCardDesc.h"



namespace bv {
namespace videocards
{




// ***********************
//
const std::string &             FakeVideoCardDesc::GetVideoCardUID      () const
{
    return UID();
}

// ***********************
//
void                            FakeVideoCardDesc::Deserialize          ( const IDeserializer & deser )
{
    deser;
}

// ***********************
//
IVideoCardPtr                   FakeVideoCardDesc::CreateVideoCard      () const
{
    return IVideoCardPtr();
}



}   // videocards
}   // bv

