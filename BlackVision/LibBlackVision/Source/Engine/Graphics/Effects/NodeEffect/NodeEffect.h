#pragma once

#include "Engine/Interfaces/IValue.h"

#include <set>

namespace bv {

enum class NodeEffectType : int
{
    NET_DEFAULT = 0,
    NET_ALPHA_MASK,
    NET_NODE_MASK,
    NET_IMAGE_MASK,
    NET_WIREFRAME,
    NET_MIX_CHANNELS,
    
    NET_LIGHT_SCATTERING,
    NET_SHADOW,
    NET_BLUR,

    NET_BOUNDING_BOX,

    NET_TOTAL
};


class RenderablePass;
class NodeEffectLogic;
class SceneNode;
class RenderLogicContext;
class ITextureDescriptor;
DEFINE_CONST_PTR_TYPE( ITextureDescriptor )

class NodeEffect
{
private:

    NodeEffectLogic *   m_logic;
    NodeEffectType      m_type;

public:

                    NodeEffect                  ( NodeEffectLogic * logic,  NodeEffectType nodeEffectType );
                    ~NodeEffect                 ();

    void            Update                      ();
    void            Render                      ( SceneNode * node, RenderLogicContext * ctx );

    //FIXME: remove when transition is finished - this doesn't seem to be required in this model
    NodeEffectType  GetType                     () const;

    unsigned int    GetNumValues                () const;
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    IValuePtr       GetValue                    ( const std::string & name ) const;

    void            AddTexture                  ( const ITextureDescriptorConstPtr & txDesc );


    void            GetRenderPasses             ( std::set< const RenderablePass * > * passes ) const;
};

DEFINE_PTR_TYPE(NodeEffect)
DEFINE_CONST_PTR_TYPE(NodeEffect)

} //bv
