#include "stdafx.h"

#include "ModelNodeEffect.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Assets/Assets.h"

#include "UseLoggerLibBlackVision.h"

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

	if( m_assetsDescs.size() > 0 )
    {
		auto serContext = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
        ser.EnterArray( "assets" );
        for( auto ad : m_assetsDescs )
        {
            ser.EnterChild( "asset" );
			if( serContext->GetAssets() )
			{
				auto uid = serContext->GetAssets()->Key2UID( ad->GetKey() );
				ser.SetAttribute( "uid", uid );
			}
			else
			{
				ad->Serialize( ser );
			}

            ser.ExitChild(); // asset
        }
        ser.ExitChild(); // assets
    }

    ser.ExitChild(); // effect
}

// ********************************
//
ModelNodeEffectPtr							ModelNodeEffect::CreateTyped 		( const IDeserializer & deser )
{
	auto typeStr = deser.GetAttribute( "type" );

	auto type = SerializationHelper::String2T< int >( typeStr );

	if( type.isValid )
	{
		auto ret = ModelNodeEffect::Create( NodeEffectType( type.ham ) );

		// params
		auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
		for( auto param : params )
		{
			if( ret->m_paramValModel->GetParameter( param->GetName() ) == nullptr )
			{
				LOG_MESSAGE( SeverityLevel::warning ) << "effect " << typeStr << " does not have parameter " << param->GetName() << ", which is serialized.";
			}

			ret->m_paramValModel->SetParameter( param );
		}

		// assets
		if( deser.EnterChild( "assets" ) )
		{
			do
			{
				deser.EnterChild( "asset" );

				auto uid = deser.GetAttribute( "uid" );

				auto assetDesc = AssetManager::GetInstance().CreateDesc( deser );

				if( assetDesc )
				{
					ret->AddAsset( assetDesc );
				}

				deser.ExitChild(); // asset
			}
			while( deser.NextChild() );
        
			deser.ExitChild(); // assets
		}

		return ret;
	}
	else
	{
		return nullptr;
	}
}

// ********************************
//
ISerializablePtr							ModelNodeEffect::Create				( const IDeserializer & deser )
{
	return CreateTyped( deser );
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
ModelNodeEffectPtr                          ModelNodeEffect::Create             ( NodeEffectType type )
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
