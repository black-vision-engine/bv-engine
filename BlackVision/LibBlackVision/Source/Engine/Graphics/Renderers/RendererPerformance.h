#pragma once

#include <hash_map>




namespace bv
{

class RenderableEntity;
class PdrTimeQuery;
class SceneNode;


class RendererPerformance
{
private:
    typedef std::hash_map< const RenderableEntity *, PdrTimeQuery * >       PdrTimeQueryMap;

    PdrTimeQueryMap         m_timeQueryMap;

public:
    void        FreeAllPDResources          ( RenderableEntity * renderable );

    void        BeginGPUQuery               ( SceneNode * sceneNode );
    void        EndGPUQuery                 ( SceneNode * sceneNode );

    void        QueryPreviousGPUResult      ( SceneNode * sceneNode );

private:

    PdrTimeQuery *      GetTimeQuery        ( RenderableEntity * renderable );
};


}	// bv


