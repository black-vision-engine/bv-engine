#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Assets/Font/Text.h"

#include "Assets/Font/TextHelper.h"


namespace bv { namespace model
{

class Text3DUtils
{
public:

    struct TextLayout
    {
        float                   Size;
        TextConstPtr            TextAsset;
        SizeType                BlurSize;
        float                   Spacing;
        TextAlignmentType       Tat;
        SizeType                OutlineSize;
        UInt32                  ViewWidth;
        UInt32                  ViewHeight;
        model::TextArranger *   Arranger;
        bool                    UseKerning;
    };

public:

    /*static Float3AttributeChannelConstPtr           CreateAttrChannel           ( const wchar_t & wch, const std::string & fontPath, SizeType size );*/
    static VertexAttributesChannelPtr               CreateEmptyVACForText3D     ();

    static std::vector< ConnectedComponentPtr >     CreateText                  ( const std::wstring& text, TextConstPtr textAsset, TextLayout layout );

private:

    static std::vector< glm::vec3 >                 CreateLinesFromContour      ( const ContoursVec & contours );
    static ConnectedComponentPtr                    CreateLetter                ( const wchar_t character, TextConstPtr& textAsset, TextLayout layout );
    static void                                     ArrangeText                 ( const std::wstring& text, std::vector< ConnectedComponentPtr > & components, TextLayout layout );
};


} // model
} // bv
