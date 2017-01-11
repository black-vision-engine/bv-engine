#pragma once

#include "CoreDEF.h"

#include "Engine/Interfaces/IValue.h"


namespace bv {

class SceneNodeRepr;

namespace nrl 
{

enum class NNodeEffectType : int
{
    NNET_DEFAULT = 0,
    NNET_ALPHA_MASK,
    NNET_NODE_MASK,
    NNET_WIREFRAME,
    NNET_MIX_CHANNELS,
	NNET_BLUR,

    NNET_TOTAL
};

class NRenderContext;

class NNodeEffect
{
private:

    NNodeEffectType     m_type;

protected:
    
                        NNodeEffect     ( NNodeEffectType type );

public:

    virtual             ~NNodeEffect    ();

    virtual void        Render          ( SceneNodeRepr * node, NRenderContext * ctx ) = 0;

    NNodeEffectType     GetType         () const;

    virtual IValuePtr   GetValue        ( const std::string & name ) const = 0;

};

DEFINE_PTR_TYPE(NNodeEffect)
DEFINE_CONST_PTR_TYPE(NNodeEffect)

} // nrl
} // bv
