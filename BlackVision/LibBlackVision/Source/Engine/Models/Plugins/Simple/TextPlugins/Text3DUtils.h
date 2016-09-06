#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Assets/Font/Text.h"

namespace bv { namespace model
{

class Text3DUtils
{
private:
    Text3DUtils();
    ~Text3DUtils();

public:

    /*static Float3AttributeChannelConstPtr           CreateAttrChannel           ( const wchar_t & wch, const std::string & fontPath, SizeType size );*/
    static VertexAttributesChannelPtr               CreateEmptyVACForText3D     ();

    static std::vector< ConnectedComponentPtr >     CreateText                  ( const std::wstring& text, TextConstPtr textAsset );

private:
    static ConnectedComponentPtr                    CreateLetter                ( const wchar_t character, TextConstPtr& textAsset );
};


} // model
} // bv
