#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"
#include "System/BasicTypes.h"

namespace bv {

class IValue;
class TransformableEntity;

class SceneNode
{
    typedef std::vector<TransformableEntity *>  TransformableEntityVec;
    typedef std::vector<SceneNode *>            SceneNodeVec;

private:

    TransformableEntityVec  m_transformables;
    SceneNodeVec            m_sceneNodes;

    TransformableEntity *   m_transformRep;

public:

                            SceneNode           ( TransformableEntity * transformRep = nullptr );
                            ~SceneNode          ();

    SizeType                NumChildrenNodes    () const;
    SizeType                NumTransformables   () const;

    void                    AddChildNode        ( SceneNode * child );
    void                    AddTransformable    ( TransformableEntity * transformable );

    SceneNode *             GetChild            ( int idx );
    TransformableEntity *   GetTransformable    ( int idx );
    TransformableEntity *   GetAnchor           ();

    void                    RegisterTransformRep( TransformableEntity * transformable );


//  void                    Update              ( double t, const Transform & parentTransform ); 
    void                    Update              ( const std::vector< Transform > & parentTransforms );

    bool                    IsVisible           () const;
    void                    SetVisible          ( bool visible );

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

};

} // bv
