#pragma once

#include "XML/BVXMLSerializer.h"
#include "XML/BVXMLDeserializer.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"

#include "Serialization/BV/BVSerializeContext.h"

#include "Serialization/SerializationHelper.h"
#include "BVDeserializeContext.h"

#include <sstream>

namespace bv {

namespace CloneViaSerialization {

template< typename T >
std::shared_ptr< T >                              Clone( const T* obj, std::string name, AssetDescsWithUIDsPtr assets, model::OffsetTimeEvaluatorPtr sceneTimeline )
{
    auto serCtx = BVSerializeContext::CreateContextFromEmptiness();
    JsonSerializeObject ser( serCtx );

    serCtx->SetAssets( assets );

    obj->Serialize( ser );

    auto deserCtx = BVDeserializeContext::CreateContextFromEmptiness();
    JsonDeserializeObject deser( std::move( ser ), deserCtx );
    
    deserCtx->SetAssets( assets );
    deserCtx->SetSceneTimeline( sceneTimeline );

    return SerializationHelper::DeserializeObject< T >( deser, name );
}

template< typename T >
std::shared_ptr< T >                              ClonePtr( const T* obj, std::string name, AssetDescsWithUIDsPtr assets, model::OffsetTimeEvaluatorPtr sceneTimeline )
{
    auto serCtx = BVSerializeContext::CreateContextFromEmptiness();
    JsonSerializeObject ser( serCtx );

    serCtx->SetAssets( assets );

    obj->Serialize( ser );

    auto deserCtx = BVDeserializeContext::CreateContextFromEmptiness();
    JsonDeserializeObject deser( std::move( ser ), deserCtx );

    deserCtx->SetAssets( assets );
    deserCtx->SetSceneTimeline( sceneTimeline );
    
    auto sucess = deser.EnterChild( name );
    assert( sucess ); { sucess; } // FIXME error handling
    auto clone = T::Create( deser );
    deser.ExitChild();
    return std::static_pointer_cast< T >( clone );
}

} }
