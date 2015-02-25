#include "DefaultCirclePlugin.h"

#include "Mathematics/Defines.h"

namespace bv { namespace model {

#define TESSELATION_PARAM_NAME "tesselation"

DefaultPluginParamValModelPtr   DefaultCirclePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr         vacModel    = std::make_shared< DefaultParamValModel >();

    ParamIntPtr paramN             = ParametersFactory::CreateParameterInt( TESSELATION_PARAM_NAME, timeEvaluator );

    model->SetVertexAttributesChannelModel( vacModel );
    vacModel->AddParameter( paramN );

    paramN->SetVal( 3, 0.f );

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

class CircleGenerator : public IGeometryOnlyGenerator
{
    int tesselation;

public:
    CircleGenerator( int n )
        : tesselation( n )
        {}

    IGeometryGenerator::Type GetType() { return IGeometryGenerator::Type::GEOMETRY_ONLY; }

    void GenerateGeometry( Float3AttributeChannelPtr verts ) 
    { 
        for( int i = 0; i < tesselation; i++ )
        {
            double angle = i * 2 * PI / tesselation;
            verts->AddAttribute( glm::vec3( cos( angle ), sin( angle ), 0 ) );
        }
    }
};

IGeometryGenerator*           DefaultCirclePlugin::GetGenerator()
{
    int n = GetTesselation();
    return new CircleGenerator( n );
}

bool                                DefaultCirclePlugin::NeedsTopologyUpdate()
{
    return true;
}

int                                         DefaultCirclePlugin::GetTesselation()
{
    auto param = GetParameter( TESSELATION_PARAM_NAME );
    auto intParam = QueryTypedParam< ParamIntPtr >( param );
    return intParam->Evaluate();
}

} }