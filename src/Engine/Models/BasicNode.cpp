#include "BasicNode.h"

#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "System/Print.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Graphics/SceneGraph/BasicScene.h"
#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Renderers/OGLRenderer/vborect.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Models/Plugins/Interfaces/IGeometryChannelDescriptor.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/RenderableArrayDataElements.h"

#include "Engine/Graphics/Resources/VertexDescriptor.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/IndexBuffer.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Models/Updaters/GeometryUpdater.h"
#include "Engine/Models/Updaters/TransformUpdater.h"
#include "Engine/Models/Updaters/RendererStateUpdater.h"
#include "Engine/Models/Updaters/ShaderParamUpdater.h"

#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannelDescriptor.h"
#include "Engine/Graphics/Resources/Textures/TextureManager.h"
#include "Engine/Graphics/Resources/Texture2D.h"

namespace bv { namespace model {

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

// ********************************
//
BasicNode::BasicNode()
{}

// ********************************
//
SceneNode*                  BasicNode::BuildScene()
{
    auto renderableType = GetRenderableType();

    RenderableEffect *  effect      = CreateRenderaleEffectMockImplementationForCompleteDummies();
    RenderableEntity *  renderEnt   = nullptr;
    //RenderableArrayDataSingleVertexBuffer * rad = CreateRenderableArrayData( renderableType );


    //CreateRenderableData( &vao ); // TODO: Powinno zwracac indeksy albo vao w zaleznosci od rodzaju geometrii
    //effect = ;


    //FIXME: to powinna ogarniac jakas faktoria-manufaktura
    switch( renderableType )
    {
        case PrimitiveType::PT_TRIANGLE_STRIP:
        {
            //FIXME: it should be constructed as a proper type RenderableArrayDataArraysSingleVertexBuffer * in the first place
            //FIXME: this long type name suggests that something wrong is happening here (easier to name design required)
            RenderableArrayDataArraysSingleVertexBuffer * radasvb = CreateRenderableArrayDataTriStrip();
            renderEnt = new TriangleStrip( radasvb, effect );
            break;
        }
        case PrimitiveType::PT_TRIANGLES:
        case PrimitiveType::PT_TRIANGLE_MESH:
            assert( false );
        default:
            return nullptr;
    }

    bv::Transform worldTrans;

    std::vector< bv::Transform > trans;
    trans.push_back( worldTrans );

    renderEnt->SetWorldTransforms( trans );

    // TODO: dodac liste layerow do zwracanego SceneNode
    for( auto p : m_plugins )
    {
        int i = 0;
        for( auto tex : p->GetTextures() )
        {
            SamplerWrappingMode wp[] = { SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT }; 
            auto textureSampler = new TextureSampler( i, tex->m_texName, bv::SamplerSamplingMode::SSM_MODE_2D, SamplerFilteringMode::SFM_LINEAR, wp, glm::vec4( 0.f, 0.f, 1.f, 0.f ));
            effect->GetPass( 0 )->GetPixelShader()->AddTextureSampler( textureSampler );

            auto loadedTex = bv::GTextureManager.LoadTexture( tex->m_resHandle, false );

            effect->GetPass( 0 )->GetPixelShader()->Parameters()->AddTexture( loadedTex );

            i++;
        }

        //FIXME: Only last plugin should be used here as its output corresponds to the final transformation (list)
        //FIXME: Updater only sets proper local and world matrices for the geometry and all model transformations have already been updated at this point

        if ( p == m_plugins.back() )
        {
            UpdatersManager & updatersManager = UpdatersManager::get();

            auto transChannel = p->GetTransformChannel();        
            auto geomChannel = p->GetGeometryChannel();
            
            assert( transChannel != nullptr );
            assert( geomChannel != nullptr );

            TransformUpdater * transformUpdater = new TransformUpdater( renderEnt, transChannel );
            updatersManager.RegisterUpdater( transformUpdater );

            if ( !geomChannel->IsTimeInvariant() )
            {
                GeometryUpdater * geometryUpdater = new GeometryUpdater( renderEnt, geomChannel );
                updatersManager.RegisterUpdater( geometryUpdater );
            }

            auto psc = p->GetPixelShaderChannel();
            auto renderCtx = psc->GetRendererContext();

            assert( renderCtx );

            for( int i = 0; i < effect->NumPasses(); ++i )
            {
                auto inst = effect->GetPass( i )->GetStateInstance();

                assert( !inst->GetAlphaState() );
                assert( !inst->GetCullState() );
                assert( !inst->GetDepthState() );
                assert( !inst->GetFillState() );
                assert( !inst->GetOffsetState() );
                assert( !inst->GetStencilState() );

                RendererStatesBuilder::Create( inst, renderCtx );

                RenderStateUpdater * rendererStateUpdater = new RenderStateUpdater( inst, renderCtx );
                updatersManager.RegisterUpdater( rendererStateUpdater );
            }
        }
    }

    SceneNode* ret = new SceneNode(renderEnt);

    for( auto ch : m_children )
    {
        ret->AddChildNode(ch->BuildScene());
    }

    return ret;
}

// ********************************
//
void            BasicNode::AddChild                 ( Node* n )
{
    m_children.push_back( n );
}

// ********************************
//
void            BasicNode::AddLayer                 ( Node* n )
{
    m_layers.push_back( n );
}

// ********************************
//
void            BasicNode::AddPlugin                ( IPlugin* plugin )
{
    m_plugins.push_back(plugin);
}

// ********************************
//
void BasicNode::Update(float t)
{
    for(auto l : m_layers)
        l->Update( t );

    for(auto pl : m_plugins)
        pl->Update( t );

    for(auto ch : m_children)
        ch->Update( t );
}

// ********************************
//
PrimitiveType                       BasicNode::GetRenderableType        ()                        const
{
    if( m_plugins.empty() )
    {
        assert(false);
    }

    return m_plugins.back()->GetGeometryChannel()->GetPrimitiveType();
}

// ********************************
//
bool                                BasicNode::CreateRenderableData     (/* VertexArray ** vao*/ ) const
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

    VertexArray ** vao = {nullptr};
    assert( false );
    *vao = new VertexArray();

    int channelLoc = 0;

    for( auto attrCh : attribChannels )
    {
        auto desc       = attrCh->GetDescriptor();
        
        VertexDescriptor*   vd = VertexDescriptor::Create( 1, channelLoc++, desc->GetType(), desc->GetSemantic(), (int)desc->GetSemantic());
        VertexBuffer*       vb = new VertexBuffer( vertNum, desc->GetEntrySize() );

        vb->WriteToBuffer( attrCh->GetData(), attrCh->GetNumEntries() * desc->GetEntrySize() );

        //(*vao)->AddEntry( vb, vd );
    }

    return true;
}

namespace
{


const IVertexShaderChannel* GetVertexShaderChannel( const std::vector< IPlugin* >& plugins )
{
    if( !plugins.empty() )
        return plugins.back()->GetVertexShaderChannel();

    return nullptr;
}

const IPixelShaderChannel* GetPixelShaderChannel( const std::vector< IPlugin* >& plugins )
{
    if( !plugins.empty() )
        return plugins.back()->GetPixelShaderChannel();

    return nullptr;
}

const IGeometryShaderChannel* GetGeometryShaderChannel( const std::vector< IPlugin* >& plugins )
{
    if( !plugins.empty() )
        return plugins.back()->GetGeometryShaderChannel();

    return nullptr;
}

} // anonymous

// ********************************
//
PixelShader*                        BasicNode::CreatePixelShader       ()                                                               const
{
    auto psCh = GetPixelShaderChannel( m_plugins );

    if(psCh != nullptr)
    {
        std::stringstream code;

        ReadFile(code, psCh->GetShaderFile());

        PixelShader * s = new PixelShader(code.str());

        BasicNode::RegisterShaderParameters(psCh, s->GetOrCreateShaderParameters());
        
        s->RegisterUpdater(ShaderParamUpdater::Create(psCh, s));

        return s;
    }
    else
    {
        // FIXME:
        return nullptr;
    }
}

// ********************************
//
VertexShader*                       BasicNode::CreateVertexShader      ()                                                               const
{
    auto vsCh = GetVertexShaderChannel( m_plugins );

    if(vsCh != nullptr)
    {
        std::stringstream code;

        ReadFile(code, vsCh->GetShaderFile());

        VertexShader * s = new VertexShader(code.str());

        BasicNode::RegisterShaderParameters(vsCh, s->GetOrCreateShaderParameters());
        
        s->RegisterUpdater(ShaderParamUpdater::Create(vsCh, s));

        return s;
    }
    else
    {
        return new PassThroughVertexShader(new PassThroughVertexShaderParametersDescriptor());
    }
}

// ********************************
//
GeometryShader*                     BasicNode::CreateGeometryShader    ()                                                               const
{
    auto gsCh = GetGeometryShaderChannel( m_plugins );

    if(gsCh != nullptr)
    {
        std::stringstream code;

        ReadFile(code, gsCh->GetShaderFile());

        GeometryShader * s = new GeometryShader(code.str());

        BasicNode::RegisterShaderParameters(gsCh, s->GetOrCreateShaderParameters());
        
        s->RegisterUpdater(ShaderParamUpdater::Create(gsCh, s));

        return s;
    }
    else
    {
        return nullptr;
    }
}

// ********************************
//FIXME: reimplement someday
RenderableEffect *                  BasicNode::CreateRenderaleEffectMockImplementationForCompleteDummies   ()                                                              const
{
    RenderableEffect *  ret = new RenderableEffect();

    RenderablePass *    renderPass = new RenderablePass(CreatePixelShader(), CreateVertexShader(), CreateGeometryShader());

    ret->AddPass(renderPass);

    return ret;
}

// ********************************
//
RenderableArrayDataSingleVertexBuffer * BasicNode::CreateRenderableArrayData( PrimitiveType type ) const
{
    if( m_plugins.empty() )
    {
        return nullptr;
    }

    auto geometryChannel = m_plugins.back()->GetGeometryChannel();

    auto components = geometryChannel->GetComponents();
    auto geomDesc = geometryChannel->GetDescriptor();

    if( components.empty() )
    {
        return nullptr;
    }

    switch( type )
    {
        case PrimitiveType::PT_TRIANGLE_STRIP:
            return CreateRenderableArrayDataArrays( components, geomDesc, geometryChannel->IsTimeInvariant() );
        case PrimitiveType::PT_TRIANGLES:
        case PrimitiveType::PT_TRIANGLE_MESH:
            //FIXME: implement
            assert( false );
        default:
            return nullptr;
    }
}

// ********************************
//
RenderableArrayDataArraysSingleVertexBuffer *   BasicNode::CreateRenderableArrayDataTriStrip   () const
{
    if( m_plugins.empty() )
    {
        return nullptr;
    }

    auto geometryChannel = m_plugins.back()->GetGeometryChannel();

    auto components = geometryChannel->GetComponents();
    auto geomDesc = geometryChannel->GetDescriptor();

    if( components.empty() )
    {
        return nullptr;
    }
    
    return CreateRenderableArrayDataArrays( components, geomDesc, geometryChannel->IsTimeInvariant() );
}

// ********************************
//
RenderableArrayDataArraysSingleVertexBuffer * BasicNode::CreateRenderableArrayDataArrays( const std::vector< IConnectedComponent * > & ccVec, const IGeometryChannelDescriptor * desc, bool isTimeInvariant ) const
{
    //FIXME: a bit of hackery because memory layout may be different than what this constructor suggests (this time it is not)
    //FIXME: this code should be moved to some utility classes from this poor BasicNode (not so basic right now)
    //FIXME: check that plugin's channel signature is the same for all connected components

    DataBuffer::Semantic vbSemantic = DataBuffer::Semantic::S_STATIC;

    if ( !isTimeInvariant )
    {
        vbSemantic = DataBuffer::Semantic::S_DYNAMIC;
    }

    VertexBuffer * vertexBuffer         = new VertexBuffer( TotalNumVertices( ccVec ), desc->SingleVertexEntrySize(), vbSemantic );
    VertexDescriptor * vertexDescriptor = CreateVertexDescriptor( desc );

    VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vertexBuffer, vertexDescriptor );
    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    char * vbData = vertexBuffer->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P

    unsigned int currentOffset = 0;

    for( auto cc : ccVec )
    {
        assert( !cc->GetVertexAttributeChannels().empty() );

        vao->AddCCEntry( cc->GetNumVertices() );

        AddVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * desc->SingleVertexEntrySize();
    }

    return rad;
}

// ********************************
//
void                            BasicNode::AddVertexDataToVBO              ( char * data, IConnectedComponent * cc ) const
{
    unsigned int numVertices = cc->GetNumVertices();
    unsigned int offset = 0;

    for( unsigned int i = 0; i < numVertices; ++i )
    {
        for( auto vach : cc->GetVertexAttributeChannels() )
        {
            assert( vach->GetNumEntries() == numVertices );

            auto eltSize = vach->GetDescriptor()->GetEntrySize();
            const char * eltData = vach->GetData();

            memcpy( &data[ offset ], &eltData[ i * eltSize ], eltSize );

            offset += eltSize;
        }
    }
}

// ********************************
//
VertexDescriptor *                  BasicNode::CreateVertexDescriptor          ( const IGeometryChannelDescriptor * desc ) const
{
    VertexDescriptor * vertexDescriptor = new VertexDescriptor( desc->GetNumVertexChannels() );

    unsigned int attributeOffset = 0;

    for( unsigned int i = 0; i < desc->GetNumVertexChannels(); ++i )
    {
        auto * channelDesc = desc->GetVertexChannelDescriptor( i );

        //FIXME: default channel location just copied from model channel ordering (maybe it should be a permutation or something)
        vertexDescriptor->SetAttribute( i, i, attributeOffset, channelDesc->GetType(), channelDesc->GetSemantic() );
        attributeOffset += channelDesc->GetEntrySize();     
    }

    vertexDescriptor->SetStride( attributeOffset );

    return vertexDescriptor;
}

// ********************************
//
unsigned int                        BasicNode::TotalNumVertices       ( const std::vector< IConnectedComponent * > & ccVec ) const
{
    unsigned int totalNumVertices = 0;

    for( auto cc : ccVec )
    {
        totalNumVertices += cc->GetNumVertices();
    }

    return totalNumVertices;
}

// ********************************
//
unsigned int                        BasicNode::TotalSize             ( const std::vector< IConnectedComponent * > & ccVec, const IGeometryChannelDescriptor * desc ) const
{
    return TotalNumVertices( ccVec ) * desc->SingleVertexEntrySize();
}

// ********************************
//
void                                BasicNode::RegisterShaderParameters (const IShaderChannel* shaderChannel, ShaderParameters * shParams)
{
    for(auto param : shaderChannel->GetValuesList())
    {
        GenericShaderParam* genShaderParam = ShaderParamFactory::Get().Create( param->GetName(), param->GetParamType() );
        shParams->RegisterParameter(genShaderParam);
    }
}

// ********************************
//
void            BasicNode::Print                    (std::ostream& out, int tabs) const
{
    out << "------------------NODE-------------------- : " << this << debug::EndLine(tabs);
    //out << "Transform plugins: " << m_transformPlugins.size() << debug::EndLine(tabs + 1);
    //for(auto tp : m_transformPlugins)
    //    tp->Print(out, tabs + 1);
    
    //out << debug::EndLine(tabs) << "Pixel Shader plugin: " << m_pshaderPlugin << debug::EndLine(tabs + 1);
    //if(m_pshaderPlugin != nullptr)
    //    m_pshaderPlugin->Print(out, tabs + 1);

    //out << debug::EndLine(tabs) << "Vertex Shader plugin: " << m_vshaderPlugin << debug::EndLine(tabs + 1);
    //if(m_vshaderPlugin != nullptr)
    //    m_vshaderPlugin->Print(out, tabs + 1);

    //out << debug::EndLine(tabs) << "Geometry Shader plugin: " << m_gshaderPlugin << debug::EndLine(tabs + 1);
    //if(m_gshaderPlugin != nullptr)
    //    m_gshaderPlugin->Print(out, tabs + 1);

    out << debug::EndLine(tabs) << "------------------PARENT OF---------------- : " << m_children.size() << debug::EndLine(tabs + 1);
    for(auto ch : m_children)
    {
        ch->Print(out, tabs + 1);
        out << debug::EndLine(tabs + 1);
    }
}

} // model
} // bv
