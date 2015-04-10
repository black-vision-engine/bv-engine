#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"
#include "CoreDEF.h"

namespace bv {

class IValue;
class TransformableEntity;

class SceneNode
{
    typedef std::vector<SceneNode *>            SceneNodeVec;

private:

    SceneNodeVec            m_sceneNodes;

    TransformableEntity *   m_transformable;

public:

                            SceneNode           ( TransformableEntity * transformable = nullptr );
                            ~SceneNode          ();

    SizeType                NumChildNodes       () const;

    void                    AddChildNode        ( SceneNode * child );
    SceneNode *             DetachChildNode     ( unsigned int idx );

    SceneNode *             GetChild            ( unsigned int idx );
    TransformableEntity *   GetTransformable    ();

    void                    Update              ( const std::vector< Transform > & parentTransforms );

    bool                    IsVisible           () const;
    void                    SetVisible          ( bool visible );
    

// ***********************************************************************************************
//                              HACKISH GLOBAL EFFECT INTERFACE
// ***********************************************************************************************
//FIXME: add some kind of global effect here
//FIXME: instead of IsOverriden, SetOverriden and so on simplu use Get/Set GlobalEffect which is applied to the result
private:

    bool                    m_visible;

    const IValue *          m_overrideAlphaVal;

    bool                    m_overridenStateAlphaMask;
    bool                    m_overridenStateNodeMask;

public:

    void                    SetOverrideAlphaVal ( const IValue * val );
    const IValue *          GetOverrideAlphaVal () const;

    bool                    IsOverridenAM       () const;
    bool                    IsOverridenNM       () const;

    void                    SetOverridenAM      ( bool overriden );
    void                    SetOverridenNM      ( bool overriden );

// ***********************************************************************************************
//                          END OF HACKISH GLOBAL EFFECT INTERFACE
// ***********************************************************************************************
};

} // bv
