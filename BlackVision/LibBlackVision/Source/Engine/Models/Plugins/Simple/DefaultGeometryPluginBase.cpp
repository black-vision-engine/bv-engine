#include "DefaultGeometryPluginBase.h"

//#include "Engine/Models/Plugins/Channels/Geometry/Simple/VertexAttributesChannelVariableTopology.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"

#include "Mathematics/defines.h"

namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGeometryPluginDescBase::DefaultGeometryPluginDescBase                                ( const std::string & uid, const std::string & name )
    : BasePluginDescriptor( uid, name )
{
}

// *******************************
//
bool                            DefaultGeometryPluginDescBase::CanBeAttachedTo      ( IPluginConstPtr plugin )  const
{
    if( !BasePluginDescriptor::CanBeAttachedTo( plugin ) )
    {
        return false;
    }

    //Geometry generator cannot be attached to a plugin which generates geometry itself
    if( plugin && plugin->GetVertexAttributesChannel() )
    {
        return false;
    }

    return true;
}

// *******************************
//
//IPluginPtr                      DefaultGeometryPluginDescBase::CreatePlugin         ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
//{
//    //return CreatePluginTyped< DefaultGeometryPluginBase >( name, prev, timeEvaluator );
//}

// *******************************
//
//DefaultPluginParamValModelPtr   DefaultGeometryPluginDescBase::CreateDefaultModel   ( ITimeEvaluatorPtr timeEvaluator ) const
//{
//    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
//    DefaultParamValModelPtr         vacModel    = std::make_shared< DefaultParamValModel >();
//
//    //ParamFloatPtr paramN             = ParametersFactory::CreateParameterFloat( "n", timeEvaluator );
//    ParamFloatPtr paramAngleStart	 = ParametersFactory::CreateParameterFloat( "angleStart", timeEvaluator );
//    ParamFloatPtr paramAngleEnd		 = ParametersFactory::CreateParameterFloat( "angleEnd", timeEvaluator );
//
//    //vacModel->AddParameter( paramN );
//    vacModel->AddParameter( paramAngleStart );
//    vacModel->AddParameter( paramAngleEnd );
//
//    model->SetVertexAttributesChannelModel( vacModel );
//
//    //Set default parameters
//    //paramN->SetVal( 5.f, 0.f ); // FIXME: this is f*)*(&ing ridiculous!
//    paramAngleStart->SetVal( float( PI/4 ), 0 );
//    paramAngleEnd->SetVal( float( 3*PI/4 ), 0 );
//
//    return model;
//}

// *******************************
//
std::string                     DefaultGeometryPluginDescBase::UID                  ()
{
    assert( false );
    return "";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
//
DefaultGeometryPluginBase::DefaultGeometryPluginBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
{
    //auto asParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "angleStart" );
    //assert( asParam );
    //float asVal = QueryTypedParam< ParamFloatPtr > ( asParam )->Evaluate();

    //auto aeParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "angleEnd" );
    //assert( aeParam );
    //float aeVal = QueryTypedParam< ParamFloatPtr > ( aeParam )->Evaluate();

    //InitGeometry();
}

//DefaultGeometryPluginBase::~DefaultGeometryPluginBase(void)
//{
//}

IVertexAttributesChannelConstPtr    DefaultGeometryPluginBase::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

//// geometry generators, no UVs, "old" approach
//// FIXME: definitely deprecated, should be done like UVed ones!!!
//
//double angleStart, angleEnd, dangle, z;
//
//void GenerateBase( Float3AttributeChannelPtr verts )
//{
//    double angle = angleStart;
//
//    for( ; angle < angleEnd; angle += dangle )
//    {
//        verts->AddAttribute( glm::vec3( cos( angle ), z, sin( angle ) ) );
//
//        if( verts->GetNumEntries()%2 == 1 )
//        {
//            verts->AddAttribute( glm::vec3( 0, z, 0 ) );
//        }
//    }
//    angle = angleEnd;
//}
//
//double angle, z1, z2;
//
//void GenerateSide( Float3AttributeChannelPtr verts )
//{
//    verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
//    verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
//    verts->AddAttribute( glm::vec3( 0, z1, 0 ) );
//    verts->AddAttribute( glm::vec3( 0, z2, 0 ) );
//}
//
//// geometry&uv generators - functors FTW!!!
//
//class GenerateBaseUV : public IGeometryAndUVsGenerator 
//{
//private:
//
//    // FIXME: use m_ prefix for instance variables !!!!
//    double z;
//
//public:
//
//    GenerateBaseUV( double z_ ) : z( z_ ) { }
//
//    void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
//    {
//        double angle = angleStart;
//
//        for( ; angle < angleEnd; angle += dangle )
//        {
//            verts->AddAttribute( glm::vec3( cos( angle ), z, sin( angle ) ) );
//            uvs->AddAttribute( glm::vec2( angle, z ) );
//
//            if( verts->GetNumEntries()%2 == 1 )
//            {
//                verts->AddAttribute( glm::vec3( 0, z, 0 ) );
//                uvs->AddAttribute( glm::vec2( 0, z ) );
//            }
//        }
//        angle = angleEnd;
//        verts->AddAttribute( glm::vec3( cos( angle ), z, sin( angle ) ) );
//        uvs->AddAttribute( glm::vec2( angle, z ) );
//    }
//};
//
//class GenerateSideUV : public IGeometryAndUVsGenerator {
//    double z1, z2;
//    double angle;
//
//public:
//    GenerateSideUV( double z1_, double z2_, double angle_ ) : z1( z1_ ), z2( z2_ ), angle( angle_ ) {}
//
//void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
//{
//    verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
//    verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
//    verts->AddAttribute( glm::vec3( 0, z1, 0 ) );
//    verts->AddAttribute( glm::vec3( 0, z2, 0 ) );
//
//    uvs->AddAttribute( glm::vec2( angle, z1 ) );
//    uvs->AddAttribute( glm::vec2( angle, z2 ) );
//    uvs->AddAttribute( glm::vec2( 0, z1 ) );
//    uvs->AddAttribute( glm::vec2( 0, z2 ) );
//}
//};
//
//class GenerateRoundSideUV : public IGeometryAndUVsGenerator {
//    double angleStart, angleEnd;
//    double z1, z2;
//public:
//    GenerateRoundSideUV( double as, double ae, double z1_, double z2_ ) :
//        angleStart( as ), angleEnd( ae ), z1( z1_ ), z2( z2_ ) {}
//
//void GenerateGeometryAndUVs( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
//{
//    double angle = angleStart;
//
//    for( ; angle <= angleEnd; angle += dangle )
//    {
//        verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
//        uvs->AddAttribute( glm::vec2( angle, z1 ) );
//
//        verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
//        uvs->AddAttribute( glm::vec2( angle, z2 ) );
//    }
//    angle = angleEnd;
//
//    verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
//    uvs->AddAttribute( glm::vec2( angle, z1 ) );
//
//    verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
//    uvs->AddAttribute( glm::vec2( angle, z2 ) );
//}
//};

void DefaultGeometryPluginBase::InitGeometry()
{
    DefaultGeometryAndUVsVertexAttributeChannel* channel;
    if( m_vaChannel==NULL ) // FIXME: this should be smarter and maybe moved to DefaultGeometryAndUVsVertexAttributeChannel
    {
        channel = new DefaultGeometryAndUVsVertexAttributeChannel( PrimitiveType::PT_TRIANGLE_STRIP );
        m_vaChannel = VertexAttributesChannelPtr( (VertexAttributesChannel*) channel );
    } else
    {
        channel = (DefaultGeometryAndUVsVertexAttributeChannel*) m_vaChannel.get();
        channel->ClearAll();
    }

    auto gen = GetGenerator();
    channel->GenerateAndAddConnectedComponent( *gen );
}

// *************************************
//
void                                DefaultGeometryPluginBase::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    m_pluginParamValModel->Update();

    if( NeedsTopologyUpdate() )
    {
        InitGeometry();
        m_vaChannel->SetNeedsTopologyUpdate( true );
    }
}

} }