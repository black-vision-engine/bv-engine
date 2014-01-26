#pragma once

#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

namespace bv { namespace model {

class TextHelper
{
public:
    /////////////////////////////////////
    // This function loads font definition from file. Builds font atlases with the specified size.
    // Returns ResourceHandle of this font
    static const ResourceHandle *       LoadFont            ( const std::string& fontFileName, size_t size );

    /////////////////////////////////////
    // This function creates TextureInfo from atlas specified atlas ind fontReasource.
    // Returns new TextureInfo with texture name specified in texName.
    static const ResourceHandle *       GetAtlasTextureInfo ( const ResourceHandle * fontResource, bool bolded = false, bool italic = false );

    /////////////////////////////////////
    // This function creates empty VertexAttributeChannel for text with proper attribute descriptor. ( position, texture cord )
    static VertexAttributesChannel*     CreateVACForText    ();

    /////////////////////////////////////
    // This function builds VertexAttributeChannel for text and fontResource.
    static void                         BuildVACForText     ( VertexAttributesChannel* vertexAttributeChannel, const ResourceHandle * fontResource, const std::wstring& text, bool bolded = false, bool italic = false );
};


} // model
} // bv