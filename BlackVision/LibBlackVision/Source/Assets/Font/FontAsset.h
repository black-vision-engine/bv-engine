#pragma once

#include "Assets/Asset.h"
#include "Assets/Font/Text.h"

namespace bv
{

class FontAsset;
DEFINE_CONST_PTR_TYPE( FontAsset )

// *******************************
// Implements font resource with full size atlas and all its mimmaps.
class FontAsset : public model::Asset, public std::enable_shared_from_this< FontAsset >
{
	static const std::string		uid;

protected:
	virtual VoidConstPtr			QueryThis		() const override;

public:

	// *******************************
	// Returns testure resource with atlas and mipmaps.
	model::TextConstPtr				GetText			() const;

	static FontAssetConstPtr		Create			( const model::TextConstPtr & textAtlas );

	explicit						FontAsset	( const model::TextConstPtr & textAtlas );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	model::TextConstPtr				m_text;
};

} // bv