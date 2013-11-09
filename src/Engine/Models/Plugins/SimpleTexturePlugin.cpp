#include "SimpleTexturePlugin.h"
#include "System/Print.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"

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
SimpleTexturePlugin::SimpleTexturePlugin                    ( const IPlugin* prev, const std::vector< std::string > & texturesFilesNames )
    : m_prev( prev )
{
    assert( prev != nullptr );

    for(unsigned int i = 0; i < texturesFilesNames.size(); ++i)
    {
        m_textures.push_back( LoadTexture( "Tex" + std::to_string( i ), texturesFilesNames[ i ] ) );
    }

    m_geomChannel = nullptr;
    EvalGeometryChannel( prev );
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
const ITransformChannel*    SimpleTexturePlugin::GetTransformChannel         () const
{
    return m_prev->GetTransformChannel();
}

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

            geomChannelDesc.AddVertexAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

            auto geomChannel = new model::GeometryChannel( prevGeomChannel->GetPrimitiveType(), geomChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
            m_geomChannel = geomChannel;
        }

        for( unsigned int i = 0; i < m_textures.size(); ++i )
        {

            auto verTexAttrChannel = new model::Float2VertexAttributeChannel( desc, m_textures[ 0 ].second, true );

            for( unsigned int j = 0; j < prevCompChannels[0]->GetNumEntries(); ++j )
            {
                const glm::vec3* pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );
                verTexAttrChannel->AddVertexAttribute( glm::vec2( pos[ j ].x, pos[ j ].y ) );
            }

            connComp->m_vertexAttributeChannels.push_back( verTexAttrChannel );

            m_geomChannel->AddConnectedComponent( connComp );
        }
    }
}

// *************************************
//
SimpleTexturePlugin::TexturePair SimpleTexturePlugin::LoadTexture( const std::string& name, const std::string& path ) const
{
    TextureLoader texLoader;

    Resource texture( name, path );

    return SimpleTexturePlugin::TexturePair( texLoader.LoadResource( &texture ), name );
}

// *************************************
//
void                SimpleTexturePlugin::Update              ( float t )
{
    if( m_prev->GetGeometryChannel()->NeedsPositionsUpdate( t ) )
    {
        for( unsigned int i = 0; i < m_geomChannel->GetComponents().size(); ++i )
        {
            auto connComp = static_cast< const model::ConnectedComponent* >( m_geomChannel->GetComponents()[ i ] );
            auto compChannels = connComp->m_vertexAttributeChannels;

            auto & verts  = dynamic_cast< Float3VertexAttributeChannel* >(compChannels[0])->GetVertices();
            auto & uvs    = dynamic_cast< Float2VertexAttributeChannel* >(compChannels[1])->GetVertices();

            for( unsigned int i = 0; i < verts.size(); ++i )
            {
                uvs[ i ].x = verts[ i ].x;
                uvs[ i ].y = verts[ i ].y;
            }
        }

        m_geomChannel->SetNeedsPositionUpdate( true );
    }

    BasePlugin::Update( t );

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
        out << "Texture: " << t.second << debug::EndLine( tabs );
    }
}


//// ***************************** DESCRIPTOR ********************************** 
////PLUGIN NAME
//const std::string SimpleTextureVertexPluginPD::pluginName( "SimpleTextureVertex" );
//
//// *************************************
////
//SimpleTextureVertexPluginPD::SimpleTextureVertexPluginPD()
//    : BaseParametersDescriptor( pluginName )
//{
//}
//
//// ***************************** PLUGIN ********************************** 
//
//// *************************************
////
//SimpleTextureVertexPlugin::SimpleTextureVertexPlugin            ()
//{
//}
//
//// *************************************
////
//std::string SimpleTextureVertexPlugin::GetShaderFile              () const
//{
//    // TODO; do not bind SmpleTexturePlugin with exact shader - at some point it will be provided by GLSL shader service or whatever else resource manager
//    return "../dep/media/shaders/simpletexture.vert";
//}
//
//// *************************************
////
//void                SimpleTextureVertexPlugin::Update              ( float t )
//{
//    //FIXME: implement - whatever is required here
//}
//
//// *************************************
////
//void                SimpleTextureVertexPlugin::Print               ( std::ostream & out, int tabs ) const
//{
//    out << GetName() << debug::EndLine( tabs );
//}

} // model
} // bv
