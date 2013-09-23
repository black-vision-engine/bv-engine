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
#include "Engine\Graphics\Resources\VertexArray.h"
#include "Engine\Graphics\Resources\IndexBuffer.h"
#include "Engine\Graphics\Shaders\RenderableEffect.h"
#include "Engine\Models\Updaters\TransformUpdater.h"
#include "Engine\Models\Updaters\ShaderParamUpdater.h"

#include "Engine\Models\Plugins\Interfaces\IGeometryChannel.h"
#include "Engine\Models\Plugins\Interfaces\IConnectedComponent.h"
#include "Engine\Models\Plugins\Interfaces\IVertexAttributeChannel.h"

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

SceneNode* BasicNode::BuildScene()
{
    VertexBuffer *      vb          = nullptr;
    IndexBuffer *       ib          = nullptr;
    VertexDescriptor *  vd          = nullptr;
    VertexArray *       vao         = nullptr;

    RenderableEntity *  renderEnt   = nullptr;
    RenderableEffect *  effect      = nullptr;

    CreateRenderableData( &vao ); // TODO: Powinno zwracac indeksy albo vao w zaleznosci od rodzaju geometrii
    effect = CreateRenderaleEffectMockImplementationForCompleteDummies();
    auto renderableType = GetRenderableType();

    //FIXME: to powinna ogarniac jakas faktoria-manufaktura
    switch( renderableType )
    {
        case PrimitiveType::PT_TRIANGLE_STRIP:
            renderEnt = new TriangleStrip( vao, effect );
            break;
        case PrimitiveType::PT_TRIANGLES:
        case PrimitiveType::PT_TRIANGLE_MESH:
        default:
            return nullptr;
    }

    // TODO: dodac liste layerow do zwracanego SceneNode
    TransformUpdater* tu = new TransformUpdater(renderEnt, &m_transformPlugins);

    UpdatersManager::get().RegisterUpdater(tu);

    SceneNode* ret = new SceneNode(renderEnt);

    for( auto ch : m_children )
    {
        ret->AddChildNode(ch->BuildScene());
    }

    return ret;
}

bool BasicNode::AddChild(Node* n)
{
    m_children.push_back(n);

    return true;
}

void BasicNode::AddPlugin               ( IPlugin* plugin )
{
    m_plugins.push_back(plugin);
}

void BasicNode::addTransformPlugin(ITransformPlugin* tPlugin)
{
    m_transformPlugins.push_back(tPlugin);
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

    for(auto pl : m_plugins)
        pl->Update(t);

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

PrimitiveType                       BasicNode::GetRenderableType        ()                        const
{
    if( m_plugins.empty() )
    {
        assert(false);
    }

    return m_plugins.back()->GetGeometryChannel()->GetPrimitiveType();
}

bool                                BasicNode::CreateRenderableData     (VertexArray ** vao)    const
{
    if( m_plugins.empty() )
    {
        return false;
    }

    auto components = m_plugins.back()->GetGeometryChannel()->GetComponents();

    if( components.empty() )
    {
        return nullptr;
    }

    auto lastComponent = components.back();

    auto vertNum    = lastComponent->GetNumVertices();

    auto attribChannels = lastComponent->GetVertexAttributeChannels();

    if( attribChannels.empty() )
    {
        return nullptr;
    }

    *vao = new VertexArray();

    for( auto attrCh : attribChannels )
    {
        //FIXME: SUUUUUX
        //auto type       = attrCh->GetType();
        //auto semantic   = attrCh->GetSemantic();

        //VertexDescriptor*   vd = VertexDescriptor::Create( 1, type, semantic, (int)semantic);
        //VertexBuffer*       vb = new VertexBuffer( vertNum, attrCh->GetEntrySize() );

        //(*vao)->AddEntry( vb, vd );
    }

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
