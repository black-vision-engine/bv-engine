#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv
{

class SceneNode;
class Renderer;
class RenderableEntity;
class Shader;

void    ExperimentalDeleteAndCleanup( SceneNode * node, Renderer * renderer );
void    ExperimentalDeleteSingleNode( SceneNode * node, Renderer * renderer );
void    ExperimentalFreePdrResources( RenderableEntity * renderable, Renderer * renderer );
void    ExperimentalDelOursPdrEffect( RenderableEffectPtr effect, Renderer * renderer );
void    ExperimentalDeleteShaderPdr ( Shader * shader, Renderer * renderer );

} //bv
