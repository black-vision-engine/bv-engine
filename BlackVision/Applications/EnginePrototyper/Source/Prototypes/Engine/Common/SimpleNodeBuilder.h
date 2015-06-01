#pragma once

#include "Engine/Graphics/Effects/DefaultEffect.h"
#include "Engine/Graphics/Types/Transform.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexArray.h"

#include "Prototypes/Engine/Common/ShaderDataSourceCreator.h"


namespace bv {

typedef std::vector< Transform >    TTransformVec;

class SimpleNodeBuilder
{
public:

    static SceneNode *          CreateRectNodeSolidColor( float w, float h, float z );
    static SceneNode *          CreateRectNodeTexture   ( float w, float h, float z, const std::string & textureFile );

protected:

    static RenderableEffectPtr  CreateRenderableEffect  ( ShaderDataSourceType sdst, const std::string & textureFile = "", const std::string & textureName = "" );
    static RenderableEntity *   CreateRenderableTriStrip( RenderableArrayDataArraysSingleVertexBuffer * vaobuf, RenderableEffectPtr effect );
    static TTransformVec        DefaultTransform        ();

};

} // bv
