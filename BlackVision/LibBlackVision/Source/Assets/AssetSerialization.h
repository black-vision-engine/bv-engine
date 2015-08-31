#include "Engine/Interfaces/ISerializable.h"

#include "Texture/TextureAssetDescriptor.h"
#include "Texture/AnimationAssetDescriptor.h"

namespace bv {

namespace model {

class AssetSerialization : public AssetDesc {
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

} }