#pragma once

#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/Resource.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

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
    static ResourceHandleConstPtr       LoadFont            ( const std::string& fontFileName, size_t size, size_t blurSize, const std::wstring& atlasCharSetFile = SUPPROTED_CHARS_FILE );

    /////////////////////////////////////
    // This function creates TextAtlas from specified FontReasource.
    // Returns new TextAtlas.
    static const TextAtlas *            GetAtlas            ( const ResourceHandle * fontResource, bool bolded = false, bool italic = false );

    /////////////////////////////////////
    // This function creates TextureInfo from atlas specified atlas ind fontReasource.
    // Returns new TextureInfo with texture name specified in texName.
    static ResourceHandleConstPtr       GetAtlasTextureInfo ( const TextAtlas * textAtlas );

    /////////////////////////////////////
    // This function creates empty VertexAttributeChannel for text with proper attribute descriptor. ( position, texture cord )
    static VertexAttributesChannel*     CreateEmptyVACForText   ();

    /////////////////////////////////////
    // This function builds VertexAttributeChannel for text and fontResource.
    static float                        BuildVACForText     ( VertexAttributesChannel* vertexAttributeChannel, const TextAtlas * textAtlas, const std::wstring& text, unsigned int blurSize, float spacing, TextAlignmentType tat, bool useKerning = true );
};


} // model
} // bv