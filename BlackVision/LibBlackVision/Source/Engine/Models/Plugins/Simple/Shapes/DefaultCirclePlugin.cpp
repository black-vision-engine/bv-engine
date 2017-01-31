#include "stdafx.h"

#include "DefaultCirclePlugin.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Mathematics/Defines.h"

#include "Memory/MemoryLeaks.h"



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


DefaultPluginParamValModelPtr   DefaultCirclePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreateVacModel();
    h.AddSimpleParam( DefaultCirclePlugin::PN_OUTER_RADIUS, 1.f, true, true );
    h.AddSimpleParam( DefaultCirclePlugin::PN_INNER_RADIUS, 0.f, true, true );
    h.AddSimpleParam( DefaultCirclePlugin::PN_OPEN_ANGLE, 360.f, true, true );
    h.AddSimpleParam( DefaultCirclePlugin::PN_TESSELATION, 10, true, true );
    h.AddParam< IntInterpolator, DefaultCirclePlugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( DefaultCirclePlugin::PN_OPEN_ANGLE_MODE, DefaultCirclePlugin::OpenAngleMode::CW, true, true );
    
    h.SetOrCreatePSModel();

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

// *******************************
//
class CircleGenerator : public IGeometryNormalsUVsGenerator
{

private:

    int         m_tesselation;
    float       m_innerRadius, m_outerRadius;
    double      m_totalAngle;
    DefaultCirclePlugin::OpenAngleMode m_mode;

public:

    CircleGenerator                 ( int n, float ir, float or, float oa, DefaultCirclePlugin::OpenAngleMode m )
        : m_tesselation( n )
        , m_innerRadius( ir )
        , m_outerRadius( or )
        , m_totalAngle ( oa / 360.f * 2 * PI )
        , m_mode( m )
        {}

    void GenerateGeometryNormalsUVs( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr normals, Float2AttributeChannelPtr uvs )
    {
        if( m_tesselation < 1 )
            m_tesselation = 1;

        double angleOffset;
        if( m_mode == DefaultCirclePlugin::OpenAngleMode::CW )
            angleOffset = 0;
        else if( m_mode == DefaultCirclePlugin::OpenAngleMode::CCW )
            angleOffset = m_totalAngle;
        else if( m_mode == DefaultCirclePlugin::OpenAngleMode::SYMMETRIC )
            angleOffset = m_totalAngle/2;
        else
        {
            assert( false );
            return;
        }

        float uvOuter = 0.5;
        float uvInner = uvOuter * m_innerRadius / m_outerRadius;

        for( int i = 0; i <= m_tesselation; i++ )
        {
            double angle = i * m_totalAngle / m_tesselation;
            angle -= angleOffset;

            double cosAngle = cos( angle );
            double sinAngle = sin( angle );

            glm::vec3 unitVector = glm::vec3( cosAngle, sinAngle, 0 );

            verts->AddAttribute( unitVector * m_innerRadius );
            verts->AddAttribute( unitVector * m_outerRadius );

            uvs->AddAttribute( glm::vec2( 0.5f + uvInner * cosAngle, 0.5 + uvInner * sinAngle ) );
            uvs->AddAttribute( glm::vec2( 0.5f + uvOuter * cosAngle, 0.5 + uvOuter * sinAngle ) );
        }

        GeometryGeneratorHelper::GenerateNonWeightedNormalsFromTriangleStrips( verts, normals );
    }
};


DefaultCirclePlugin::DefaultCirclePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
    : DefaultGeometryPluginBase( name, uid, prev, model )
{
		InitGeometry();
}


std::vector<IGeometryGeneratorPtr>           DefaultCirclePlugin::GetGenerators()
{
    return std::vector<IGeometryGeneratorPtr>( 1, std::make_shared< CircleGenerator >( GetTesselation(),
                                GetInnerRadius(),
                                GetOuterRadius(),
                                GetOpenAngle(),
                                GetOpenAngleMode() ) );
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