#include "stdafx.h"

#include "TextureInputAssetDesc.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string   TextureInputAssetDesc::uid = "TEXTURE_INPUT_ASSET_DESC";



// ***********************
//
TextureInputAssetDesc::TextureInputAssetDesc( InputSlotBinding binding )
    :   m_binding( binding )
{}


// ***********************
//
TextureInputAssetDescPtr	TextureInputAssetDesc::Create   ( InputSlotBinding binding )
{
    return TextureInputAssetDescPtr( new TextureInputAssetDesc( binding ) );
}

  // ***********************
  //
void                TextureInputAssetDesc::Serialize        ( ISerializer & ser ) const
{
    ser.EnterChild( "asset" );

    m_binding.Serialize( ser );

    ser.ExitChild();
}

// ***********************
//
void                TextureInputAssetDesc::Deserialize      ( const IDeserializer & sob )
{
    auto obj = SerializationHelper::Create< const TextureInputAssetDesc >( sob );

    if( obj == nullptr || obj->GetUID() != uid )
    {
        assert( !"Failed" ); //FIXME: Error handling.
        return;
    }

    *this = *obj; // if you think this is ugly, see why it cannot be done:
}


// ***********************
//
ISerializableConstPtr       TextureInputAssetDesc::Create          ( const IDeserializer & deser )
{
    auto binding = InputSlotBinding::Create( deser );

    if( binding.IsValid() )
        return TextureInputAssetDesc::Create( binding.GetVal() );
    else
        return nullptr;
}


// ***********************
//
const std::string &	        TextureInputAssetDesc::GetUID() const
{
    return TextureInputAssetDesc::UID();
}

// ***********************
//
const std::string &         TextureInputAssetDesc::UID()
{
    return TextureInputAssetDesc::uid;
}


// ***********************
//
bool                        TextureInputAssetDesc::IsCacheable() const
{
    return false;
}

// ***********************
//
VoidConstPtr                TextureInputAssetDesc::QueryThis() const
{
    return shared_from_this();
}



// ***********************
//
std::string				TextureInputAssetDesc::GetKey		() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}


// ***********************
//
std::string             TextureInputAssetDesc::ComputeKey   () const
{
    return "TextureInputAsset" + m_binding.GetName() + Convert::T2String( m_binding.GetIndex() );
}

// ***********************
//
std::string             TextureInputAssetDesc::GetProposedShortKey () const
{
    return GetKey();
}

// ***********************
//
SizeType                TextureInputAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    assert( false );

    return assetSize;
}



} // bv

