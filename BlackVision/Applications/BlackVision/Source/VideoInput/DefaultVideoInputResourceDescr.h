#include "IVideoInput.h"

namespace bv { namespace model {

	class DefaultVideoInputResourceDescr : public AssetDesc, public std::enable_shared_from_this< DefaultVideoInputResourceDescr >
	{
		DefaultTextureDescriptor* descr;
		IVideoInput *input;

	public:
		DefaultVideoInputResourceDescr( DefaultTextureDescriptor* d, IVideoInput *i ) : descr( d ), input( i ) {}

		virtual PluginResourceType      GetResourceType () const { return PluginResourceType::PRT_VIDEOINPUT; }

		virtual  DefaultTextureDescriptor*     GetITextureDescriptor  () const /*override*/ { return descr; }

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