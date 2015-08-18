#include "Engine/Interfaces/ISerializable.h"

#include "Texture/TextureAssetDescriptor.h"

namespace bv {

namespace model {

class AssetSerialization : public AssetDesc {
public:
    static ISerializablePtr Create( DeserializeObject& dob )
    {
        return TextureAssetDesc::Create( dob );
    }
};

} }