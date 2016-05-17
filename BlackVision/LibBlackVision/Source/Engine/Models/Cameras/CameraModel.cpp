#include "stdafx.h"
#include "CameraModel.h"

#include "Engine/Models/ModelHelper.h"

#include "UseLoggerLibBlackVision.h"



namespace bv { namespace model {


const std::string       CameraModel::PARAMETERS::IS_PERSPECTIVE         = "IsPerspective";
const std::string       CameraModel::PARAMETERS::VIEWPORT_HEIGHT        = "ViewportHeight";
const std::string       CameraModel::PARAMETERS::VIEWPORT_WIDTH         = "ViewportWidth";
const std::string       CameraModel::PARAMETERS::DIRECTION              = "Direction";
const std::string       CameraModel::PARAMETERS::POSITION               = "Position";
const std::string       CameraModel::PARAMETERS::FOV                    = "FOV";
const std::string       CameraModel::PARAMETERS::FAR_CLIPPING_PLANE     = "FarClippingPlane";
const std::string       CameraModel::PARAMETERS::NEAR_CLIPPING_PLANE    = "NearClippingPlane";

// ***********************
//
CameraModel::CameraModel( ITimeEvaluatorPtr timeEvaluator )
{
    m_paramModel = std::make_shared< DefaultParamValModel >();

    AddBoolParam( m_paramModel, timeEvaluator, PARAMETERS::IS_PERSPECTIVE, true, true );
    AddFloatParam( m_paramModel, timeEvaluator, PARAMETERS::FOV, 45.0f, true );
    AddFloatParam( m_paramModel, timeEvaluator, PARAMETERS::NEAR_CLIPPING_PLANE, 0.1f, true );
    AddFloatParam( m_paramModel, timeEvaluator, PARAMETERS::FAR_CLIPPING_PLANE, 100.0f, true );
    AddFloatParam( m_paramModel, timeEvaluator, PARAMETERS::VIEWPORT_WIDTH, 1920.0f, true );
    AddFloatParam( m_paramModel, timeEvaluator, PARAMETERS::VIEWPORT_HEIGHT, 1080.0f, true );
    AddVec3Param( m_paramModel, timeEvaluator, PARAMETERS::POSITION, glm::vec3( 0.0f, 0.0f, 0.0f ), true );
    AddVec3Param( m_paramModel, timeEvaluator, PARAMETERS::DIRECTION, glm::vec3( 0.0f, 0.0f, -1.0f ), true );
}

// ***********************
//
CameraModel::~CameraModel()
{}

// *************************************
//
void                            CameraModel::Serialize             ( ISerializer & ser ) const
{
    ser.EnterChild( "camera" );

    ser.EnterArray( "params" );
    for( auto param : m_paramModel->GetParameters() )
    {
        param->Serialize( ser );
    }
    ser.ExitChild(); // params

    ser.ExitChild(); // camera
}

// ********************************
//
ISerializablePtr	            CameraModel::Create                 ( const IDeserializer & deser )
{
    // params
    auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );

    //FIXME: take first timeeval for light creation
    assert( params.size() > 0 );
    auto camera = std::shared_ptr< CameraModel >();

    for( auto param : params )
    {
        if( camera->m_paramModel->GetParameter( param->GetName() ) == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "Camera does not have parameter " << param->GetName() << ", which is serialized.";
        }
        camera->m_paramModel->SetParameter( param );
    }

    return camera;
}

// *************************************
//
void                            CameraModel::Update                ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  CameraModel::GetParameters         ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr                   CameraModel::GetParameter          ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  CameraModel::GetValues       () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr                          CameraModel::GetValue       ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
}


} // model
} //bv
