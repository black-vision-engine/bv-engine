#pragma once

#include "Assets/AssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"

namespace bv {

class AnimationAssetDesc : public AssetDesc, public std::enable_shared_from_this< AnimationAssetDesc >
{
private:

	static const std::string		uid;

protected:

	virtual VoidConstPtr            QueryThis	() const override;

public:

    virtual void                Serialize       ( SerializeObject & sob ) const;
    static ISerializableConstPtr     Create          ( DeserializeObject & dob );

	virtual const std::string &		GetUID		() const override;

	virtual bool					IsCacheable	() const override;

	static const std::string &		UID			();

public:
                                                
                                                ~AnimationAssetDesc	();

	const std::vector< TextureAssetDescConstPtr > &  GetFrames		() const;

    static AnimationAssetDescConstPtr			CreateFromDirFrames ( const std::string & path, const std::string & filter );
    //static AnimationAssetDescConstPtr			Create ( const std::vector< TextureAssetDescConstPtr > & frames );

private:
	explicit						AnimationAssetDesc	( const std::vector< std::string > & frames );
	explicit						AnimationAssetDesc	( const std::vector< TextureAssetDescConstPtr > & frames );

    std::vector< TextureAssetDescConstPtr >  m_frames;

};

} //bv
