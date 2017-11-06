#include "stdafx.h"

#include "ModelNodeEffect.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Assets/Assets.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectFactory.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"

#include "UseLoggerLibBlackVision.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

std::string ModelNodeEffect::EFFECT_ENABLED_PARAM_NAME = "effectIsEnabled";

// ********************************
//
namespace 
{

// ********************************
//
std::map< NodeEffectType, UInt32 > EffectNumRequiredAssetsInit()
{
    std::map< NodeEffectType, UInt32 > m;
    // FIXME: nrl - fix this
    //    m[ NodeEffectType::NET_IMAGE_MASK ] = 1;

    return m;
}
static std::map< NodeEffectType, UInt32 > effectNumRequiredAssets = EffectNumRequiredAssetsInit();

// ********************************
//
UInt32 GetEffectNumRequiredAssets( NodeEffectType effectType)
{
    auto it = effectNumRequiredAssets.find( effectType );

    if( it != effectNumRequiredAssets.end() )
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

} // anonymous

// ********************************
//
ModelNodeEffect::ModelNodeEffect    ( NodeEffectType type )
    : m_type( type )
    , m_paramValModel( std::make_shared< DefaultParamValModel >() )
    , m_assetsDescs( GetEffectNumRequiredAssets( type ) )
{}

// ***********************
//
ModelNodeEffect::ModelNodeEffect    ( NodeEffectType type, DefaultParamValModelPtr model )
    : m_type( type )
    , m_paramValModel( model )
    , m_assetsDescs( GetEffectNumRequiredAssets( type ) )
{}

// ********************************
//
ModelNodeEffect::~ModelNodeEffect  ()
{
}

// ********************************
//
void                                        ModelNodeEffect::Serialize          ( ISerializer & ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

    ser.EnterChild( "effect" );
    ser.SetAttribute( "type", Convert::T2String< NodeEffectType >( GetType() ) );

    if( context->detailedInfo )
    {
        ser.SetAttribute( "numRequiredAssets", Convert::T2String< SizeType >( NumRequiredAssets() ) );
        m_paramValModel->Serialize( ser );

        if( m_assetsDescs.size() > 0 )
        {
            auto serContext = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
            ser.EnterArray( "assets" );
            for( auto ad : m_assetsDescs )
            {
                if( ad != nullptr )
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
            }
            ser.ExitChild(); // assets
        }
    }

    ser.ExitChild(); // effect
}

// ********************************
//
ModelNodeEffectPtr							ModelNodeEffect::CreateTyped 		( const IDeserializer & deser )
{
    auto typeStr = deser.GetAttribute( "type" );

    auto type = Convert::String2T< NodeEffectType >( typeStr, NodeEffectType::NET_DEFAULT );

    auto deserContext = Cast< BVDeserializeContext * >( deser.GetDeserializeContext() );

    if( type != NodeEffectType::NET_DEFAULT )
    {
        auto retI = ModelNodeEffectFactory::CreateModelNodeEffect( type, "", deserContext->GetSceneTimeline() );

        assert( std::dynamic_pointer_cast< ModelNodeEffect >( retI ) );

        auto ret = std::static_pointer_cast< ModelNodeEffect >( retI );

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
            auto idx = 0;
            do
            {
                deser.EnterChild( "asset" );

                auto uid = deser.GetAttribute( "uid" );

                AssetDescConstPtr assetDesc;
                if( uid != "" )
                {
                    assetDesc = deserContext->GetAssets()->UID2Asset( uid );
                }
                else
                {
                    assetDesc = AssetManager::GetInstance().CreateDesc( deser );
                }

                if( assetDesc )
                {
                    ret->AddAsset( assetDesc, idx++ );
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

// ***********************
//
ModelNodeEffectPtr                          ModelNodeEffect::Create             ( NodeEffectType type, DefaultParamValModelPtr model )
{
    return std::make_shared< ModelNodeEffect >( type, model );
}

// ********************************
//
bool                                        ModelNodeEffect::AddAsset           ( const AssetDescConstPtr & assetDesc, SizeType idx )
{
    if( idx < NumRequiredAssets() )
    {
        m_assetsDescs[ idx ] = assetDesc;
        return true;
    }
    else
    {
        return false;
    }
}

// ********************************
//
bool                                        ModelNodeEffect::RemoveAsset        ( SizeType idx )
{
    if( idx < m_assetsDescs.size() )
    {
        m_assetsDescs.erase( m_assetsDescs.begin() + idx );
        return true;
    }
    else
    {
        return false;
    }
}

// ********************************
//
AssetDescVec                                ModelNodeEffect::GetAssets          () const
{
    return m_assetsDescs;
}

// ********************************
//
UInt32                                      ModelNodeEffect::NumRequiredAssets  () const
{
    return GetEffectNumRequiredAssets( GetType() );
}

// ********************************
//
bool                                        ModelNodeEffect::IsEnabled           () const
{
    auto p = m_paramValModel->GetParameter( EFFECT_ENABLED_PARAM_NAME );

    assert( p );

    auto pTyped = QueryTypedParam< ParamBoolPtr >( p );

    return pTyped->Evaluate();
}

} // model
} // bv
