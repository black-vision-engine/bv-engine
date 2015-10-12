#pragma once

#include "Assets/AssetDescriptor.h"
#include "Engine/Types/Enums.h"
#include "Serialization/Json/JsonSerializationObjects.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

class VideoStreamAssetDesc;
DEFINE_CONST_PTR_TYPE( VideoStreamAssetDesc )

class VideoStreamAssetDesc : public AssetDesc, public std::enable_shared_from_this< VideoStreamAssetDesc >
{
private:

	static const std::string	uid;

	std::string					m_streamPath;

	UInt32						m_width;
	UInt32						m_height;

	UInt32						m_frameRate;

protected:


	virtual VoidConstPtr						QueryThis					() const override;

public:
	explicit									VideoStreamAssetDesc		( const std::string & streamPath );
	explicit									VideoStreamAssetDesc		( const std::string & streamPath, UInt32 width, UInt32 height, UInt32 frameRate );


    virtual void								Serialize					( SerializeObject & sob ) const;
    virtual void							    Serialize					( JsonSerializeObject & sob ) const;
	static ISerializableConstPtr			    Create						( DeserializeObject & dob );
	static ISerializableConstPtr				Create						( JsonDeserializeObject & dob );

	virtual const std::string &					GetUID						() const override;
	
	virtual bool								IsCacheable					() const override;
	
	static const std::string &					UID							();

	const std::string &							GetStreamPath				() const;
	UInt32										GetWidth					() const;
	UInt32										GetHeight					() const;
	UInt32										GetFrameRate				() const;
	
	static VideoStreamAssetDescConstPtr			Create						( const std::string & streamPath );
	static VideoStreamAssetDescConstPtr			Create						( const std::string & streamPath, UInt32 width, UInt32 height, UInt32 frameRate );


private:
	//For internal use only
	template<class Serializer>		void									SerializeAsset			( Serializer& sob ) const;
	template<class Deserializer>	static VideoStreamAssetDescConstPtr		DeserializeAsset		( Deserializer& dob );
};

} // bv