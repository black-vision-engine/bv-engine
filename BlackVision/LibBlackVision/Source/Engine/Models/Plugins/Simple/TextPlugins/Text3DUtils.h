#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Assets/Font/Text.h"
#include "Assets/Font/3D/FontAsset3D.h"

#include "Assets/Font/TextHelper.h"


namespace bv { namespace model
{

class Text3DUtils
{
public:

    struct TextLayout
    {
        float                   Size;
        float                   Spacing;
        float                   NewLineSize;
        FontAsset3DConstPtr     FontAsset;
        UInt32                  ViewWidth;
        UInt32                  ViewHeight;
        model::TextArranger *   Arranger;
        glm::vec2               Box;
        bool                    UseKerning;
        bool                    UseBox;
        TextAlignmentType       Tat;
        wchar_t                 AlignChar;
    };

public:

    static VertexAttributesChannelPtr               CreateEmptyVACForText3D     ();
    static std::vector< ConnectedComponentPtr >     CreateText                  ( const std::wstring & text, FontAsset3DConstPtr textAsset, TextLayout layout );

private:

    static std::vector< glm::vec3 >                 CreateLinesFromContour      ( const ContoursList & contours );
    static ConnectedComponentPtr                    CreateLetter                ( const wchar_t character, FontAsset3DConstPtr & textAsset, TextLayout layout );
    static void                                     ArrangeText                 ( const std::wstring & text, std::vector< ConnectedComponentPtr > & components, TextLayout layout );
	static ConnectedComponentPtr					CreateEmptyCC				();
};


} // model
} // bv
