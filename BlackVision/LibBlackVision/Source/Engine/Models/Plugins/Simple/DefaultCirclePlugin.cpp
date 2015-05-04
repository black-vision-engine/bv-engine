#include "DefaultCirclePlugin.h"

#include "Mathematics/Defines.h"

namespace bv { namespace model {

const std::string DefaultCirclePlugin::PN_TESSELATION = "tesselation";
const std::string DefaultCirclePlugin::PN_INNER_RADIUS = "inner radius";
const std::string DefaultCirclePlugin::PN_OUTER_RADIUS = "outer radius";
const std::string DefaultCirclePlugin::PN_OPEN_ANGLE = "open angle";
const std::string DefaultCirclePlugin::PN_OPEN_ANGLE_MODE = "open angle mode";


typedef ParamEnum< DefaultCirclePlugin::OpenAngleMode > ParamEnumOAM;

// *******************************
//
VoidPtr    ParamEnumOAM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultCirclePlugin::OpenAngleMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultCirclePlugin::OpenAngleMode >( name, timeline );
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"

DefaultPluginParamValModelPtr   DefaultCirclePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( DefaultCirclePlugin::PN_OUTER_RADIUS, 1.f, true, true );
    h.AddSimpleParam( DefaultCirclePlugin::PN_INNER_RADIUS, 0.f, true, true );
    h.AddSimpleParam( DefaultCirclePlugin::PN_OPEN_ANGLE, 360.f, true, true );
    h.AddSimpleParam( DefaultCirclePlugin::PN_TESSELATION, 10, true, true );
    h.AddParam< IntInterpolator, DefaultCirclePlugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( DefaultCirclePlugin::PN_OPEN_ANGLE_MODE, DefaultCirclePlugin::OpenAngleMode::CW, true, true );

    return h.GetModel();
}

std::string                     DefaultCirclePluginDesc::UID                 ()
{
    return "DEFAULT_CIRCLE";
}

// *******************************
//
IPluginPtr                      DefaultCirclePluginDesc::CreatePlugin         ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultCirclePlugin >( name, prev, timeEvaluator );
}

//void										DefaultCirclePlugin::InitGeometry()
//{
//    
//}

class OldCircleGenerator : public IGeometryOnlyGenerator
{
    int tesselation;

public:
    OldCircleGenerator( int n )
        : tesselation( n )
        {}

    IGeometryGenerator::Type GetType() { return IGeometryGenerator::Type::GEOMETRY_ONLY; }

    void GenerateGeometry( Float3AttributeChannelPtr verts ) 
    { 
        int i = 0, j = tesselation - 1;

        for( ; j >= i ; i++, j-- )
        {
            double angle1 = j * 2 * PI / tesselation;
            double angle2 = i * 2 * PI / tesselation;
            verts->AddAttribute( glm::vec3( cos( angle1 ), sin( angle1 ), 0 ) );
            if( i != j )
                verts->AddAttribute( glm::vec3( cos( angle2 ), sin( angle2 ), 0 ) );
        }
    }
};

class CircleGenerator : public IGeometryOnlyGenerator
{
    int tesselation;
    float inner_radius, outer_radius;
    double total_angle;
    DefaultCirclePlugin::OpenAngleMode mode;

public:
    CircleGenerator( int n, float ir, float or, float oa, DefaultCirclePlugin::OpenAngleMode m )
        : tesselation( n )
        , inner_radius( ir )
        , outer_radius( or )
        , total_angle ( oa / 360.f * 2 * PI )
        , mode( m )
        {}

    IGeometryGenerator::Type GetType() { return IGeometryGenerator::Type::GEOMETRY_ONLY; }

    void GenerateGeometry( Float3AttributeChannelPtr verts ) 
    {
        double angle_offset;
        if( mode == DefaultCirclePlugin::OpenAngleMode::CW )
            angle_offset = 0;
        else if( mode == DefaultCirclePlugin::OpenAngleMode::CCW )
            angle_offset = total_angle;
        else if( mode == DefaultCirclePlugin::OpenAngleMode::SYMMETRIC )
            angle_offset = total_angle/2;
        else
        {
            assert( false );
            return;
        }

        for( int i = 0; i <= tesselation; i++ )
        {
            double angle = i * total_angle / tesselation;
            angle -= angle_offset;

            glm::vec3 unitVector = glm::vec3( cos( angle ), sin( angle ), 0 );

            verts->AddAttribute( unitVector * inner_radius );
            verts->AddAttribute( unitVector * outer_radius );
        }
    }
};


DefaultCirclePlugin::DefaultCirclePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
		InitGeometry();
}


std::vector<IGeometryGeneratorPtr>           DefaultCirclePlugin::GetGenerators()
{
    return std::vector<IGeometryGeneratorPtr>( 1, IGeometryGeneratorPtr( new CircleGenerator( GetTesselation(),
                                GetInnerRadius(),
                                GetOuterRadius(),
                                GetOpenAngle(),
                                GetOpenAngleMode() ) ) );
}

bool DefaultCirclePlugin::NeedsTopologyUpdate()
{
    return
        ParameterChanged( PN_OPEN_ANGLE_MODE ) ||
        ParameterChanged( PN_TESSELATION ) ||
        ParameterChanged( PN_OUTER_RADIUS ) ||
        ParameterChanged( PN_INNER_RADIUS ) ||
        ParameterChanged( PN_OPEN_ANGLE );
}

int DefaultCirclePlugin::GetTesselation()
{
    auto param = GetParameter( PN_TESSELATION );
    auto intParam = QueryTypedParam< ParamIntPtr >( param );
    return intParam->Evaluate();
}

float DefaultCirclePlugin::GetInnerRadius()
{
    auto param = GetParameter( PN_INNER_RADIUS );
    auto qParam = QueryTypedParam< ParamFloatPtr >( param );
    return qParam->Evaluate();
}

float DefaultCirclePlugin::GetOuterRadius()
{
    auto param = GetParameter( PN_OUTER_RADIUS );
    auto qParam = QueryTypedParam< ParamFloatPtr >( param );
    return qParam->Evaluate();
}

float DefaultCirclePlugin::GetOpenAngle()
{
    auto param = GetParameter( PN_OPEN_ANGLE );
    auto qParam = QueryTypedParam< ParamFloatPtr >( param );
    return qParam->Evaluate();
}

DefaultCirclePlugin::OpenAngleMode DefaultCirclePlugin::GetOpenAngleMode()
{
    auto param = GetParameter( PN_OPEN_ANGLE_MODE );
    auto qParam = QueryTypedParam< std::shared_ptr< ParamEnum< DefaultCirclePlugin::OpenAngleMode > > >( param );
    return qParam->Evaluate();
}

} }