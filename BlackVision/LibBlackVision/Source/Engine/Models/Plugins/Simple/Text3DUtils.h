#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model
{

class Text3DUtils
{
private:
    Text3DUtils();
    ~Text3DUtils();

public:

    static Float3AttributeChannelConstPtr CreateAttrChannel( const wchar_t & wch, const std::string & fontPath, SizeType size );


};


} // model
} // bv
