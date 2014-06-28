#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"


namespace bv {

class IValue;
class TransformableEntity;

class SceneNode
{
    typedef std::vector<TransformableEntity *>  TransformableEntityVec;
    typedef std::vector<SceneNode *>            SceneNodeVec;

private:

    bool                    m_visible;
    bool                    m_overridenState;

    const IValue *          m_overrideAlphaVal;

    TransformableEntityVec  m_transformables;
    SceneNodeVec            m_sceneNodes;

    TransformableEntity *   m_transformRep;

public:

                            SceneNode           ( TransformableEntity * transformRep = nullptr );
                            ~SceneNode          ();

    int                     NumChildrenNodes    () const;
    int                     NumTransformables   () const;

    void                    AddChildNode        ( SceneNode * child );
    void                    AddTransformable    ( TransformableEntity * transformable );

    SceneNode *             GetChild            ( int idx );
    TransformableEntity *   GetTransformable    ( int idx );
    TransformableEntity *   GetAnchor           ();

    void                    RegisterTransformRep( TransformableEntity * transformable );

    void                    SetOverrideAlpha    ( const IValue * val );
    const IValue *          GetOverrideAlpha    () const;

//    void                    Update              ( double t, const Transform & parentTransform ); 
    void                    Update              ( const std::vector< Transform > & parentTransforms );

    bool                    IsVisible           () const;
    void                    SetVisible          ( bool visible );

    bool                    IsOverriden         () const;
    void                    SetOverriden        ( bool overriden );

};

}

//#include "Engine/Models/IPlugin.h"
//#include "Mathematics/Interpolators/Interpolators.h"
//#include "Mathematics/Transform/Transform.h"
//
//
//namespace bv
//{
//class Renderer;
//class ShaderParameter;
//class PixelShader;
//class VertexShader;
//class VBORect;
//class Camera;
//
//class InterpolatorsList
//{
//    typedef std::pair<BaseInterpolator*, char*> InterpolatorMemPair;
//
//    std::vector<InterpolatorMemPair> interpolators;
//
//public:
//    void update(float t);
//
//    void addInterpolator(BaseInterpolator* inter, char* mem = nullptr);
//
//    void setInterpolatorData(int i, char*);
//
//    int size() const { return interpolators.size(); }
//
//    explicit InterpolatorsList(){  }
//};
//
//class SceneNode;
//
//class ShaderData
//{
//private:
//    std::vector<ShaderParameter*>   shadersParameters;
//    PixelShader*                    pshader;
//    VertexShader*                   vshader;
//
//public:
//    explicit ShaderData() {}
//
//    friend class SceneNode;
//    friend class ShaderDataCmp;
//};
//
//
//class DataConnector
//{
//    char* m_data;
//public:
//    ParametersDescriptors           m_paramDesc;
//    std::vector<ShaderParameter*>   m_shaderParams;
//    InterpolatorsList*              m_interpolatorList;
//
//    void initDataPointers();
//
//    virtual ~DataConnector(){}
//};
//
//
//class SceneNode
//{
//private:
//
//    std::vector<SceneNode*>         m_children;
//    ShaderData*                     m_shData;
//    VBORect*                        m_geometry;
//    Transform                       m_localTransform;
//
//    DataConnector                   m_dataConnector;
//
//public:
//    explicit SceneNode(SceneNode* parent);
//    void addChild(SceneNode* ch);
//
//    //void addShaderParameter(ShaderParameter*);
//    void setShader(PixelShader*);
//    void setShader(VertexShader*);
//    void setGeometry(VBORect*);
//    void setLocalTransform(const Transform& trans);
//
//    void addParamDesc(const ParametersDescriptors& desc);
//    void setInterpolatorsList(InterpolatorsList* interList);
//
//    void init();
//    void update(float t);
//    void render( Renderer* renderer, const Camera& camera, const Transform& worldTransform);
//};
//
//} // bv
