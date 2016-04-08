#pragma once


namespace bv {

class Renderer;
class SceneNode;
class UniformBuffer;


class Scene
{
private:

    SceneNode *             m_root;

    UniformBuffer *         m_lightsBuffer;

public:

                            Scene               ();
                            ~Scene              ();

public:

    SceneNode *             GetRoot             () const;
    void                    SetRoot             ( SceneNode * node );

    UniformBuffer *         GetLightsBuffer     () const;

};

DEFINE_PTR_TYPE( Scene );
DEFINE_CONST_PTR_TYPE( Scene );

typedef std::vector< Scene * > SceneVec;

} // bv
