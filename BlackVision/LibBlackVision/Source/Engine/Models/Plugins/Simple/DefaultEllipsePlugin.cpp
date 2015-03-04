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

class OldEllipseGenerator : public IGeometryOnlyGenerator
{
    float quality;
    float radius1, radius2;

public:
    OldEllipseGenerator( float q, float r1, float r2 )
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

class EllipseGenerator : public IGeometryOnlyGenerator
{
    float quality;
    float radius1, radius2; // border is parametrized as [ x, y ] = [ radius1*cos(t), radius2*sin(t) ]

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

    inline double f( double x )
    {
        double a = radius1;
        double b = radius2;

        double t = acos( x / a );
        return b*sin(t);
    }

    inline double I( double x )
    {
        double a = radius1;
        double b = radius2;

        double l = x * sqrt( 1 - pow( x / a , 2 ) );
        double r = a * asin(x / a);
        
        return  b/2 * (l + r );
    }

    inline double E( double x1, double x2 )
    {
        double I_ = I(x2) - I(x1);

        double T = (x2-x1)/2*(f(x1)+f(x2));

        return I_ - T;
    }

    static double x1, preferredArea; // FIXME?
    inline double N( double x2 )
    {
        auto val = E( x1, x2 ) - preferredArea;
        //printf( "%f\n", float( val ) );
        return val;
    }

    double Newton( double t )
    {
        x1 = radius1 * cos( t ); 

        double a = -radius1, b = x1;

        preferredArea = 1000 / quality;

        const double eps = 0.01;

        while( N(a) * N(b) > 0 && abs( N(a) ) > eps && b-a > eps )
        {
            double mid = ( a + b )/2;
            if( x1 < mid )
                b = mid;
            else
                a = mid;
        }

        while( abs( N( a ) ) > eps && b-a > eps )
        {
            assert( N(a) * N(b) < 0 );
            double mid = ( a + b )/2;
            if( N(a) * N(mid) < 0 )
                b = mid;
            else
                a = mid;
        }

        return acos( a / radius1 );
    }

    void GenerateGeometry( Float3AttributeChannelPtr verts ) 
    { 
        double t1 = 0, t2 = 2 * PI;

        while( t1 < PI )
        {
            AddPoint( t2, verts );
            AddPoint( t1, verts );

            t1 = Newton( t1 );
            t2 = 2 * PI - t1;
        }

        AddPoint( PI, verts );
    }
};

double EllipseGenerator::x1;
double EllipseGenerator::preferredArea;

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