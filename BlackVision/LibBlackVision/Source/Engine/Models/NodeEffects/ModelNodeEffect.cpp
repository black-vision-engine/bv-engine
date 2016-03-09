#include "stdafx.h"

#include "ModelNodeEffect.h"
#include "Serialization/SerializationHelper.h"
#include "Assets/Assets.h"

namespace bv { namespace model {


// ********************************
//
ModelNodeEffect::ModelNodeEffect  ( NodeEffectType type )
    : m_type( type )
    , m_paramValModel( std::make_shared< DefaultParamValModel >() )
{
}

// ********************************
//
ModelNodeEffect::~ModelNodeEffect  ()
{
}

// ********************************
//
void                                        ModelNodeEffect::Serialize          ( ISerializer & ser ) const
{
    ser.EnterChild( "effect" );
        ser.SetAttribute( "type", SerializationHelper::T2String< NodeEffectType >( GetType() ) );
        m_paramValModel->Serialize( ser );
    ser.ExitChild();
}

// ********************************
//
void                                        ModelNodeEffect::Update             ( TimeType t )
{
    { t; }
    m_paramValModel->Update();
}

// ********************************
//
NodeEffectType                              ModelNodeEffect::GetType            () const
{
    return m_type;
}

// ********************************
//
void                                        ModelNodeEffect::RegisterEvaluator  ( IParamValEvaluatorPtr paramEval )
{
    m_paramValModel->RegisterAll( paramEval );
}

// ********************************
//
IParameterPtr                               ModelNodeEffect::GetParameter       ( const std::string & name ) const
{
    return m_paramValModel->GetParameter( name );
}

// ********************************
//
const std::vector< IParameterPtr > &        ModelNodeEffect::GetParameters      () const
{
    return m_paramValModel->GetParameters();
}

// ********************************
//
const std::vector< IValueConstPtr > &       ModelNodeEffect::GetValues          () const
{
    return m_paramValModel->GetValues();
}

// ********************************
//
IModelNodeEffectPtr                         ModelNodeEffect::Create             ( NodeEffectType type )
{
    return std::make_shared< ModelNodeEffect >( type );
}

// ********************************
//
bool                                        ModelNodeEffect::AddAsset               ( const AssetDescConstPtr & assetDesc )
{
    m_assetsDescs.push_back( assetDesc );
    return true;
}


// ********************************
//
AssetDescVec                                ModelNodeEffect::GetAssets          () const
{
    return m_assetsDescs;
}

} // model
} // bv
