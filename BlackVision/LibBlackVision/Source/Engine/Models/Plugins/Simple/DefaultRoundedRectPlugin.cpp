#include "DefaultRoundedRectPlugin.h"

namespace bv { namespace model {

DefaultRoundedRectPluginDesc::DefaultRoundedRectPluginDesc() 
    : DefaultGeometryPluginDescBase( UID(), "rounded rect" ) { }

DefaultPluginParamValModelPtr   DefaultRoundedRectPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr model     = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr vacModel        = std::make_shared< DefaultParamValModel >();

    auto paramS                             = ParametersFactory::CreateParameterVec2( DefaultRoundedRectPlugin::PN_SIZE, timeEvaluator );
    auto paramB                             = ParametersFactory::CreateParameterVec4( DefaultRoundedRectPlugin::PN_BEVELS, timeEvaluator );

    vacModel->AddParameter( paramS );
    vacModel->AddParameter( paramB );

    model->SetVertexAttributesChannelModel( vacModel );

    paramS->SetVal( glm::vec2( 1.f, 1.f ), 0.f );
    paramB->SetVal( glm::vec4( 0, 0, 0, 0 ) ,0.f );

    return model;
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

class RoundedRectGenerator : public IGeometryOnlyGenerator
{
    glm::vec2 size;
    glm::vec4 bevels;
public:
    RoundedRectGenerator( glm::vec2 s, glm::vec4 b )
        : size( s ), bevels( b ) { }

    Type GetType() { return Type::GEOMETRY_ONLY; }

    void GenerateGeometry( Float3AttributeChannelPtr verts )
    {
        float sx = size[0] / 2, sy = size[1] / 2;
        glm::vec3 vertices[4] = {   glm::vec3( -sx, -sy, 0 ),
                                    glm::vec3(  sx, -sy, 0 ),
                                    glm::vec3(  sx,  sy, 0 ),
                                    glm::vec3( -sx,  sy, 0 ) };

        //glm::vec3 p = vertices[0] + ;
        verts->AddAttribute( vertices[ 0 ] );
        verts->AddAttribute( vertices[ 1 ] );
        verts->AddAttribute( vertices[ 2 ] );
        verts->AddAttribute( vertices[ 3 ] );
    }
};

IGeometryGenerator*                 DefaultRoundedRectPlugin::GetGenerator()
{
    return new RoundedRectGenerator( GetSize(), GetBevels() );
}

bool                                DefaultRoundedRectPlugin::NeedsTopologyUpdate()
{
    return GetCachedParameter( PN_SIZE )->Changed()
        || GetCachedParameter( PN_BEVELS )->Changed();
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

} }