#pragma once

#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/Font/Text.h"

namespace bv
{

class FontResource;
DEFINE_CONST_PTR_TYPE( FontResource )

// *******************************
// Implements font resource with full size atlas and all its mimmaps.
class FontResource : public model::IResourceNEW, public std::enable_shared_from_this< FontResource >
{
	static const std::string		uid;

protected:
	virtual VoidConstPtr			QueryThis		() const override;

public:

	// *******************************
	// Returns testure resource with atlas and mipmaps.
	model::TextAtlasConstPtr		GetAtlas		() const;

	static FontResourceConstPtr		Create			( const model::TextAtlasConstPtr & textAtlas );

	explicit						FontResource	( const model::TextAtlasConstPtr & textAtlas );
	
	virtual const std::string &		GetUID			() const override;

	static const std::string &		UID				();

private:

	model::TextAtlasConstPtr		m_textAtlas;
};

} // bv