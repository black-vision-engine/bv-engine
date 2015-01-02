#pragma once

#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/Resource.h"
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
    static ResourceHandleConstPtr       LoadFont            ( const std::string& fontFileName, size_t size, size_t blurSize, size_t outlineSize, const std::wstring& atlasCharSetFile = SUPPROTED_CHARS_FILE );

    /////////////////////////////////////
    // This function creates TextAtlas from specified FontReasource.
    // Returns new TextAtlas.
    static const TextAtlas *            GetAtlas            ( const ResourceHandle * fontResource );

    /////////////////////////////////////
    // This function creates empty VertexAttributeChannel for text with proper attribute descriptor. ( position, texture cord )
    static VertexAttributesChannel*     CreateEmptyVACForText   ();

    /////////////////////////////////////
    // This function builds VertexAttributeChannel for text and fontResource.
    static float                        BuildVACForText     ( VertexAttributesChannel* vertexAttributeChannel, const TextAtlas * textAtlas, const std::wstring& text, unsigned int blurSize, float spacing, TextAlignmentType tat, SizeType outlineSize, bool useKerning = true );
};


} // model
} // bv