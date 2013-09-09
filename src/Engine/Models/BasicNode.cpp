#include "BasicNode.h"
#include "Plugin.h"
#include "Engine/Graphics/SceneGraph/BasicScene.h"
#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Renderers/OGLRenderer/vborect.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
#include "System/Print.h"

#include "Engine\Graphics\Resources\VertexDescriptor.h"
#include "Engine\Graphics\Resources\VertexBuffer.h"
#include "Engine\Graphics\Resources\IndexBuffer.h"
#include "Engine\Graphics\Shaders\RenderableEffect.h"
#include "Engine\Models\Updaters\TransformUpdater.h"
#include "Engine\Models\Updaters\ShaderParamUpdater.h"

#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace bv {

namespace
{
    bool FileExists( const std::string& fileName )
    {
        struct stat info;
        int ret = -1;

        ret = stat(fileName.c_str(), &info);
        return 0 == ret;
    }

    bool ReadFile(std::ostream& out, const std::string& fileName)
    {    
        if( ! FileExists(fileName) )
        {
            return false;
        }

        std::ifstream inFile( fileName, std::ios::in );
        if( !inFile )
        {
            return false;
        }

        while( inFile.good() )
        {
            int c = inFile.get();
            if( ! inFile.eof() )
                out << (char) c;
        }

        inFile.close();

        return true;
    }
}

BasicNode::BasicNode()
    : m_gshaderPlugin(nullptr)
    , m_pshaderPlugin(nullptr)
    , m_vshaderPlugin(nullptr)
{
}

SceneNode* BasicNode::buildScene()
{
    VertexBuffer *      vb          = nullptr;
    IndexBuffer *       ib          = nullptr;
    VertexDescriptor *  vd          = nullptr;

    RenderableEntity *  renderEnt   = nullptr;
    RenderableEffect *  effect      = nullptr;

    CreateRenderableData( &vd, &vb, &ib );
    effect = CreateRenderaleEffectMockImplementationForCompleteDummies();
    RenderableEntity::RenderableType renderableType = GetRenderableType();

    //FIXME: to powinna ogarniac jakas faktoria-manufaktura
    switch( renderableType )
    {
        case RenderableEntity::RenderableType::RT_TRIANGLE_STRIP:
            renderEnt = new TriangleStrip( vd, vb, effect );
            break;
        case RenderableEntity::RenderableType::RT_TRIANGLES:
        case RenderableEntity::RenderableType::RT_TRIANGLE_MESH:
        default:
            return nullptr;
    }

    // TODO: dodac liste layerow do zwracanego SceneNode
    TransformUpdater* tu = new TransformUpdater(renderEnt, &m_transformPlugins);

    UpdatersManager::get().RegisterUpdater(tu);

    SceneNode* ret = new SceneNode(renderEnt);

    for( auto ch : m_children )
    {
        ret->AddChildNode(ch->buildScene());
    }

    return ret;


    //SceneNode* ret =  new SceneNode(nullptr);

    //if(m_pshaderPlugin)
    //{
    //    //ret->setShader(PixelShader::create(m_pshaderPlugin->GetShaderFile(), "", 1, 0)); // TODO: We do not need 4 params but 1.
    //    ret->addParamDesc(m_pshaderPlugin->getDescriptors());
    //}
    //else
    //    assert( false );
    //    //ret->setShader(VertexShader::create("../dep/media/shaders/basic.frag", "", 1, 0));

    //if(m_vshaderPlugin)
    //    ret->setShader(VertexShader::create(m_vshaderPlugin->GetShaderFile(), "", 1, 0));
    //else
    //    ret->setShader(VertexShader::create("../dep/media/shaders/solid.vert", "", 1, 0));


    //if(m_transformPlugins.empty())
        // TODO:
        //ret->addParamDesc(m_transformPlugin->getDescriptors());

    //if(m_geometryPlugins.empty())

    //if(!m_goemetryPlugin)
    //{
    //    ret->setGeometry(new VBORect(1.f, 1.f));
    //}
    //else
    //{
    //    // TODO:
    //}

    //ret->setInterpolatorsList(buildInterpolatorList());



    //return ret;
    return nullptr;
}

bool BasicNode::addChild(Node* n)
{
    m_children.push_back(n);

    return true;
}

void BasicNode::addTransformPlugin(ITransformPlugin* tPlugin)
{
    m_transformPlugins.push_back(tPlugin);
}

void BasicNode::addGeometryPlugin(IGeometryGenPlugin* gPlugin)
{
    m_geometryPlugins.push_back(gPlugin);
}

void BasicNode::setPixelShaderPlugin(IShaderPlugin* psPlugin)
{
    m_pshaderPlugin = psPlugin;
}

void BasicNode::setVertexShaderPlugin(IShaderPlugin* vsPlugin)
{
    m_vshaderPlugin = vsPlugin;
}

void BasicNode::setGeometryShaderPlugin(IShaderPlugin* gsPlugin)
{
    m_gshaderPlugin = gsPlugin;
}


void BasicNode::Print(std::ostream& out, int tabs) const
{
    out << "------------------NODE-------------------- : " << this << debug::EndLine(tabs);
    out << "Transform plugins: " << m_transformPlugins.size() << debug::EndLine(tabs + 1);
    for(auto tp : m_transformPlugins)
        tp->Print(out, tabs + 1);

    out << debug::EndLine(tabs) << "Geometry plugins: " << m_geometryPlugins.size();
    for(auto gp : m_geometryPlugins)
        gp->Print(out, tabs + 1);
    
    out << debug::EndLine(tabs) << "Pixel Shader plugin: " << m_pshaderPlugin << debug::EndLine(tabs + 1);
    if(m_pshaderPlugin != nullptr)
        m_pshaderPlugin->Print(out, tabs + 1);

    out << debug::EndLine(tabs) << "Vertex Shader plugin: " << m_vshaderPlugin << debug::EndLine(tabs + 1);
    if(m_vshaderPlugin != nullptr)
        m_vshaderPlugin->Print(out, tabs + 1);

    out << debug::EndLine(tabs) << "Geometry Shader plugin: " << m_gshaderPlugin << debug::EndLine(tabs + 1);
    if(m_gshaderPlugin != nullptr)
        m_gshaderPlugin->Print(out, tabs + 1);

    out << debug::EndLine(tabs) << "------------------PARENT OF---------------- : " << m_children.size() << debug::EndLine(tabs + 1);
    for(auto ch : m_children)
    {
        ch->Print(out, tabs + 1);
        out << debug::EndLine(tabs + 1);
    }
}

void BasicNode::Update(float t)
{
    for(auto tp : m_transformPlugins)
        tp->Update(t);

    for(auto gp : m_geometryPlugins)
        gp->Update(t);

    if(m_pshaderPlugin != nullptr)
        m_pshaderPlugin->Update(t);

    if(m_vshaderPlugin != nullptr)
        m_vshaderPlugin->Update(t);

    if(m_gshaderPlugin != nullptr)
        m_gshaderPlugin->Update(t);

    for(auto ch : m_children)
    {
        ch->Update(t);
    }
}

RenderableEntity::RenderableType    BasicNode::GetRenderableType        ()                                                              const
{
    // TODO:
    return (RenderableEntity::RenderableType)m_geometryPlugins.back()->AdjacencyType(); // FIXME: remove cast
}

bool                                BasicNode::CreateRenderableData     (VertexDescriptor** vd, VertexBuffer** vb, IndexBuffer** ib)    const
{
    *vd = VertexDescriptor::Create(1, VertexDescriptor::AttrType::AT_FLOAT3, VertexDescriptor::AttrSemantic::AS_POSITION, 0); // TODO: Sprawdz size i ustaw AttrType poprawnie

    auto& back = m_geometryPlugins.back();

    *vb = new VertexBuffer(back->Vertices().size(), back->VertexSize());

    for(auto& v : back->Vertices())
    {
        (*vb)->AddVertex(v);
    }

    if(back->AdjacencyType() != IGeometryGenPlugin::Adjacency::A_TRIANGLE_MESH)
    {
        *ib = nullptr;
    }
    else
    {
        assert(back->Indices().size() > 0);
        *ib = new IndexBuffer(back->Indices().size(), sizeof(int));

        for(auto& i : back->Indices())
        {
            (*ib)->AddIndex(i);
        }
    }

    //TODO: czas 0

    //(*vb)->AddVertex()// FORNE
    return true;
}

namespace
{

template<class ShaderType>
ShaderType* CreateShader(IShaderPlugin* pl)
{
    if(pl != nullptr)
    {
        std::stringstream code;

        ReadFile(code, pl->GetShaderFile());

        ShaderType * s = new ShaderType(code.str());

        BasicNode::RegisterShaderParameters(pl, s->GetOrCreateShaderParameters());
        
        s->RegisterUpdater(ShaderParamUpdater::Create(pl, s));

        return s;
    }
    else
    {
        // FIXME:
        return nullptr;
    }
}

} // anonymous

PixelShader*                        BasicNode::CreatePixelShader       ()                                                               const
{
    return CreateShader<PixelShader>(m_pshaderPlugin);
}

VertexShader*                       BasicNode::CreateVertexShader      ()                                                               const
{
    VertexShader * vs = CreateShader<VertexShader>(m_vshaderPlugin);

    return vs ? vs : new PassThroughVertexShader(new PassThroughVertexShaderParametersDescriptor());
}

GeometryShader*                     BasicNode::CreateGeometryShader    ()                                                               const
{
    return CreateShader<GeometryShader>(m_gshaderPlugin);
}

RenderableEffect*                   BasicNode::CreateRenderaleEffectMockImplementationForCompleteDummies   ()                                                              const
{
    RenderableEffect* ret = new RenderableEffect();

    RenderablePass* renderPass = new RenderablePass(CreatePixelShader(), CreateVertexShader(), CreateGeometryShader());

    ret->AddPass(renderPass);

    return ret;
}

void                                BasicNode::RegisterShaderParameters(IShaderPlugin* shaderPlugin, ShaderParameters * shParams)
{
    for(auto param : shaderPlugin->GetValuesList())
    {
        GenericShaderParam* genShaderParam = ShaderParamFactory::Get().Create( param->GetName(), param->GetParamType() );
        shParams->RegisterParameter(genShaderParam);
    }
}

} // bv
