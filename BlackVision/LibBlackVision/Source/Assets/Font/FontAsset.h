#pragma once

#include "Assets/Asset.h"
#include "Assets/Font/Text.h"
#include "Assets/Font/TextRepresentation.h"

#include "Assets/AssetManager.h"			// Only for LoadTypedAsset template specialization

namespace bv
{

class FontAsset;
DEFINE_CONST_PTR_TYPE( FontAsset )

// *******************************
// Implements font asset with full size atlas and all its mimmaps.
class FontAsset : public Asset
{
	static const std::string		uid;

protected:
public:

	// *******************************
	// Returns testure asset with atlas and mipmaps.
	TextConstPtr					GetText			() const;
	
	virtual const std::string &		GetUID			() const override;
	static const std::string &		UID				();

    explicit						FontAsset		( const TextConstPtr & textAtlas, TextRepresentationPtr representation );

protected:

	TextConstPtr					m_text;
    TextRepresentationPtr           m_representation;
};



} // bv