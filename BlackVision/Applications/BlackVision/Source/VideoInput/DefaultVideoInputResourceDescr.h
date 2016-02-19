#pragma once
#include "IVideoInput.h"

namespace bv { namespace model {

	class DefaultVideoInputResourceDescr : public AssetDesc, public std::enable_shared_from_this< DefaultVideoInputResourceDescr >
	{
		DefaultTextureDescriptorPtr descr;
		IVideoInput *input;

	public:
		DefaultVideoInputResourceDescr( DefaultTextureDescriptorPtr d, IVideoInput *i ) : descr( d ), input( i ) {}
        virtual void                Serialize       ( ISerializer& /*sob*/ ) const { assert( !"implement me" ); return; }
        static ISerializablePtr     Create          ( ISerializer& /*dob*/ ) { assert( !"implement me" ); return nullptr; }

		virtual PluginResourceType      GetResourceType () const { return PluginResourceType::PRT_VIDEOINPUT; }

		virtual  DefaultTextureDescriptorPtr     GetITextureDescriptor  () const /*override*/ { return descr; }
        virtual  SizeType                        EstimateMemoryUsage    () const override;

		//virtual void Update() const override { input->Update(); }

protected:
	virtual VoidConstPtr            QueryThis	() const override { return shared_from_this(); }

    static std::string uid;
public:
    virtual const std::string &		GetUID		() const override { return uid; }

    virtual bool					IsCacheable	() const override { return false; }

    static const std::string &		UID			() { return uid; }
	};

    DEFINE_CONST_PTR_TYPE( DefaultVideoInputResourceDescr );

} }