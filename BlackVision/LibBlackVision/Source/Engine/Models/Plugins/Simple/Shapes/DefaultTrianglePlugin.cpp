#include "stdafx.h"

#include "DefaultTrianglePlugin.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


DefaultTrianglePluginDesc::DefaultTrianglePluginDesc()
    : DefaultGeometryPluginDescBase( UID(), "triangle" ) { }

IPluginPtr                      DefaultTrianglePluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTrianglePlugin >( name, prev, timeEvaluator );
}

DefaultPluginParamValModelPtr   DefaultTrianglePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
	ModelHelper h( timeEvaluator );

	h.SetOrCreateVacModel();
	h.AddSimpleParam( DefaultTrianglePlugin::PN_POINTA, glm::vec3( 0, 1, 0 ), true, true );
	h.AddSimpleParam( DefaultTrianglePlugin::PN_POINTB, glm::vec3( -1, 0, 0 ), true, true );
	h.AddSimpleParam( DefaultTrianglePlugin::PN_POINTC, glm::vec3( 1, 0, 0 ), true, true );

    h.SetOrCreatePSModel();

	return h.GetModel();
}

std::string                     DefaultTrianglePluginDesc::UID                 ()
{
    return "DEFAULT_TRIANGLE";
}


class TriangleGenerator : public IGeometryNormalsGenerator
{
    glm::vec3 pa_, pb_, pc_;
public:
    TriangleGenerator( glm::vec3 pa, glm::vec3 pb, glm::vec3 pc )
        : pa_( pa ), pb_( pb ), pc_( pc ) { }

    virtual void GenerateGeometryNormals( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals ) override
    {
        verts->AddAttribute( pa_ );
        verts->AddAttribute( pb_ );
        verts->AddAttribute( pc_ );

        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }
};

std::vector<IGeometryGeneratorPtr>    DefaultTrianglePlugin::GetGenerators()
{
    return std::vector< IGeometryGeneratorPtr >( 1, std::make_shared< TriangleGenerator >( GetPointA(), GetPointB(), GetPointC() ) );
}

bool                                DefaultTrianglePlugin::NeedsTopologyUpdate()
{
	return ParameterChanged( PN_POINTA )
        || ParameterChanged( PN_POINTB )
        || ParameterChanged( PN_POINTC );
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

DefaultTrianglePlugin::DefaultTrianglePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
    : DefaultGeometryPluginBase( name, uid, prev, model ) 
{ 
	InitGeometry();
}


const std::string DefaultTrianglePlugin::PN_POINTA = "point a";
const std::string DefaultTrianglePlugin::PN_POINTB = "point b";
const std::string DefaultTrianglePlugin::PN_POINTC = "point c";

} }