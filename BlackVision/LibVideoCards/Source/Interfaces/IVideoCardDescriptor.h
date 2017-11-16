#pragma once

#include "Interfaces/IVideoCard.h"
#include "Serialization/IDeserializer.h"
#include <set>

namespace bv {
namespace videocards
{



/**@brief Descriptor of video card.
@ingorup VideoCards*/
class IVideoCardDesc
{
public:

    virtual const std::string &     GetVideoCardUID     () const                                = 0;

    virtual void                    Deserialize         ( const IDeserializer & deser )         = 0;
    virtual IVideoCardPtr           CreateVideoCard     () const                                = 0;

    virtual                         ~IVideoCardDesc     () {}

};

DEFINE_PTR_TYPE( IVideoCardDesc )


} //videocards
} //bv
