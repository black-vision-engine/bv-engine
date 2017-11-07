#include "stdafx.h"

#include "ModelBaseLight.h"
#include "Engine/Models/Lights/HelperModelLights.h"
#include "UseLoggerLibBlackVision.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


// *************************************
//
                        ModelBaseLight::ModelBaseLight          ()
{
    m_paramModel = std::make_shared< DefaultParamValModel >();
}

// *************************************
//
void                    ModelBaseLight::Serialize             ( ISerializer & ser ) const
{
    ser.EnterChild( "light" );

    ser.SetAttribute( "type", Convert::T2String( GetType() ) );

    ser.EnterArray( "params" );
    for( auto param : m_paramModel->GetParameters() )
    {
        param->Serialize( ser );
    }
    ser.ExitChild(); // params

    ser.ExitChild(); // light
}

// ********************************
//
ISerializablePtr	    ModelBaseLight::Create				( const IDeserializer & deser )
{
    auto typeName = deser.GetAttribute( "type" );
    auto type = Convert::String2T< LightType >( typeName );

    // params
    auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );

    //FIXME: take first timeeval for light creation
    assert( params.size() > 0 );
    auto light = std::shared_ptr< ModelBaseLight >( HelperModelLights::CreateModelLight( type,  params[ 0 ]->GetTimeEvaluator() ) );

    for( auto param : params )
    {
        if( light->m_paramModel->GetParameter( param->GetName() ) == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "light " << typeName << " does not have parameter " << param->GetName() << ", which is serialized.";
        }
        light->m_paramModel->SetParameter( param );
    }

    return light;
}

// *************************************
//
void                    ModelBaseLight::Update                ( TimeType t )
{
    { t; }
    m_paramModel->Update();
}

// *************************************
//
std::vector< IParameterPtr > &  ModelBaseLight::GetParameters         ()
{
    return m_paramModel->GetParameters();
}

// *************************************
//
IParameterPtr           ModelBaseLight::GetParameter          ( const std::string & name )
{
    return m_paramModel->GetParameter( name );
}

// *************************************
//
const std::vector< IValueConstPtr > &  ModelBaseLight::GetValues     () const
{
    return m_paramModel->GetValues();
}

// *************************************
//
IValueConstPtr          ModelBaseLight::GetValue            ( const std::string & name ) const
{
    return m_paramModel->GetValue( name );
}

} //model
} //bv
