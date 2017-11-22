#pragma once

#include "Interfaces/IVideoCardDescriptor.h"


namespace bv {
namespace videocards
{


// ***********************
//
class FakeVideoCardDesc : public IVideoCardDesc
{
private:

    static std::string      sUID;

public:

    virtual const std::string &     GetVideoCardUID     () const override;

    virtual void                    Deserialize         ( const IDeserializer & deser ) override;
    virtual IVideoCardPtr           CreateVideoCard     () const override;

    virtual                         ~FakeVideoCardDesc  () {}

public:

    static const std::string &      UID                     () { return sUID; }
    static IVideoCardDescPtr        CreateDescriptor        () { return std::make_shared< FakeVideoCardDesc >(); }
};


DEFINE_PTR_TYPE( FakeVideoCardDesc )

}   // videocards
}   // bv
