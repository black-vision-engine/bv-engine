#pragma once

#include "Assets/Font/2D/FontAsset2D.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h" // FIXME: Asset shouldn't depend on model


namespace bv { 

namespace model
{
class TextArranger;

} // model

class TextAtlas;

// ***********************
//
enum class TextAlignmentType
{
    Left    = 0 ,
    Center      ,
    Right       ,
    Character   ,
    Justification
};


// ***********************
//
struct TextLayoutInfo
{
    float                   Interspace;
    float                   NewLineSize;
    float                   SpaceSize;
    float                   AspectRatio;
    float                   MaxLength;
    TextAlignmentType       TextAlign;
    wchar_t                 AlignChar;
    bool                    UseKerning;
    bool                    UseOutline;
};


class TextHelper
{
public:
    /////////////////////////////////////
    // This function loads font definition from file. Builds font atlases with the specified size.
    // Returns AssetHandle of this font
    static FontAsset2DConstPtr          LoadFont            ( const FontAssetDescConstPtr & fontAssetDesc );

    /////////////////////////////////////
    // This function creates TextAtlas from specified FontReasource.
    // Returns new TextAtlas.
    static TextAtlasConstPtr            GetAtlas            ( const AssetConstPtr & asset );

    /////////////////////////////////////
    // This function creates empty VertexAttributeChannel for text with proper attribute descriptor. ( position, texture cord )
    static model::VertexAttributesChannelPtr CreateEmptyVACForText   ();

    /////////////////////////////////////
    // This function builds VertexAttributeChannel for text and fontAsset.
    static float                        BuildVACForText     ( model::VertexAttributesChannel * vertexAttributeChannel, const TextAtlasConstPtr & textAtlas, const std::wstring& text, SizeType blurSize, float spacing, TextAlignmentType tat, wchar_t alignChar, SizeType outlineSize, UInt32 viewWidth, UInt32 viewHeight, float newLineSize, glm::vec2 box, model::TextArranger * arranger = nullptr, bool useKerning = true );

	static std::vector< glm::vec2 >		GetAtlasCoordsForGlyph( const Glyph * glyph, SizeType atlasW, SizeType atlasH, Float32 atlasBlurSize );

    /////////////////////////////////////
    // Computes alignement and aplies text translation. Version PC translates positions and centers, version P only positions.
    static void                         ApplyAlignementPC   ( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot, std::vector< model::IConnectedComponentPtr > & components );
    static void                         ApplyAlignementP    ( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot, std::vector< model::ConnectedComponentPtr > & components );
    static void                         ApplyAlignement     ( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot, const std::wstring& text, std::vector< glm::vec3 > & layout, float boxWidth, int beginIdx, int endIdx );

    static float                        ComputeAlignement   ( TextAlignmentType tat, glm::vec3 & translate, glm::vec3 & translateDot );

    /////////////////////////////////////
    // Makes letters arrangment.
    static std::vector< glm::vec3 >     LayoutLetters       ( const std::wstring & text, TextRepresentationConstPtr textRepr, TextLayoutInfo & layout );

    /////////////////////////////////////
    // Other helpers
    static bool                         IsWhitespace        ( wchar_t character );
    static bool                         IsEnter             ( wchar_t character );

};


} // bv