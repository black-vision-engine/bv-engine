#pragma once


namespace bv {

class Renderer;
class SceneNode;
class UniformBuffer;
class RenderableEntity;

class Scene
{
private:

    SceneNode *             m_root;

    UniformBuffer *         m_lightsBuffer;
    
    // @todo Maybe it should move to container class for engine grid lines.
    // In future when other helper object will apear in BV, we should implement
    // genering solution for handling it.
    RenderableEntity *      m_gridLines;
    bool                    m_gridLinesVisible;

public:

                            Scene               ();
                            ~Scene              ();

public:

    SceneNode *             GetRoot             () const;
    void                    SetRoot             ( SceneNode * node );

    UniformBuffer *         GetLightsBuffer     () const;

    RenderableEntity *      GetGridLines            () const;
    void                    SetGridLinesRenderable  ( RenderableEntity * renderable );
    void                    SetGridLinesVisible     ( bool visibility );
    bool                    GetGridLinesVisibility  ()  { return m_gridLinesVisible; }

};

DEFINE_PTR_TYPE( Scene );
DEFINE_CONST_PTR_TYPE( Scene );

typedef std::vector< Scene * > SceneVec;

} // bv
