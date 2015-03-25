#pragma once

#include "Assets/Font/FontAsset.h"
#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/Font/Text.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h" // FIXME: Asset shouldn't depend on model
#include "Mathematics/Rect.h"

namespace bv { 

class TextAtlas;

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
    // Returns AssetHandle of this font
    static FontAssetConstPtr			LoadFont            ( const std::string & fontFileName, UInt32 size, UInt32 blurSize, UInt32 outlineSize );

    /////////////////////////////////////
    // This function creates TextAtlas from specified FontReasource.
    // Returns new TextAtlas.
    static TextAtlasConstPtr            GetAtlas            ( const AssetConstPtr & asset );

    /////////////////////////////////////
    // This function creates empty VertexAttributeChannel for text with proper attribute descriptor. ( position, texture cord )
    static model::VertexAttributesChannel * CreateEmptyVACForText   ();

    /////////////////////////////////////
    // This function builds VertexAttributeChannel for text and fontAsset.
    static float                        BuildVACForText     ( model::VertexAttributesChannel * vertexAttributeChannel, const TextAtlasConstPtr & textAtlas, const std::wstring& text, unsigned int blurSize, float spacing, TextAlignmentType tat, SizeType outlineSize, bool useKerning = true );
};


} // bv