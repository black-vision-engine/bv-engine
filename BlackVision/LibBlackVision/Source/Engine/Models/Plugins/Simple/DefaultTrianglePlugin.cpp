#include "DefaultTrianglePlugin.h"

namespace bv { namespace model {


DefaultTrianglePluginDesc::DefaultTrianglePluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "triangle" ) { }

IPluginPtr                      DefaultTrianglePluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTrianglePlugin >( name, prev, timeEvaluator );
}

DefaultPluginParamValModelPtr   DefaultTrianglePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr vacModel            = std::make_shared< DefaultParamValModel >();

    auto paramPA    = ParametersFactory::CreateParameterVec3( DefaultTrianglePlugin::PN_POINTA, timeEvaluator );
    auto paramPB    = ParametersFactory::CreateParameterVec3( DefaultTrianglePlugin::PN_POINTB, timeEvaluator );
    auto paramPC    = ParametersFactory::CreateParameterVec3( DefaultTrianglePlugin::PN_POINTC, timeEvaluator );

    vacModel->AddParameter( paramPA );
    vacModel->AddParameter( paramPB );
    vacModel->AddParameter( paramPC );

    model->SetVertexAttributesChannelModel( vacModel );

    return model;
}

std::string                     DefaultTrianglePluginDesc::UID                 ()
{
    return "DEFAULT_TRIANGLE";
}


class TriangleGenerator : public IGeometryOnlyGenerator
{
    glm::vec3 pa_, pb_, pc_;
public:
    TriangleGenerator( glm::vec3 pa, glm::vec3 pb, glm::vec3 pc )
        : pa_( pa ), pb_( pb ), pc_( pc ) { }

    virtual Type GetType() override { return Type::GEOMETRY_ONLY; }

    virtual void GenerateGeometry( Float3AttributeChannelPtr verts ) override
    {
        verts->AddAttribute( pa_ );
        verts->AddAttribute( pb_ );
        verts->AddAttribute( pc_ );
    }
};

std::vector<IGeometryGenerator*>    DefaultTrianglePlugin::GetGenerators()
{
    return std::vector<IGeometryGenerator*>( 1, new TriangleGenerator( GetPointA(), GetPointB(), GetPointC() ) );
}

bool                                DefaultTrianglePlugin::NeedsTopologyUpdate()
{
    return GetCachedParameter( PN_POINTA )->Changed()
        || GetCachedParameter( PN_POINTB )->Changed()
        || GetCachedParameter( PN_POINTC )->Changed();
}


glm::vec3                                   DefaultTrianglePlugin::GetPointA()
{
    auto param = GetParameter( PN_POINTA );
    auto qParam = QueryTypedParam< ParamVec3Ptr >( param );
    return qParam->Evaluate();
}

glm::vec3                                   DefaultTrianglePlugin::GetPointB()
{
    auto param = GetParameter( PN_POINTB );
    auto qParam = QueryTypedParam< ParamVec3Ptr >( param );
    return qParam->Evaluate();
}

glm::vec3                                   DefaultTrianglePlugin::GetPointC()
{
    auto param = GetParameter( PN_POINTC );
    auto qParam = QueryTypedParam< ParamVec3Ptr >( param );
    return qParam->Evaluate();
}

const std::string DefaultTrianglePlugin::PN_POINTA = "point a";
const std::string DefaultTrianglePlugin::PN_POINTB = "point b";
const std::string DefaultTrianglePlugin::PN_POINTC = "point c";

} }