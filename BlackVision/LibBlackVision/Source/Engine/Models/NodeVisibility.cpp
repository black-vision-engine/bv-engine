#include "stdafx.h"

#include "NodeVisibility.h"

namespace bv { namespace model
{

namespace
{
static std::string NODE_VISIBILITY_PARAM_NAME = "isVisible";
}

// ***********************
//
NodeVisibility::NodeVisibility  ()
    : m_paramValModel( nullptr )
{
    InitializeVisibleParam();
}

// ***********************
//
NodeVisibility::~NodeVisibility ()
{}

// ***********************
//
void                NodeVisibility::SetVisible     ( bool visible )
{
    m_isVisible = visible;
}

// ***********************
//
bool                NodeVisibility::IsVisible       () const
{
    return m_isVisible;
}

// ***********************
//
void                NodeVisibility::InitializeVisibleParam  ()
{
    m_paramValModel = DefaultParamValModel::Create();

    m_paramValModel->AddParameter( ParametersFactory::CreateParameterBool( NODE_VISIBILITY_PARAM_NAME, nullptr ) );

    m_isVisibleValParamState = model::GetValueParamState< bool >( m_paramValModel.get(), NODE_VISIBILITY_PARAM_NAME );
}

// ***********************
//
IParameterPtr       NodeVisibility::GetVisibleParameter     ()
{
    return m_paramValModel->GetParameter( NODE_VISIBILITY_PARAM_NAME );
}

// ***********************
//
void                NodeVisibility::Serialize  ( ISerializer & ser ) const
{
    ser.EnterChild( "NodeVisibility" );

    ser.SetAttribute( NODE_VISIBILITY_PARAM_NAME, SerializationHelper::T2String( m_isVisible ) );

    m_paramValModel->Serialize( ser );

    ser.ExitChild();
}

// ***********************
//
void                NodeVisibility::Update          ( TimeType )
{
    if( m_isVisibleValParamState.Changed() )
        m_isVisible = m_isVisibleValParamState.GetValue();
}

// ***********************
//
ISerializablePtr    NodeVisibility::Create      ( const IDeserializer & deser )
{
    return CreateTyped( deser );
}

// ***********************
//
NodeVisibilityPtr   NodeVisibility::CreateTyped ( const IDeserializer & deser )
{
    if( deser.EnterChild( "NodeVisibility" ) )
    {
        auto ret = std::make_shared< NodeVisibility >();

        ret->m_isVisible = SerializationHelper::String2T< bool >( deser.GetAttribute( NODE_VISIBILITY_PARAM_NAME ) );

        auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
        for( auto param : params )
        {
            if( ret->m_paramValModel->GetParameter( param->GetName() ) == nullptr )
                LOG_MESSAGE( SeverityLevel::warning ) << "NodeVisibility does not have parameter " << param->GetName() << ", which is serialized.";
            else
                ret->m_paramValModel->SetParameter( param );
        }

        return ret;
    }
    else
        return std::make_shared< NodeVisibility >();
}

} // model
} // bv