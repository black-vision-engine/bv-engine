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
    //virtual void                    Deserialize     ( const IDeserializer& sob );
    static ISerializableConstPtr    Create          ( const IDeserializer& dob );

	virtual const std::string &		GetUID		() const override;

	virtual bool					IsCacheable	() const override;

    virtual std::string             GetKey      () const override;

	static const std::string &		UID			();

    virtual SizeType                EstimateMemoryUsage () const override;

public:
                                                
                                                ~AnimationAssetDesc	();

    std::string                     GetPath		() const;
    std::string                     GetFilter   () const;
    SizeType                        GetNumFrames() const;

	//const std::vector< TextureAssetDescConstPtr > &  GetFrames		() const;

	//const std::vector< TextureAssetDescConstPtr > &  GetFrames		() const;

    static AnimationAssetDescConstPtr			Create ( const std::string & path, SizeType numFrames, UInt32 width, UInt32 height, const std::string & filter );
    static AnimationAssetDescConstPtr			Create ( const std::string & path, SizeType numFrames, const std::string & filter );
    //static AnimationAssetDescConstPtr			Create ( const std::vector< TextureAssetDescConstPtr > & frames );

private:
	explicit						AnimationAssetDesc	( const std::string & frames, SizeType numFrames, UInt32 width, UInt32 height, const std::string & filter );
    explicit						AnimationAssetDesc	( const std::string & frames, const std::string & filter );
	//explicit						AnimationAssetDesc	( const std::vector< TextureAssetDescConstPtr > & frames );

    //std::vector< TextureAssetDescConstPtr > m_frames;
    std::string                             m_path;
    std::string                             m_filter;

    SizeType                                m_numFrames;
    UInt32                                  m_width;
    UInt32                                  m_height;
};

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string& GetAssetDescUID<AnimationAsset>()
{
    return AnimationAssetDesc::UID();
}


} //bv
