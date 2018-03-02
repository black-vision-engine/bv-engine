#include "stdafx.h"

#include "AudioInputAssetDesc.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"




namespace bv
{

const std::string   AudioInputAssetDesc::uid = "AUDIO_INPUT_ASSET_DESC";



// ***********************
//
AudioInputAssetDesc::AudioInputAssetDesc( InputSlotBinding binding )
    : m_binding( binding )
{}


// ***********************
//
AudioInputAssetDescPtr	AudioInputAssetDesc::Create   ( InputSlotBinding binding )
{
    return AudioInputAssetDescPtr( new AudioInputAssetDesc( binding ) );
}

// ***********************
//
void                AudioInputAssetDesc::Serialize        ( ISerializer & ser ) const
{
    ser.EnterChild( "asset" );

    ser.SetAttribute( "type", UID() );

    m_binding.Serialize( ser );

    ser.ExitChild();
}

// ***********************
//
void                AudioInputAssetDesc::Deserialize      ( const IDeserializer & sob )
{
    auto obj = SerializationHelper::Create< const AudioInputAssetDesc >( sob );

    if( obj == nullptr || obj->GetUID() != uid )
    {
        assert( !"Failed" ); //FIXME: Error handling.
        return;
    }

    *this = *obj; // if you think this is ugly, see why it cannot be done:
}


// ***********************
//
ISerializableConstPtr       AudioInputAssetDesc::Create          ( const IDeserializer & deser )
{
    auto binding = InputSlotBinding::Create( deser );

    if( binding.IsValid() )
        return AudioInputAssetDesc::Create( binding.GetVal() );
    else
        return nullptr;
}


// ***********************
//
const std::string &	        AudioInputAssetDesc::GetUID() const
{
    return AudioInputAssetDesc::UID();
}

// ***********************
//
const std::string &         AudioInputAssetDesc::UID()
{
    return AudioInputAssetDesc::uid;
}


// ***********************
//
bool                        AudioInputAssetDesc::IsCacheable() const
{
    return false;
}

// ***********************
//
VoidConstPtr                AudioInputAssetDesc::QueryThis() const
{
    return shared_from_this();
}



// ***********************
//
std::string				AudioInputAssetDesc::GetKey		() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}


// ***********************
//
std::string             AudioInputAssetDesc::ComputeKey   () const
{
    return "AudioInputAsset" + m_binding.GetName() + Convert::T2String( m_binding.GetIndex() );
}

// ***********************
//
std::string             AudioInputAssetDesc::GetProposedShortKey () const
{
    return GetKey();
}

// ***********************
//
SizeType                AudioInputAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    assert( false );

    return assetSize;
}



} // bv

