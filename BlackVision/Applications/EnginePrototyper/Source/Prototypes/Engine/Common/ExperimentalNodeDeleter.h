#pragma once


namespace bv
{

class SceneNode;
class Renderer;
class RenderableEntity;

void    ExperimentalDeleteAndCleanup( SceneNode * node, Renderer * renderer );
void    ExperimentalDeleteSingleNode( SceneNode * node, Renderer * renderer );
void    ExperimentalFreePdrResources( RenderableEntity * renderable, Renderer * renderer );

} //bv
