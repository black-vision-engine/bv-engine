#include "DefaultCirclePlugin.h"

#include "Mathematics/Defines.h"

namespace bv { namespace model {

const std::string DefaultCirclePlugin::PN_TESSELATION = "tesselation";
const std::string DefaultCirclePlugin::PN_INNER_RADIUS = "inner radius";
const std::string DefaultCirclePlugin::PN_OUTER_RADIUS = "outer radius";
const std::string DefaultCirclePlugin::PN_OPEN_ANGLE = "open angle";
const std::string DefaultCirclePlugin::PN_OPEN_ANGLE_MODE = "open angle mode";

DefaultPluginParamValModelPtr   DefaultCirclePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr         vacModel    = std::make_shared< DefaultParamValModel >();

    ParamIntPtr paramN      = ParametersFactory::CreateParameterInt( DefaultCirclePlugin::PN_TESSELATION, timeEvaluator );
    ParamFloatPtr paramOR   = ParametersFactory::CreateParameterFloat( DefaultCirclePlugin::PN_OUTER_RADIUS, timeEvaluator );
    ParamFloatPtr paramIR   = ParametersFactory::CreateParameterFloat( DefaultCirclePlugin::PN_INNER_RADIUS, timeEvaluator );
    ParamFloatPtr paramOA   = ParametersFactory::CreateParameterFloat( DefaultCirclePlugin::PN_OPEN_ANGLE, timeEvaluator );

    model->SetVertexAttributesChannelModel( vacModel );
    vacModel->AddParameter( paramN );
    vacModel->AddParameter( paramOR );
    vacModel->AddParameter( paramIR );
    vacModel->AddParameter( paramOA );

    paramN->SetVal( 3, 0.f );
    paramOR->SetVal( 1.f, 0.f );
    paramIR->SetVal( 0.f, 0.f );
    paramOA->SetVal( 360.f, 0.f );

    return model;
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

public:
    CircleGenerator( int n, float ir, float or, float oa )
        : tesselation( n )
        , inner_radius( ir )
        , outer_radius( or )
        , total_angle ( oa / 360.f * 2 * PI )
        {}

    IGeometryGenerator::Type GetType() { return IGeometryGenerator::Type::GEOMETRY_ONLY; }

    void GenerateGeometry( Float3AttributeChannelPtr verts ) 
    { 
        for( int i = 0; i <= tesselation; i++ )
        {
            double angle = i * total_angle / tesselation;

            glm::vec3 unitVector = glm::vec3( cos( angle ), sin( angle ), 0 );

            verts->AddAttribute( unitVector * inner_radius );
            verts->AddAttribute( unitVector * outer_radius );
        }
    }
};

IGeometryGenerator*           DefaultCirclePlugin::GetGenerator()
{
    return new CircleGenerator( GetTesselation(),
                                GetInnerRadius(),
                                GetOuterRadius(),
                                GetOpenAngle() );
}

bool DefaultCirclePlugin::NeedsTopologyUpdate()
{
    return true;
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

} }