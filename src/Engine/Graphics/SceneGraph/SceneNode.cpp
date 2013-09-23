#include "SceneNode.h"
#include <cassert>

namespace bv {

// ********************************
//
SceneNode::SceneNode           ( TransformableEntity * transformRep )
{
    RegisterTransformRep( transformRep );
}

// ********************************
//
SceneNode::~SceneNode          ()
{
    m_transformRep = nullptr; //this one is not owned here

    for ( auto transformable : m_transformables )
    {
        delete transformable;
    }

    for ( auto node : m_sceneNodes )
    {
        delete node;
    }
}

// ********************************
//
int                     SceneNode::NumChildrenNodes    () const
{
    return m_sceneNodes.size();
}

// ********************************
//
int                     SceneNode::NumTransformables   () const
{
    return m_transformables.size();
}

// ********************************
//
void                    SceneNode::AddChildNode        ( SceneNode * child )
{
    m_sceneNodes.push_back( child );
}

// ********************************
//
void                    SceneNode::AddTransformable    ( TransformableEntity * transformable )
{
    m_transformables.push_back( transformable );
}

// ********************************
//
SceneNode *             SceneNode::GetChild            ( int idx )
{
    assert( idx >= 0 );
    assert( idx < NumChildrenNodes() );

    return m_sceneNodes[ idx ];
}

// ********************************
//
TransformableEntity *   SceneNode::GetTransformable    ( int idx )
{
    assert( idx >= 0 );
    assert( idx < NumTransformables() );

    return m_transformables[ idx ];
}

// ********************************
//
TransformableEntity *   SceneNode::GetAnchor           ()
{
    return m_transformRep;
}


// ********************************
//
void            SceneNode::RegisterTransformRep( TransformableEntity * transformable )
{
    m_transformRep = transformable;
}

// ********************************
//
void            SceneNode::Update              ( double t, const std::vector< Transform > & parentTransforms )
{
    const std::vector< Transform > * pWorldTransforms = &parentTransforms;

    if( m_transformRep )
    {
        m_transformRep->UpdateTransforms( t, *pWorldTransforms );

        pWorldTransforms = &m_transformRep->WorldTransforms();
    }

    const std::vector< Transform > & worldTransforms = *pWorldTransforms;

    for ( auto transformable : m_transformables )
    {
        transformable->UpdateTransforms( t, worldTransforms );
    }

    for ( auto node : m_sceneNodes )
    {
        node->Update( t, worldTransforms );
    }
}

}

//#include "Camera.h"
//
//#include "Engine/Graphics/Renderers/OGLRenderer/glslprogram.h"
//#include "Engine/Graphics/Renderers/OGLRenderer/vborect.h"
//#include "Engine/Graphics/Shaders/Parameters.h"
//
//namespace bv
//{
//
//SceneNode::SceneNode(SceneNode* parent)
//{
//    m_shData = new ShaderData();
//
//    if(!parent)
//        return;
//
//    parent->addChild(this);
//}
//
//void SceneNode::addChild(SceneNode* ch)
//{
//    m_children.push_back(ch);
//}
//
//void SceneNode::setShader(PixelShader* ps)
//{
//    m_shData->pshader = ps;
//}
//
//void SceneNode::setShader(VertexShader* vs)
//{
//    m_shData->vshader = vs;
//}
//
//void SceneNode::setGeometry(VBORect* geo)
//{
//    m_geometry = geo;
//}
//
//void SceneNode::setLocalTransform(const Transform& trans)
//{
//    m_localTransform = trans;
//}
//
//void SceneNode::addParamDesc(const ParametersDescriptors& desc)
//{
//    m_dataConnector.m_paramDesc.merge(desc);
//}
//
//void SceneNode::setInterpolatorsList(InterpolatorsList* interList)
//{
//    m_dataConnector.m_interpolatorList = interList;
//}
//
//void SceneNode::init()
//{
//    m_dataConnector.initDataPointers();
//
//    for(auto ch : m_children)
//    {
//        ch->init();
//    }
//}
//
//void SceneNode::update(float t)
//{
//    m_dataConnector.m_interpolatorList->update(t);
//
//    for(auto ch : m_children)
//    {
//        ch->update(t);
//    }
//}
//
//void SceneNode::render(Renderer* renderer, const Camera& camera, const Transform& worldTransform)
//{
//    GLSLProgram* prog = GLSLPrograms::get().getProgram(m_shData->pshader, m_shData->vshader);
//
//    prog->Use();
//
//    for(auto param : m_dataConnector.m_shaderParams)
//    {
//        param->update(prog);
//
//        if( param->getSemantic() == ParameterSemantic::LOCAL_TRANSFORM )
//        {
//            assert( param->getName() == "MVP" );
//
//            glm::mat4* localTrans = reinterpret_cast<glm::mat4*>(param->getLocalBuffer());
//
//            *localTrans = *localTrans * camera.getViewProjectionMatrix();
//        }
//    }
//
//    m_geometry->Render();
//
//    for(auto ch : m_children)
//    {
//        ch->render(renderer, camera, worldTransform * m_localTransform);
//    }
//}
//
//namespace
//{
//    int getSize(TypeKind tk)
//    {
//        switch(tk)
//        {
//        case TypeKind::VEC4:
//            return sizeof(glm::vec4);
//        case TypeKind::MAT4:
//            return sizeof(glm::mat4);
//        default:
//            //TODO:Implement
//            assert(false);
//            return 0;
//        }
//    }
//
//    ShaderParameter* createShaderParam(const std::string& name, char* data, TypeKind type, ParameterSemantic semantic)
//    {
//        switch(type)
//        {
//        case TypeKind::VEC4:
//            return new ShaderParameterVec4(name, data, semantic);
//        case TypeKind::MAT4:
//            return new ShaderParameterMat4(name, data, semantic);
//        default:
//            //TODO:Implement
//            assert(false);
//            return nullptr;
//        }
//    }
//}
//
//void DataConnector::initDataPointers()
//{
//    m_data = nullptr;
//    int toAlloc = 0;
//    for(int i = 0; i <  m_paramDesc.size(); ++i)
//    {
//        toAlloc += getSize(m_paramDesc.getParam(i).getType());
//    }
//
//    m_data = new char[toAlloc];
//
//    int offset = 0;
//    for(int i = 0; i <  m_interpolatorList->size(); ++i)
//    {
//        char* memAddr = m_data + offset;
//        m_interpolatorList->setInterpolatorData(i, memAddr);
//        m_shaderParams.push_back(createShaderParam(m_paramDesc.getParam(i).getName(), memAddr, m_paramDesc.getParam(i).getType(), m_paramDesc.getParam(i).getSemantic()));
//        offset += getSize(m_paramDesc.getParam(i).getType());
//    }
//}
//
//void InterpolatorsList::addInterpolator(BaseInterpolator* inter, char* mem)
//{
//    interpolators.push_back(std::make_pair(inter, mem));
//}
//
//void InterpolatorsList::setInterpolatorData(int i, char* data)
//{
//    assert(i >= 0 && i < size());
//
//    interpolators[i].second = data;
//}
//
//void InterpolatorsList::update(float t)
//{
//    for(auto i : interpolators)
//    {
//        i.first->evalToCBuffer(t, i.second);
//    }
//}
//
//} // bv
