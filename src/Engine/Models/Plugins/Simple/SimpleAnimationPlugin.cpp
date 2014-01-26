#include "SimpleAnimationPlugin.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/PluginsFactory.h"


namespace bv { namespace model {

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleAnimationPlugin::SimpleAnimationPlugin                    ( const IPlugin * prev, const std::vector< std::string > & texturesFilesNames, const ParamFloat & frameCounter, model::RendererContext * ctx, TextureAttachmentMode mode )
    : BasePlugin( prev )
    , m_attachmentMode( mode )
    , m_frameCounter( frameCounter )
{
    assert( prev != nullptr );

    for( unsigned int i = 0; i < texturesFilesNames.size(); ++i )
    {
        m_textures.push_back( LoadTexture( "Tex" + std::to_string( i ), texturesFilesNames[ i ] ) );
    
        printf( "Loading sequence %d%%\r", (int) ( 100.f * ( ( float(i + 1) / texturesFilesNames.size() ) ) ) );
    }
    printf( "\n" );

    m_vaChannel = nullptr;

    EvalGeometryChannel( prev );

    m_numFrames  = texturesFilesNames.size();

    // Set Pixel Shader Channel
	std::vector<ParamTransform> txMat;
	std::vector<ParamFloat> alphas;
    std::vector<ParamVec4> bc;

    m_pixelShaderChannel = new model::TexturePixelShaderChannel( "../dep/media/shaders/simpleanimation.frag", alphas, txMat, bc );

    if( !ctx )
        ctx = RendererContext::CreateDefault();

    m_pixelShaderChannel->SetRendererContext( ctx );

    auto rendContext = m_pixelShaderChannel->GetRendererContext();
    rendContext->cullCtx->enabled = false;

	m_vertexShaderChannel = new model::TextureVertexShaderChannel( "../dep/media/shaders/simpleanimation.vert" );
}

// *************************************
//
SimpleAnimationPlugin::~SimpleAnimationPlugin        ()
{
    delete m_vaChannel;
    delete m_pixelShaderChannel;
    delete m_vertexShaderChannel;
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
    auto prevGeomChannel = prev->GetVertexAttributesChannel();
    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

    for( unsigned int i = 0; i < prevGeomChannel->GetComponents().size(); ++i )
    {
        ConnectedComponent* connComp = new ConnectedComponent();
        VertexAttributesChannelDescriptor vaChannelDesc;

        auto prevConnComp = static_cast< const model::ConnectedComponent* >( prevGeomChannel->GetComponents()[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

        if( m_vaChannel == nullptr )
        {
            for( auto prevCompCh : prevCompChannels )
            {
                auto prevCompChDesc = prevCompCh->GetDescriptor();
                vaChannelDesc.AddAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
            }

            m_texCoordChannelIndex = vaChannelDesc.GetNumVertexChannels();

            vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            auto vaChannel = new model::VertexAttributesChannel( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
            m_vaChannel = vaChannel;
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

            auto verTexAttrChannel = new model::Float2AttributeChannel( desc, m_textures[ 0 ]->m_texName, true );

            for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
            {
                const glm::vec3* pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );
                verTexAttrChannel->AddAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
            }

            connComp->AddAttributeChannel( AttributeChannelPtr( verTexAttrChannel ) );

            m_vaChannel->AddConnectedComponent( connComp );
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

// *************************************
//
unsigned int        SimpleAnimationPlugin::CurrentFrame        ( TimeType t ) const
{
    float fFrame = m_frameCounter.Evaluate( t );
    int nFrame = (int) fFrame;

    return nFrame % m_numFrames;
}

// *************************************
//
unsigned int        SimpleAnimationPlugin::PredictedNextFrame  ( TimeType t ) const
{
    return ( 1 + CurrentFrame( t ) ) % m_numFrames; //FIXME: this sux - it strongly depends on Timeline used possible time modifications (e.g. playing backwards)
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
void                SimpleAnimationPlugin::Update              ( TimeType t )
{
    if( m_attachmentMode == TextureAttachmentMode::MM_FREE )
    {
        if( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate( t ) )
        {
            for( unsigned int i = 0; i < m_vaChannel->GetComponents().size(); ++i )
            {
                auto connComp = static_cast< const model::ConnectedComponent* >( m_vaChannel->GetComponents()[ i ] );
                auto compChannels = connComp->GetAttributeChannels();

                if( auto posChannel = AttributeChannel::GetPositionChannel( compChannels ) )
                    if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, m_texCoordChannelIndex ) )
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

    m_vaChannel->SetNeedsAttributesUpdate( true );

    m_vaChannel->Update( t );
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

const IVertexAttributesChannel *            SimpleAnimationPlugin::GetVertexAttributesChannel          () const
{
    return m_vaChannel;
}

const IPixelShaderChannel *         SimpleAnimationPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;
}

const IVertexShaderChannel *        SimpleAnimationPlugin::GetVertexShaderChannel      () const
{
    return m_vertexShaderChannel;
}

// *************************************
//
Textures            SimpleAnimationPlugin::GetTextures                 () const
{
    auto prevTextures = m_prevPlugin->GetTextures();
    prevTextures.insert( prevTextures.end(), m_textures.begin(), m_textures.end() );
    return prevTextures;
}

} // model
} // bv
