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

    virtual void                    Serialize       ( ISerializer& sob ) const;
    virtual void                    Deserialize     ( ISerializer& sob );
    static ISerializableConstPtr    Create          ( ISerializer& dob );

	virtual const std::string &		GetUID		() const override;

	virtual bool					IsCacheable	() const override;

    virtual std::string             GetKey      () const override;

	static const std::string &		UID			();

public:
                                                
                                                ~AnimationAssetDesc	();

    std::string                     GetPath		() const;
    std::string                     GetFilter   () const;

	//const std::vector< TextureAssetDescConstPtr > &  GetFrames		() const;

	//const std::vector< TextureAssetDescConstPtr > &  GetFrames		() const;

    static AnimationAssetDescConstPtr			Create ( const std::string & path, const std::string & filter );
    //static AnimationAssetDescConstPtr			Create ( const std::vector< TextureAssetDescConstPtr > & frames );

private:
	explicit						AnimationAssetDesc	( const std::string & frames, const std::string & filter );
	//explicit						AnimationAssetDesc	( const std::vector< TextureAssetDescConstPtr > & frames );

    //std::vector< TextureAssetDescConstPtr > m_frames;
    std::string                             m_path;
    std::string                             m_filter;
};

} //bv
