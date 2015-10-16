#include "Assets/Assets.h"

namespace bv {

struct TextureUtils
{

	static TextureAssetConstPtr			GenerateMipMaps					( SingleTextureAssetConstPtr texture, Int32 numLevels, MipMapFilterType ft );

	static TextureFormat				ToTextureFormat					( UInt32 bpp, UInt32 channelNum );
	static TextureFormat				NearestSupportedTextureFormat	( TextureFormat format );
	static UInt32						ToBPP							( TextureFormat format );

	static MemoryChunkConstPtr		    LoadImage						( const std::string & path, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum );
	static SingleTextureAssetConstPtr	LoadSingleTexture				( const SingleTextureAssetDescConstPtr & singleTextureResDesc, bool cacheOnDisk );

	static TextureAssetConstPtr			LoadOrginalTextureOnly			( const TextureAssetDescConstPtr & desc );
	static TextureAssetConstPtr			LoadTextureAndMipMaps			( const TextureAssetDescConstPtr & desc );
	static TextureAssetConstPtr			LoadTextureAndGenerateMipMaps	( const TextureAssetDescConstPtr & desc );

    static TextureAssetConstPtr         GetFromRawDataCache             ( const TextureAssetDescConstPtr & desc );
    static SingleTextureAssetConstPtr	GetFromRawDataCache             ( const SingleTextureAssetDescConstPtr & desc );
    static void                         AddToRawDataCache               ( const TextureAssetConstPtr & textureRes );
};

}  // bv