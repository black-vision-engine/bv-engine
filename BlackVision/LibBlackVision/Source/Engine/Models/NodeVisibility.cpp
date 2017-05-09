#include "stdafx.h"

#include "NodeVisibility.h"

namespace bv { namespace model
{

// ***********************
//
NodeVisibility::NodeVisibility  ()
    : m_paramValModel( nullptr )
    , m_hasMoreThanOneKey( false )
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
    m_visibleParam->SetVal( visible, 0.f );
}

// ***********************
//
bool                NodeVisibility::IsVisible       () const
{
    if( m_hasMoreThanOneKey )
        return m_visibleParam->Evaluate();
    else
        return m_isVisible;
}

// ***********************
//
void                NodeVisibility::AddKey          ( bool val, TimeType t )
{
    m_visibleParam->SetVal( val, t );

    m_hasMoreThanOneKey = m_visibleParam->GetNumKeys() > 1;

    m_isVisible = m_visibleParam->Evaluate();
}

// ***********************
//
void                NodeVisibility::RemoveKey       ( TimeType t )
{
    m_visibleParam->RemoveVal( t );

    m_hasMoreThanOneKey = m_visibleParam->GetNumKeys() > 1;
    m_isVisible = m_visibleParam->Evaluate();
}

// ***********************
//
void                NodeVisibility::InitializeVisibleParam  ()
{
    m_paramValModel = DefaultParamValModel::Create();

    m_visibleParam = ParametersFactory::CreateParameterBool( "isVisible", nullptr );

    m_paramValModel->AddParameter( m_visibleParam );
}

// ***********************
//
void                NodeVisibility::Serialize  ( ISerializer & ser ) const
{
    ser.EnterChild( "NodeVisibility" );

    ser.SetAttribute( "isVisible", SerializationHelper::T2String( m_isVisible ) );

    m_paramValModel->Serialize( ser );

    ser.ExitChild();
}

// ***********************
//
ISerializablePtr    NodeVisibility::Create      ( const IDeserializer & deser )
{
    if( deser.EnterChild( "NodeVisibility" ) )
    {
        auto ret = std::make_shared< NodeVisibility >();

        ret->m_isVisible = SerializationHelper::String2T< bool >( deser.GetAttribute( "isVisible" ) );

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