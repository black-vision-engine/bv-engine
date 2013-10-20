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

//PLUGIN PARAMETERS
const std::string SimpleTexturePluginPD::alphaParamName( "alpha" );
const std::string SimpleTexturePluginPD::txMatrix0ParamName( "txMat0" );
const std::string SimpleTexturePluginPD::txMatrix1ParamName( "txMat1" );


// *************************************
//
SimpleTexturePluginPD::SimpleTexturePluginPD()
    : BaseParametersDescriptor( pluginName )
{
    m_params[ alphaParamName ] = ParamType::PT_FLOAT1;
    m_params[ txMatrix0ParamName ] = ParamType::PT_MAT4;
    m_params[ txMatrix1ParamName ] = ParamType::PT_MAT4;
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTexturePlugin::SimpleTexturePlugin                    ( const IPlugin* prev, const std::vector< std::string > & texturesFilesNames )
{
    for(unsigned int i = 0; i < texturesFilesNames.size(); ++i)
    {
        m_textures.push_back( LoadTexture( "Tex" + std::to_string( i ), texturesFilesNames[ i ] ) );
    }

    m_geomChannel = nullptr;
    EvalGeometryChannel( prev );
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

            m_geomChannel = new model::GeometryChannel( prevGeomChannel->GetPrimitiveType(), geomChannelDesc, true, true );
        }

        for( unsigned int i = 0; i < m_textures.size(); ++i )
        {

            auto verTexAttrChannel = new model::Float2VertexAttributeChannel( desc, m_textures[ 0 ].second, true );

            verTexAttrChannel->AddVertexAttribute( glm::vec2( 0.f, 0.f ) );
            verTexAttrChannel->AddVertexAttribute( glm::vec2( 1.f, 0.f ) );
            verTexAttrChannel->AddVertexAttribute( glm::vec2( 0.f, 1.f ) );
            verTexAttrChannel->AddVertexAttribute( glm::vec2( 1.f, 1.f ) );

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
SimpleTexturePlugin::~SimpleTexturePlugin   ()
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
void                SimpleTexturePlugin::Update              ( float t )
{
    BasePlugin::Update( t );

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
