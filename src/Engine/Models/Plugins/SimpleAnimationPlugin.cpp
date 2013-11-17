#include "SimpleAnimationPlugin.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/TexturePixelShaderChannel.h"
#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/Plugins/Parameter.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleAnimationPluginPD::pluginName( "SimpleAnimationPlugin" );


//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// *************************************
//
SimpleAnimationPluginPD::SimpleAnimationPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleAnimationPlugin::SimpleAnimationPlugin                    ( const IPlugin * prev, const std::vector< std::string > & texturesFilesNames, unsigned int fps, TextureAttachmentMode mode )
    : BasePlugin( prev )
    , m_fps( fps )
    , m_attachmentMode( mode )
    , m_startTime( -1.f )
{
    assert( prev != nullptr );
    assert( fps > 0 );

    m_secsPerFrame = 1.f / float( fps );

    for( unsigned int i = 0; i < texturesFilesNames.size(); ++i )
    {
        m_textures.push_back( LoadTexture( "Tex" + std::to_string( i ), texturesFilesNames[ i ] ) );
    
        printf( "Loading sequence %d%%\r", (int) ( 100.f * ( (float(i + 1) / texturesFilesNames.size() ) ) ) );
    }
    printf( "\n" );

    m_geomChannel = nullptr;

    EvalGeometryChannel( prev );

    m_numFrames  = texturesFilesNames.size();

    // Set Pixel Shader Channel
	std::vector<TransformF> txMat;
	std::vector<FloatInterpolator> alphas;

    m_pixelShaderChannel = new model::TexturePixelShaderChannel( "../dep/media/shaders/simpleanimation.frag", alphas, txMat );

    m_pixelShaderChannel->SetRendererContext( PluginsFactory::CreateDefaultRenderableContext() );
    auto rendContext = m_pixelShaderChannel->GetRendererContext();
    rendContext->cullCtx->enabled = false;

	m_vertexShaderChannel = new model::TextureVertexShaderChannel( "../dep/media/shaders/simpleanimation.vert" );
}

// *************************************
//
SimpleAnimationPlugin::~SimpleAnimationPlugin        ()
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
void                        SimpleAnimationPlugin::SetAttachmentMode           ( TextureAttachmentMode mode )
{
    m_attachmentMode = mode;
}

// *************************************
//
void SimpleAnimationPlugin::EvalGeometryChannel( const IPlugin * prev )
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

            geomChannelDesc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            auto geomChannel = new model::GeometryChannel( prevGeomChannel->GetPrimitiveType(), geomChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
            m_geomChannel = geomChannel;
        }

        //Only one animation is added
        for( unsigned int i = 0; i < 1; ++i )
        {
            float minX = 100000.0f, minY = 100000.0f;
            float maxX = 0.0f, maxY = 0.0f;

            //convex hull
            for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
            {
                const glm::vec3 * pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );

                minX = std::min( minX, pos[ j ].x );
                minY = std::min( minY, pos[ j ].y );
                maxX = std::max( maxX, pos[ j ].x );
                maxY = std::max( maxY, pos[ j ].y );
            }

            auto verTexAttrChannel = new model::Float2VertexAttributeChannel( desc, m_textures[ 0 ]->m_texName, true );

            for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
            {
                const glm::vec3* pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );
                verTexAttrChannel->AddVertexAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
            }

            connComp->m_vertexAttributeChannels.push_back( verTexAttrChannel );

            m_geomChannel->AddConnectedComponent( connComp );
        }
    }
}

// *************************************
//
TextureInfo * SimpleAnimationPlugin::LoadTexture( const std::string& name, const std::string& path ) const
{
    TextureLoader texLoader;

    Resource texture( name, path );

    return new TextureInfo( texLoader.LoadResource( &texture ), name );
}

namespace
{

// *************************************
//
VertexAttributeChannel *  GetPositionChannel( const std::vector< VertexAttributeChannel * > & channels )
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
    assert( channels[ index ]->GetDescriptor()->GetSemantic() == AttributeSemantic::AS_TEXCOORD );

    return channels[ index ];
}

} // anonymouse

// *************************************
//
unsigned int        SimpleAnimationPlugin::CurrentFrame        ( float t ) const
{
    if( m_startTime < 0.0f )
    {
        m_startTime = t;
    }

    unsigned int frameNum = (unsigned int) ( ( t - m_startTime ) * m_fps );

    return frameNum % m_numFrames;
}

// *************************************
//
unsigned int        SimpleAnimationPlugin::PredictedNextFrame  ( float t ) const
{
    return ( 1 + CurrentFrame( t ) ) % m_numFrames;
}

// *************************************
//
bool                SimpleAnimationPlugin::HasAnimatingTexture () const
{
    return true;
}

// *************************************
//
ISequenceAnimationSource *  SimpleAnimationPlugin::QuerySequenceAnimationSource()
{
    return static_cast< ISequenceAnimationSource * >( this );
}

// *************************************
//
void                SimpleAnimationPlugin::Update              ( float t )
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
void                SimpleAnimationPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
    for( auto t : m_textures )
    {
        out << "Texture: " << t->m_texName << debug::EndLine( tabs );
    }
}

const IGeometryChannel *            SimpleAnimationPlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

const IPixelShaderChannel *         SimpleAnimationPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;
}

const IVertexShaderChannel *        SimpleAnimationPlugin::GetVertexShaderChannel      () const
{
    return m_vertexShaderChannel;
}


} // model
} // bv
