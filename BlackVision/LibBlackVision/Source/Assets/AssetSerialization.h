#pragma once

#include <assert.h>

#include "Engine/Interfaces/ISerializable.h"

#include "Texture/TextureAssetDescriptor.h"
#include "Texture/AnimationAssetDescriptor.h"

namespace bv {

class SerializedAssetUID : public ISerializable
{
    std::string uid;

public:
    SerializedAssetUID( std::string uid ) : uid( uid ) {}

    std::string                         GetUID() { return uid; }

    virtual void                Serialize       ( SerializeObject & sob ) const
    {
        sob.SetName( "asset" );
        sob.SetValue( "uid", uid );
        sob.Pop(); // asset
    }

    static ISerializablePtr             Create( DeserializeObject& dob )
    {
        return ISerializablePtr( new SerializedAssetUID( dob.GetValue( "uid" ) ) ); // make_shared does not work for some reason ;)
    }
};

class SerializedAssetDesc : public AssetDesc {
public:
    static ISerializableConstPtr Create( DeserializeObject& dob )
    {
        if( dob.GetValue( "type" ) == "tx" )
            return TextureAssetDesc::Create( dob );
        else if( dob.GetValue( "type" ) == "anim" )
            return AnimationAssetDesc::Create( dob );
        else
        {
            assert( false );
            return nullptr;
        }
    }
};

}
