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

template<>
inline bool SetParameter< DefaultCirclePlugin::OpenAngleMode >( IParameterPtr param, TimeType t, const DefaultCirclePlugin::OpenAngleMode & val )
{
    //return SetSimpleTypedParameter< ParamEnum<DefaultCirclePlugin::OpenAngleMode> >( param, t, val );
    typedef ParamEnum<DefaultCirclePlugin::OpenAngleMode> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"
//template class SimpleParamValEvaluator< ParamEnumOAM, std::shared_ptr< ValueImpl< DefaultCirclePlugin::OpenAngleMode, ParamType::PT_ENUM > > >;
//template class SimpleParamValEvaluator< ParamFloatPtr, ValueFloatConstPtr >;
//template class bv::model::SimpleParamValEvaluator<bv::model::ParamFloatPtr, bv::ValueFloatPtr>;

DefaultPluginParamValModelPtr   DefaultCirclePluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr         vacModel    = CreateVacModel( model, timeEvaluator );//std::make_shared< DefaultParamValModel >();

    model->SetVertexAttributesChannelModel( vacModel ); // FIXME

    //AddParam( vacModel, timeEvaluator, DefaultCirclePlugin::PN_OUTER_RADIUS, 1.f, true, true );
    //AddParam( vacModel, timeEvaluator, DefaultCirclePlugin::PN_INNER_RADIUS, 0.f, true, true );
    //AddParam( vacModel, timeEvaluator, DefaultCirclePlugin::PN_OPEN_ANGLE, 360.f, true, true );
    //AddParam( vacModel, timeEvaluator, DefaultCirclePlugin::PN_TESSELATION, 3, true, true );
    AddParam< IntInterpolator, DefaultCirclePlugin::OpenAngleMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumOAM >
        ( vacModel, timeEvaluator, DefaultCirclePlugin::PN_OPEN_ANGLE_MODE, DefaultCirclePlugin::OpenAngleMode::CW, true, true );

    //ParamIntPtr paramN      = ParametersFactory::CreateParameterInt( DefaultCirclePlugin::PN_TESSELATION, timeEvaluator );
    auto paramOAM = ParametersFactory::CreateParameterEnum< DefaultCirclePlugin::OpenAngleMode >( DefaultCirclePlugin::PN_OPEN_ANGLE_MODE, timeEvaluator );

    //vacModel->AddParameter( paramN );
    vacModel->AddParameter( paramOAM );

    //paramN->SetVal( 3, 0.f );
    paramOAM->SetVal( DefaultCirclePlugin::OpenAngleMode::CW, 0.f );


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

IGeometryGenerator*           DefaultCirclePlugin::GetGenerator()
{
    //return new CircleGenerator( GetTesselation(),
    //                            GetInnerRadius(),
    //                            GetOuterRadius(),
    //                            GetOpenAngle(),
    //                            GetOpenAngleMode() );
    return new CircleGenerator( 3,
                                0,
                                1,
                                90,
                                GetOpenAngleMode() );
}

bool DefaultCirclePlugin::NeedsTopologyUpdate()
{
    return
        ParameterChanged( PN_OPEN_ANGLE_MODE ) /*||
        ParameterChanged( PN_TESSELATION ) ||
        ParameterChanged( PN_OUTER_RADIUS ) ||
        ParameterChanged( PN_INNER_RADIUS ) ||
        ParameterChanged( PN_OPEN_ANGLE )*/;
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