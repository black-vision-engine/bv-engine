#include "Engine/Interfaces/ISerializable.h"

#include "Texture/TextureAssetDescriptor.h"

namespace bv {

namespace model {

class AssetSerialization : public AssetDesc {
public:
    static ISerializableConstPtr Create( DeserializeObject& dob )
    {
        return TextureAssetDesc::Create( dob );
    }
};

} }