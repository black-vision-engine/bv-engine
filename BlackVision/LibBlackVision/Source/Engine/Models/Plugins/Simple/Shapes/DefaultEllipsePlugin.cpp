#include "stdafx.h"

#include "DefaultEllipsePlugin.h"

#include "Mathematics/Defines.h"

namespace bv { namespace model {

const std::string DefaultEllipsePlugin::PN_QUALITY = "tesselation";
const std::string DefaultEllipsePlugin::PN_OUTER_RADIUS1 = "outer radius 1";
const std::string DefaultEllipsePlugin::PN_OUTER_RADIUS2 = "outer radius 2";
const std::string DefaultEllipsePlugin::PN_OPEN_ANGLE = "open angle";
const std::string DefaultEllipsePlugin::PN_OPEN_ANGLE_MODE = "open angle mode";

typedef ParamEnum< DefaultEllipsePlugin::OpenAngleMode > ParamEnumOAM;

VoidPtr    ParamEnumOAM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultEllipsePlugin::OpenAngleMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultEllipsePlugin::OpenAngleMode >( name, timeline );
}


	
#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"

DefaultEllipsePluginDesc::DefaultEllipsePluginDesc()
        : DefaultGeometryPluginDescBase( UID(), "ellipse" ) { }


DefaultPluginParamValModelPtr   DefaultEllipsePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
	ModelHelper h( timeEvaluator );

    h.CreateVacModel();
	h.AddSimpleParam( DefaultEllipsePlugin::PN_QUALITY, 7.f, true, true );
    h.AddSimpleParam( DefaultEllipsePlugin::PN_OUTER_RADIUS1, 1.f, true, true );
    h.AddSimpleParam( DefaultEllipsePlugin::PN_OUTER_RADIUS2, 0.5f, true, true );
    h.AddSimpleParam( DefaultEllipsePlugin::PN_OPEN_ANGLE, 0.f, true, true );
	h.AddParam< IntInterpolator, DefaultEllipsePlugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( DefaultEllipsePlugin::PN_OPEN_ANGLE_MODE, DefaultEllipsePlugin::OpenAngleMode::CW, true, true );
    
    h.CreatePSModel();

    return h.GetModel();
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

class OldEllipseGenerator : public IGeometryNormalsGenerator
{
    float quality;
    float radius1, radius2;

public:
    OldEllipseGenerator( float q, float r1, float r2 )
        : quality( q )
        , radius1( r1 ), radius2( r2 )
        {}

    inline void AddPoint( double t, Float3AttributeChannelPtr& verts )
    {
        verts->AddAttribute( glm::vec3( radius1 * cos( t ), radius2 * sin( t ), 0 ) );
    }

    void GenerateGeometryNormals( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals ) 
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

        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }
};

class EllipseGenerator : public IGeometryNormalsGenerator
{
    float quality;
    float radius1, radius2; // border is parametrized as [ x, y ] = [ radius1*cos(t), radius2*sin(t) ]

public:
    EllipseGenerator( float q, float r1, float r2 )
        : quality( q )
        , radius1( r1 ), radius2( r2 )
        {}

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
        auto val = preferredArea - E( x2, x1 );
        //printf( "%f\n", float( val ) );
        return val;
    }

    double Newton( double t )
    {
        x1 = radius1 * cos( t ); 

        double a = -radius1, b = x1;

        preferredArea = 0.01 / quality;

        const double eps = 0.01 * preferredArea;

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

    void GenerateGeometryNormals( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals ) 
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

        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }
};

double EllipseGenerator::x1;
double EllipseGenerator::preferredArea;


DefaultEllipsePlugin::DefaultEllipsePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
	InitGeometry();
}


std::vector<IGeometryGeneratorPtr>           DefaultEllipsePlugin::GetGenerators()
{
    return std::vector<IGeometryGeneratorPtr>( 1, std::make_shared< OldEllipseGenerator >( GetQuality(),
                                GetOuterRadius1(),
                                GetOuterRadius2() ) );
    //return new EllipseGenerator( GetQuality(),
    //                            GetOuterRadius1(),
    //                            GetOuterRadius2() );
}

bool DefaultEllipsePlugin::NeedsTopologyUpdate()
{
    return
		ParameterChanged( PN_QUALITY ) ||
        ParameterChanged( PN_OPEN_ANGLE )||
        ParameterChanged( PN_OUTER_RADIUS1 )||
        ParameterChanged( PN_OUTER_RADIUS2 )
        || ParameterChanged( PN_OPEN_ANGLE_MODE );
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

DefaultEllipsePlugin::OpenAngleMode DefaultEllipsePlugin::GetOpenAngleMode()
{
    auto param = GetParameter( PN_OPEN_ANGLE_MODE );
    auto qParam = QueryTypedParam< std::shared_ptr< ParamEnum< DefaultEllipsePlugin::OpenAngleMode > > >( param );
    return qParam->Evaluate();
}

} }