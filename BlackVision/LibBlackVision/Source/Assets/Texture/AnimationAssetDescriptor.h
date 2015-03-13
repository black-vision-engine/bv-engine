#pragma once

#include "Assets/AssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"

namespace bv {

class AnimationAssetDesc;
DEFINE_PTR_TYPE(AnimationAssetDesc)
DEFINE_CONST_PTR_TYPE(AnimationAssetDesc)

class AnimationAssetDesc : public AssetDesc, public std::enable_shared_from_this< AnimationAssetDesc >
{
private:

	static const std::string		uid;

protected:

	virtual VoidConstPtr            QueryThis	() const override;

public:

	virtual const std::string &		GetUID		() const override;

	virtual bool					IsCacheable	() const override;

	static const std::string &		UID			();

public:

                                                AnimationAssetDesc	( const std::vector< std::string > & frames );
                                                ~AnimationAssetDesc	();

	const std::vector< TextureAssetDescConstPtr > &  GetFrames               () const;

    static AnimationAssetDescConstPtr		CreateFromDirFrames     ( const std::string & path, const std::string & filter );

private:

    std::vector< TextureAssetDescConstPtr >  m_frames;

};

} //bv
