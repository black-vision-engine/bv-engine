#pragma once

#include "Serialization/IDeserializer.h"
#include <set>

namespace bv { namespace videocards {

class IVideoCardDesc
{
public:

    virtual const std::string &     GetVideoCardUID     () const    = 0;

    virtual IVideoCardPtr           CreateVideoCard     ( const IDeserializer & deser ) const    = 0;

    virtual                         ~IVideoCardDesc     () {}

};

} //videocards
} //bv
