#include "SimpleTexturePlugin.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"
#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleTexturePluginPD::pluginName( "SimpleTexturePlugin" );

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// *************************************
//
SimpleTexturePluginPD::SimpleTexturePluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTexturePlugin::SimpleTexturePlugin                    ( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, TextureAttachmentMode amode )
    : BasePlugin( prev )
    , m_attachmentMode( amode )
{
    assert( prev != nullptr );

    for(unsigned int i = 0; i < textureDescs.size(); ++i)
    {
        auto texInfo = LoadTexture( textureDescs[ i ], "Tex" + std::to_string( i ) );
        RegisterValue( texInfo->m_texTransformVal );
        m_textures.push_back( texInfo );
    }

    m_geomChannel = nullptr;
    EvalGeometryChannel( prev );

    // Set Pixel Shader Channel
    std::vector<ParamTransform> txMat;

    for( auto t : textureDescs )
    {
        txMat.push_back( t.transformParameter );
    }

    std::vector<ParamFloat> alphas;

    m_pixelShaderChannel = new model::TexturePixelShaderChannel( "../dep/media/shaders/simpletexture.frag"
										, alphas
										, txMat );

    m_pixelShaderChannel->SetRendererContext( PluginsFactory::CreateDefaultRenderableContext() );
    auto rendContext = m_pixelShaderChannel->GetRendererContext();
    rendContext->cullCtx->enabled = false;

	m_vertexShaderChannel = new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" );
}

// *************************************
//
SimpleTexturePlugin::SimpleTexturePlugin( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, model::RendererContext * ctx, TextureAttachmentMode mode )
    : BasePlugin( prev )
    , m_attachmentMode( mode )
{
    assert( prev != nullptr );

    for( unsigned int i = 0; i < textureDescs.size(); ++i )
    {
        auto texInfo = LoadTexture( textureDescs[ i ], "Tex" + std::to_string( i ) );
        RegisterValue( texInfo->m_texTransformVal );
        m_textures.push_back( texInfo );
    }

    m_geomChannel = nullptr;
    EvalGeometryChannel( prev );

    // Set Pixel Shader Channel
    std::vector<ParamTransform> txMat;

    for( auto t : textureDescs )
    {
        txMat.push_back( t.transformParameter );
    }

    std::vector<ParamFloat> alphas;

    m_pixelShaderChannel = new model::TexturePixelShaderChannel( "../dep/media/shaders/simpletexture.frag"
										, alphas
										, txMat );

    if ( ctx )
        m_pixelShaderChannel->SetRendererContext( ctx );
    else
        m_pixelShaderChannel->SetRendererContext( PluginsFactory::CreateDefaultRenderableContext() );

    auto rendContext = m_pixelShaderChannel->GetRendererContext();
    rendContext->cullCtx->enabled = false;

	m_vertexShaderChannel = new model::TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" );
}


// *************************************
//
SimpleTexturePlugin::~SimpleTexturePlugin        ()
{}

// *************************************
//
void                        SimpleTexturePlugin::SetAttachmentMode           ( TextureAttachmentMode mode )
{
    m_attachmentMode = mode;
}

// *************************************
//
void SimpleTexturePlugin::EvalGeometryChannel( const IPlugin* prev )
{
    auto prevGeomChannel = prev->GetGeometryChannel();
    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    for( unsigned int i = 0; i < prevGeomChannel->GetComponents().size(); ++i )
    {
        ConnectedComponent* connComp = new ConnectedComponent();
        VertexAttributesChannelDescriptor geomChannelDesc;

        auto prevConnComp = static_cast< const model::ConnectedComponent* >( prevGeomChannel->GetComponents()[ i ] );
        auto prevCompChannels = prevConnComp->m_attributeChannels;

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->m_attributeChannels.push_back( prevCompCh );
        }

        if( m_geomChannel == nullptr )
        {
            for( auto prevCompCh : prevCompChannels )
            {
                auto prevCompChDesc = prevCompCh->GetDescriptor();
                geomChannelDesc.AddAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
            }

            m_texCoordChannelIndex = geomChannelDesc.GetNumVertexChannels();

            for( unsigned int i = 0; i < m_textures.size(); ++i )
            {
                geomChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
            }

            auto geomChannel = new model::VertexAttributesChannel( prevGeomChannel->GetPrimitiveType(), geomChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
            m_geomChannel = geomChannel;
        }

        for( unsigned int i = 0; i < m_textures.size(); ++i )
        {

            float minX = 100000.0f, minY = 100000.0f;
            float maxX = 0.0f, maxY = 0.0f;

            //convex hull - make sure that prevCompChannels[ 0 ] is indeed a positional channel
            for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
            {
                const glm::vec3 * pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );

                minX = std::min( minX, pos[ j ].x );
                minY = std::min( minY, pos[ j ].y );
                maxX = std::max( maxX, pos[ j ].x );
                maxY = std::max( maxY, pos[ j ].y );
            }

            auto verTexAttrChannel = new model::Float2AttributeChannel( desc, m_textures[ 0 ]->m_texName, true );

            for( unsigned int j = 0; j < prevCompChannels[0]->GetNumEntries(); ++j )
            {
                const glm::vec3* pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );
                verTexAttrChannel->AddAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
            }

            connComp->m_attributeChannels.push_back( verTexAttrChannel );
        }

        m_geomChannel->AddConnectedComponent( connComp );
    }
}

// *************************************
//
TextureInfo * SimpleTexturePlugin::LoadTexture( const TextureDescriptor & texDesc, const std::string& name ) const
{
    TextureLoader texLoader;

    Resource texture( name, texDesc.textureFileName );

    return new TextureInfo( texLoader.LoadResource( &texture ), name, texDesc.transformParameter, texDesc.wrappingMode );
}

namespace
{

// *************************************
//
AttributeChannel *   GetPositionChannel( const std::vector< AttributeChannel* > & channels )
{
    if( !channels.empty() )
    {
        // try to guess
        if( channels[ 0 ]->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION )
            return channels[ 0 ];

        for( auto ch : channels )
            if( ch->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_POSITION )
                return ch;
    }

    return nullptr;
}

// *************************************
//
AttributeChannel*   GetUVChannel( const std::vector< AttributeChannel* >& channels, unsigned int index )
{
    assert( !channels.empty() );
    assert( channels.size() > index );
    assert( channels[index]->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_TEXCOORD );

    return channels[index];
}

} // anonymouse

// *************************************
//
void                SimpleTexturePlugin::Update              ( TimeType t )
{
    if( m_attachmentMode == TextureAttachmentMode::MM_FREE )
    {
        if( m_prevPlugin->GetGeometryChannel()->NeedsAttributesUpdate( t ) )
        {
            for( unsigned int i = 0; i < m_geomChannel->GetComponents().size(); ++i )
            {
                auto connComp = static_cast< const model::ConnectedComponent* >( m_geomChannel->GetComponents()[ i ] );
                auto compChannels = connComp->m_attributeChannels;

                if( auto posChannel = GetPositionChannel( compChannels ) )
                    if( auto uvChannel = GetUVChannel( compChannels, m_texCoordChannelIndex ) )
                    {
                        auto & verts  = dynamic_cast< Float3AttributeChannel* >(posChannel)->GetVertices();
                        auto & uvs    = dynamic_cast< Float2AttributeChannel* >(uvChannel)->GetVertices();

                        for( unsigned int i = 0; i < verts.size(); ++i )
                        {
                            uvs[ i ].x = verts[ i ].x;
                            uvs[ i ].y = verts[ i ].y;
                        }
                    }
            }
        }

    }

    if ( m_prevPlugin->GetGeometryChannel()->NeedsAttributesUpdate( t ) )
    {
        m_geomChannel->SetNeedsAttributesUpdate( true );
    }

    for( auto ti : m_textures )
    {
        ti->m_texTransformVal->SetValue( ti->m_texTransform.Evaluate( t ) );
    }

    m_geomChannel->Update( t );
    m_pixelShaderChannel->Update( t );
    m_vertexShaderChannel->Update( t );

    //FIXME: update chanels according to parent (e.g. when position data has been changed)
//    m_alphaValue->SetValue( m_alphaParam->Evaluate( t ) );
//    m_tex0TransformValue->SetValue( m_tex0TransformParam->Evaluate( t ) );
//    m_tex1TransformValue->SetValue( m_tex1TransformParam->Evaluate( t ) );
}

// *************************************
//
void                SimpleTexturePlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
    for( auto t : m_textures )
    {
        out << "Texture: " << t->m_texName << debug::EndLine( tabs );
    }
}

// *************************************
//
const IVertexAttributesChannel *            SimpleTexturePlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

// *************************************
//
const IPixelShaderChannel *         SimpleTexturePlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;
}

// *************************************
//
const IVertexShaderChannel *        SimpleTexturePlugin::GetVertexShaderChannel      () const
{
    return m_vertexShaderChannel;
}

} // model
} // bv
