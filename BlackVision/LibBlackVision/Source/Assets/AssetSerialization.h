#pragma once

#include <assert.h>

#include "Serialization/ISerializable.h"

#include "Texture/TextureAssetDescriptor.h"
#include "Texture/AnimationAssetDescriptor.h"
#include "Font/FontAssetDescriptor.h"

namespace bv {

class SerializedAssetUID : public ISerializable
{
    std::string uid;

public:
    SerializedAssetUID( std::string uid ) : uid( uid ) {}

    std::string                         GetUID() { return uid; }

    virtual void                Serialize       ( ISerializer& sob ) const
    {
        sob.EnterChild( "asset" );
        sob.SetAttribute( "uid", uid );
        sob.ExitChild(); // asset
    }

    static ISerializablePtr             Create( IDeserializer& dob )
    {
        return ISerializablePtr( new SerializedAssetUID( dob.GetAttribute( "uid" ) ) ); // make_shared does not work for some reason ;)
    }
};

class SerializedAssetDesc : public AssetDesc {
public:
    static ISerializableConstPtr Create( IDeserializer& dob )
    {
        if( dob.GetAttribute( "type" ) == "tx" )
            return TextureAssetDesc::Create( dob );
        else if( dob.GetAttribute( "type" ) == "anim" )
            return AnimationAssetDesc::Create( dob );
        else if( dob.GetAttribute( "type" ) == "font" )
            return FontAssetDesc::Create( dob );
        else
        {
            assert( false );
            return nullptr;
        }
    }
};

}
