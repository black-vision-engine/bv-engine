#include "SimpleTexturePlugin.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/VertexShader/TextureVertexShaderChannel.h"
#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/Plugins/Parameter.h"


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
SimpleTexturePlugin::SimpleTexturePlugin                    ( const IPlugin * prev, const std::vector< std::string > & texturesFilesNames, TextureAttachmentMode mode )
    : BasePlugin( prev )
    , m_attachmentMode( mode )
{
    assert( prev != nullptr );

    for(unsigned int i = 0; i < texturesFilesNames.size(); ++i)
    {
        m_textures.push_back( LoadTexture( "Tex" + std::to_string( i ), texturesFilesNames[ i ] ) );
    }

    m_geomChannel = nullptr;
    EvalGeometryChannel( prev );

    // Set Pixel Shader Channel
	std::vector<TransformF> txMat;
	std::vector<FloatInterpolator> alphas;

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
SimpleTexturePlugin::~SimpleTexturePlugin        ()
{
    //delete m_alphaParam;
    //delete m_alphaValue;

    //delete m_tex0TransformParam;
    //delete m_tex0TransformValue;

    //delete m_tex1TransformParam;
    //delete m_tex1TransformValue;
}

// *************************************
//
void                        SimpleTexturePlugin::SetAttachmentMode           ( TextureAttachmentMode mode )
{
    m_attachmentMode = mode;
}

// *************************************
//
//const ITransformChannel*    SimpleTexturePlugin::GetTransformChannel         () const
//{
//    return m_prev->GetTransformChannel();
//}

// *************************************
//
void SimpleTexturePlugin::EvalGeometryChannel( const IPlugin* prev )
{
    auto prevGeomChannel = prev->GetGeometryChannel();
    VertexAttributeChannelDescriptor * desc = new VertexAttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    for( unsigned int i = 0; i < prevGeomChannel->GetComponents().size(); ++i )
    {
        ConnectedComponent* connComp = new ConnectedComponent();
        GeometryChannelDescriptor geomChannelDesc;

        auto prevConnComp = static_cast< const model::ConnectedComponent* >( prevGeomChannel->GetComponents()[ i ] );
        auto prevCompChannels = prevConnComp->m_vertexAttributeChannels;

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->m_vertexAttributeChannels.push_back( prevCompCh );
        }

        if( m_geomChannel == nullptr )
        {
            for( auto prevCompCh : prevCompChannels )
            {
                auto prevCompChDesc = prevCompCh->GetDescriptor();
                geomChannelDesc.AddVertexAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
            }

            m_texCoordChannelIndex = geomChannelDesc.GetNumVertexChannels();

            for( unsigned int i = 0; i < m_textures.size(); ++i )
            {
                geomChannelDesc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
            }

            auto geomChannel = new model::GeometryChannel( prevGeomChannel->GetPrimitiveType(), geomChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
            m_geomChannel = geomChannel;
        }

        for( unsigned int i = 0; i < m_textures.size(); ++i )
        {

            auto verTexAttrChannel = new model::Float2VertexAttributeChannel( desc, m_textures[ 0 ]->m_texName, true );

            for( unsigned int j = 0; j < prevCompChannels[0]->GetNumEntries(); ++j )
            {
                const glm::vec3* pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );
                verTexAttrChannel->AddVertexAttribute( glm::vec2( pos[ j ].x, pos[ j ].y ) );
            }

            connComp->m_vertexAttributeChannels.push_back( verTexAttrChannel );
        }

        m_geomChannel->AddConnectedComponent( connComp );
    }
}

// *************************************
//
TextureInfo * SimpleTexturePlugin::LoadTexture( const std::string & name, const std::string & path ) const
{
    TextureLoader texLoader;

    Resource texture( name, path );

    return new TextureInfo( texLoader.LoadResource( &texture ), name );
}

namespace
{

// *************************************
//
VertexAttributeChannel *   GetPositionChannel( const std::vector< VertexAttributeChannel* > & channels )
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
VertexAttributeChannel*   GetUVChannel( const std::vector< VertexAttributeChannel* >& channels, unsigned int index )
{
    assert( !channels.empty() );
    assert( channels.size() > index );
    assert( channels[index]->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_TEXCOORD );

    return channels[index];
}

} // anonymouse

// *************************************
//
void                SimpleTexturePlugin::Update              ( float t )
{
    if( m_attachmentMode == TextureAttachmentMode::MM_FREE )
    {
        if( m_prevPlugin->GetGeometryChannel()->NeedsAttributesUpdate( t ) )
        {
            for( unsigned int i = 0; i < m_geomChannel->GetComponents().size(); ++i )
            {
                auto connComp = static_cast< const model::ConnectedComponent* >( m_geomChannel->GetComponents()[ i ] );
                auto compChannels = connComp->m_vertexAttributeChannels;

                if( auto posChannel = GetPositionChannel( compChannels ) )
                    if( auto uvChannel = GetUVChannel( compChannels, m_texCoordChannelIndex ) )
                    {
                        auto & verts  = dynamic_cast< Float3VertexAttributeChannel* >(posChannel)->GetVertices();
                        auto & uvs    = dynamic_cast< Float2VertexAttributeChannel* >(uvChannel)->GetVertices();

                        for( unsigned int i = 0; i < verts.size(); ++i )
                        {
                            uvs[ i ].x = verts[ i ].x;
                            uvs[ i ].y = verts[ i ].y;
                        }
                    }
            }

            m_geomChannel->SetNeedsAttributesUpdate( true );
        }
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
const IGeometryChannel *            SimpleTexturePlugin::GetGeometryChannel          () const
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
