#include "DefaultEllipsePlugin.h"

#include "Mathematics/Defines.h"

namespace bv { namespace model {

const std::string DefaultEllipsePlugin::PN_QUALITY = "tesselation";
const std::string DefaultEllipsePlugin::PN_OUTER_RADIUS1 = "outer radius 1";
const std::string DefaultEllipsePlugin::PN_OUTER_RADIUS2 = "outer radius 2";
const std::string DefaultEllipsePlugin::PN_OPEN_ANGLE = "open angle";
//const std::string DefaultEllipsePlugin::PN_OPEN_ANGLE_MODE = "open angle mode";

//// *******************************
////
//VoidPtr    ParamEnum< DefaultEllipsePlugin::OpenAngleMode >::QueryParamTyped  ()
//{
//    return std::static_pointer_cast< void >( shared_from_this() );
//}

DefaultEllipsePluginDesc::DefaultEllipsePluginDesc()
        : DefaultGeometryPluginDescBase( UID(), "ellipse" ) { }


DefaultPluginParamValModelPtr   DefaultEllipsePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr         vacModel    = std::make_shared< DefaultParamValModel >();

    ParamFloatPtr paramN    = ParametersFactory::CreateParameterFloat( DefaultEllipsePlugin::PN_QUALITY, timeEvaluator );
    ParamFloatPtr paramOR1  = ParametersFactory::CreateParameterFloat( DefaultEllipsePlugin::PN_OUTER_RADIUS1, timeEvaluator );
    ParamFloatPtr paramOR2  = ParametersFactory::CreateParameterFloat( DefaultEllipsePlugin::PN_OUTER_RADIUS2, timeEvaluator );
    ParamFloatPtr paramOA   = ParametersFactory::CreateParameterFloat( DefaultEllipsePlugin::PN_OPEN_ANGLE, timeEvaluator );
    //auto paramOAM = ParametersFactory::CreateParameterEnum< DefaultEllipsePlugin::OpenAngleMode >( DefaultEllipsePlugin::PN_OPEN_ANGLE_MODE, timeEvaluator );

    model->SetVertexAttributesChannelModel( vacModel );
    vacModel->AddParameter( paramN );
    vacModel->AddParameter( paramOR1 );
    vacModel->AddParameter( paramOR2 );
    vacModel->AddParameter( paramOA );
    //vacModel->AddParameter( paramOAM );

    paramN->SetVal( 1.f, 0.f );
    paramOR1->SetVal( 1.f, 0.f );
    paramOR2->SetVal( 1.f, 0.f );
    paramOA->SetVal( 360.f, 0.f );
    //paramOAM->SetVal( DefaultEllipsePlugin::OpenAngleMode::CW, 0.f );

    return model;
}

std::string                     DefaultEllipsePluginDesc::UID                 ()
{
    return "DEFAULT_ELLIPSE";
}

// *******************************
//
IPluginPtr                      DefaultEllipsePluginDesc::CreatePlugin         ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultEllipsePlugin >( name, prev, timeEvaluator );
}

class EllipseGenerator : public IGeometryOnlyGenerator
{
    float quality;
    float radius1, radius2;

public:
    EllipseGenerator( float q, float r1, float r2 )
        : quality( q )
        , radius1( r1 ), radius2( r2 )
        {}

    IGeometryGenerator::Type GetType() { return IGeometryGenerator::Type::GEOMETRY_ONLY; }

    inline void AddPoint( double t, Float3AttributeChannelPtr& verts )
    {
        verts->AddAttribute( glm::vec3( radius1 * cos( t ), radius2 * sin( t ), 0 ) );
    }

    void GenerateGeometry( Float3AttributeChannelPtr verts ) 
    { 
        double t1 = 0, t2 = 2 * PI;
        double r1sq = radius1 * radius1;
        double r2sq = radius2 * radius2;

        while( t1 < t2 )
        {
            AddPoint( t2, verts );
            AddPoint( t1, verts );

            t1 += 1.0 / ( sqrt( r1sq * sin(t1)*sin(t1) + r2sq * cos(t1) * cos(t1) ) ) / quality;
            t2 -= 1.0 / ( sqrt( r1sq * sin(t2)*sin(t2) + r2sq * cos(t2) * cos(t2) ) ) / quality;
        }

        AddPoint( PI, verts );
    }
};

IGeometryGenerator*           DefaultEllipsePlugin::GetGenerator()
{
    return new EllipseGenerator( GetQuality(),
                                GetOuterRadius1(),
                                GetOuterRadius2() );
}

bool DefaultEllipsePlugin::NeedsTopologyUpdate()
{
    return
        GetCachedParameter( PN_QUALITY )->Changed() ||
        GetCachedParameter( PN_OPEN_ANGLE )->Changed() ||
        GetCachedParameter( PN_OUTER_RADIUS1 )->Changed() ||
        GetCachedParameter( PN_OUTER_RADIUS2 )->Changed();
        //|| GetCachedParameter( PN_OPEN_ANGLE_MODE )->Changed();
}

float DefaultEllipsePlugin::GetQuality()
{
    auto param = GetParameter( PN_QUALITY );
    auto qParam = QueryTypedParam< ParamFloatPtr >( param );
    return qParam->Evaluate();
}

float DefaultEllipsePlugin::GetOuterRadius1()
{
    auto param = GetParameter( PN_OUTER_RADIUS1 );
    auto qParam = QueryTypedParam< ParamFloatPtr >( param );
    return qParam->Evaluate();
}

float DefaultEllipsePlugin::GetOuterRadius2()
{
    auto param = GetParameter( PN_OUTER_RADIUS2 );
    auto qParam = QueryTypedParam< ParamFloatPtr >( param );
    return qParam->Evaluate();
}

float DefaultEllipsePlugin::GetOpenAngle()
{
    auto param = GetParameter( PN_OPEN_ANGLE );
    auto qParam = QueryTypedParam< ParamFloatPtr >( param );
    return qParam->Evaluate();
}

//DefaultEllipsePlugin::OpenAngleMode DefaultEllipsePlugin::GetOpenAngleMode()
//{
//    auto param = GetParameter( PN_OPEN_ANGLE_MODE );
//    auto qParam = QueryTypedParam< std::shared_ptr< ParamEnum< DefaultEllipsePlugin::OpenAngleMode > > >( param );
//    return qParam->Evaluate();
//}

} }