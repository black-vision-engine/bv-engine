#pragma once

#include "Assets/Asset.h"
#include "Assets/Font/Text.h"

#include "Assets/AssetManager.h"			// Only for LoadTypedAsset template specialization

namespace bv
{

class FontAsset;
DEFINE_CONST_PTR_TYPE( FontAsset )

// *******************************
// Implements font asset with full size atlas and all its mimmaps.
class FontAsset : public Asset, public std::enable_shared_from_this< FontAsset >
{
	static const std::string		uid;

protected:
	virtual VoidConstPtr			QueryThis		() const override;

public:

	// *******************************
	// Returns testure asset with atlas and mipmaps.
	TextConstPtr					GetText			() const;

	static FontAssetConstPtr		Create			( const TextConstPtr & textAtlas );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	explicit						FontAsset		( const TextConstPtr & textAtlas );

	TextConstPtr					m_text;
};



class FontAssetDesc;

template<> inline std::shared_ptr<const FontAsset> LoadTypedAsset<FontAsset>(  const AssetDescConstPtr & desc )
{
	if( typeid( FontAssetDesc ) == typeid( *( desc.get() ) ) )
	{
		auto asset = AssetManager::GetInstance().LoadAsset( desc );
		return std::static_pointer_cast<const FontAsset>( asset );
	}
	return nullptr;
}


} // bv