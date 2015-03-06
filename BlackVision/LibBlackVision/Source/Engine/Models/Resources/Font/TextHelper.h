#pragma once

#include "Engine/Models/Resources/Font/FontResource.h"
#include "Engine/Models/Resources/Font/Text.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Mathematics/Rect.h"

namespace bv { namespace model {

class TextAtlas;

#define SUPPROTED_CHARS_FILE L"../dep/Media/fonts/SupportedChars.txt"

enum class TextAlignmentType
{
    Left    = 0 ,
    Center      ,
    Right
};

class TextHelper
{
public:
    /////////////////////////////////////
    // This function loads font definition from file. Builds font atlases with the specified size.
    // Returns ResourceHandle of this font
    static FontResourceConstPtr			LoadFont            ( const std::string & fontFileName, UInt32 size, UInt32 blurSize, UInt32 outlineSize, const std::wstring & atlasCharSetFile = SUPPROTED_CHARS_FILE );

    /////////////////////////////////////
    // This function creates TextAtlas from specified FontReasource.
    // Returns new TextAtlas.
    static TextAtlasConstPtr            GetAtlas            ( const IResourceConstPtr & res );

    /////////////////////////////////////
    // This function creates empty VertexAttributeChannel for text with proper attribute descriptor. ( position, texture cord )
    static VertexAttributesChannel*     CreateEmptyVACForText   ();

    /////////////////////////////////////
    // This function builds VertexAttributeChannel for text and fontResource.
    static float                        BuildVACForText     ( VertexAttributesChannel* vertexAttributeChannel, const TextAtlasConstPtr & textAtlas, const std::wstring& text, unsigned int blurSize, float spacing, TextAlignmentType tat, SizeType outlineSize, bool useKerning = true );
};


} // model
} // bv