#pragma once


namespace bv { namespace videocards {

class IVideoCardDesc
{
public:

    virtual const std::string &     GetVideoCardUID     () const    = 0;
    virtual const std::string &     VideoCardName       () const    = 0;

    virtual IVideoCardPtr           CreateVideoCard     () const    = 0;

    virtual                         ~IVideoCardDesc     () {}

};

} //videocards
} //bv
