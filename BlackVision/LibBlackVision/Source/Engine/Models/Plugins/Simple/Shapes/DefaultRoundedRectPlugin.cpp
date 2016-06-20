#include "stdafx.h"

#include "DefaultRoundedRectPlugin.h"

#include "Mathematics/Defines.h"

namespace bv { namespace model {

DefaultRoundedRectPluginDesc::DefaultRoundedRectPluginDesc() 
    : DefaultGeometryPluginDescBase( UID(), "rounded rect" ) { }

DefaultPluginParamValModelPtr   DefaultRoundedRectPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.CreateVacModel();
    h.AddSimpleParam( DefaultRoundedRectPlugin::PN_SIZE, glm::vec2( 1, 1 ), true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PN_BEVELS, glm::vec4( 0.1, 0.1, 0.1, 0.1 ), true, true );
    h.AddSimpleParam( DefaultRoundedRectPlugin::PN_TESSELATION, 10, true, true );

    h.CreatePSModel();

    return h.GetModel();
}

IPluginPtr                      DefaultRoundedRectPluginDesc::CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultRoundedRectPlugin >( name, prev, timeEvaluator );
}

 std::string                     DefaultRoundedRectPluginDesc::UID                 ()
 {
     return "DEFAULT_ROUNDEDRECT";
 }



const std::string DefaultRoundedRectPlugin::PN_SIZE = "size";
const std::string DefaultRoundedRectPlugin::PN_BEVELS = "bevels";
const std::string DefaultRoundedRectPlugin::PN_TESSELATION = "tesselation";

class RoundedRectGenerator : public IGeometryNormalsGenerator
{
    glm::vec2 size;
    glm::vec4 bevels;
    int tesselation;
public:
    RoundedRectGenerator( glm::vec2 s, glm::vec4 b, int tesselation )
        : size( s ), bevels( b ), tesselation( tesselation ) { }

private:
    float sx, sy;
    glm::vec3 centers[4];

    int GetNPoints()
    {
        return 4 * tesselation;
    }

    glm::vec3 GetPoint( int i )
    {
        assert( i < 4 * tesselation );
        int nCenter = i / tesselation;
        glm::vec3 center = centers[ nCenter ];

        double angle = (i-nCenter) *2*PI / ((tesselation-1) * 4);

        return center + glm::vec3( cos( angle ), sin( angle ), 0 ) * bevels[nCenter];
    }
public:
    void GenerateGeometryNormals( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals )
    {
        sx = size[0] / 2, sy = size[1] / 2;
        
        centers[0] = glm::vec3(  sx - bevels[0],  sy - bevels[0], 0 );
        centers[1] = glm::vec3( -sx + bevels[1],  sy - bevels[1], 0 );
        centers[2] = glm::vec3( -sx + bevels[2], -sy + bevels[2], 0 );
        centers[3] = glm::vec3(  sx - bevels[3], -sy + bevels[3], 0 );

        int nPoints = GetNPoints(), i, j;
        for( i = 0, j = nPoints-1 ; i < j; i++, j-- )
        {
            verts->AddAttribute( GetPoint( j ) );
            verts->AddAttribute( GetPoint( i ) );
        }
        if( i==j )
            assert( false );

        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }
};

DefaultRoundedRectPlugin::DefaultRoundedRectPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
	InitGeometry();
}


std::vector<IGeometryGeneratorPtr>                 DefaultRoundedRectPlugin::GetGenerators()
{
    return std::vector<IGeometryGeneratorPtr>( 1, std::make_shared< RoundedRectGenerator >( GetSize(), GetBevels(), GetTesselation() ) );
}

bool                                DefaultRoundedRectPlugin::NeedsTopologyUpdate()
{
    return ParameterChanged( PN_SIZE )
        || ParameterChanged( PN_BEVELS )
        || ParameterChanged( PN_TESSELATION );
}

glm::vec2                                   DefaultRoundedRectPlugin::GetSize()
{
    auto param = GetParameter( PN_SIZE );
    auto tParam = QueryTypedParam< ParamVec2Ptr >( param );
    return tParam->Evaluate();
}

glm::vec4                                   DefaultRoundedRectPlugin::GetBevels()
{
    auto param = GetParameter( PN_BEVELS );
    auto tParam = QueryTypedParam< ParamVec4Ptr >( param );
    return tParam->Evaluate();
}

int                                         DefaultRoundedRectPlugin::GetTesselation()
{
    auto param = GetParameter( PN_TESSELATION );
    auto tParam = QueryTypedParam< ParamIntPtr >( param );
    return tParam->Evaluate();
}

} }