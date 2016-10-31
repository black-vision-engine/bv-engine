#pragma once

#include "Assets/Font/FontAsset.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h" // FIXME: Asset shouldn't depend on model


namespace bv { 

namespace model
{
class TextArranger;

} // model

class TextAtlas;

enum class TextAlignmentType
{
    Left    = 0 ,
    Center      ,
    Right       ,
	Dot
};

class TextHelper
{
public:
    /////////////////////////////////////
    // This function loads font definition from file. Builds font atlases with the specified size.
    // Returns AssetHandle of this font
    static FontAssetConstPtr			LoadFont            ( const FontAssetDescConstPtr & fontAssetDesc );

    /////////////////////////////////////
    // This function creates TextAtlas from specified FontReasource.
    // Returns new TextAtlas.
    static TextAtlasConstPtr            GetAtlas            ( const AssetConstPtr & asset );

    /////////////////////////////////////
    // This function creates empty VertexAttributeChannel for text with proper attribute descriptor. ( position, texture cord )
    static model::VertexAttributesChannelPtr CreateEmptyVACForText   ();

    /////////////////////////////////////
    // This function builds VertexAttributeChannel for text and fontAsset.
    static float                        BuildVACForText     ( model::VertexAttributesChannel * vertexAttributeChannel, const TextAtlasConstPtr & textAtlas, const std::wstring& text, SizeType blurSize, float spacing, TextAlignmentType tat, SizeType outlineSize, UInt32 viewWidth, UInt32 viewHeight, model::TextArranger * arranger = nullptr, bool useKerning = true );
};


} // bv