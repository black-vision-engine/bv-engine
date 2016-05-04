#include "NodeLogicBase.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"

#include "UseLoggerBVAppModule.h"


namespace bv { namespace model
{

// ***********************
//
NodeLogicBase::NodeLogicBase()
{
    m_paramValModel = std::make_shared< model::DefaultParamValModel >();
}

// ***********************
//
NodeLogicBase::~NodeLogicBase()
{}


// ***********************
//
model::IParameterPtr                            NodeLogicBase::GetParameter        ( const std::string & name ) const
{
    return m_paramValModel->GetParameter( name );
}

// ***********************
//
const std::vector< model::IParameterPtr > &     NodeLogicBase::GetParameters    () const
{
    return m_paramValModel->GetParameters();
}

// ***********************
//
void                                            NodeLogicBase::Serialize       ( ISerializer & ser ) const
{
    ser.EnterArray( "params" );
    {
        if( m_paramValModel ) 
        {
            for( auto param : m_paramValModel->GetParameters() )
                param->Serialize( ser );
        }
    }
    ser.ExitChild(); // params
}

// ***********************
//
void                                            NodeLogicBase::Deserialize     ( const IDeserializer & deser )
{
    // params
    auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
    for( auto param : params )
    {
        if( GetParameter( param->GetName() ) == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "Logic " << GetType() << " does not have parameter " << param->GetName() << ", which is serialized.";
        }

        m_paramValModel->SetParameter( param );
    }
}

}   // model
}	// bv
