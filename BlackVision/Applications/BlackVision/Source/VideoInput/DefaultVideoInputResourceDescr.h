#include "IVideoInput.h"

namespace bv { namespace model {

	class DefaultVideoInputResourceDescr //: public IVideoInputResourceDescr
	{
		DefaultTextureDescriptor* descr;
		IVideoInput *input;

	public:
		DefaultVideoInputResourceDescr( DefaultTextureDescriptor* d, IVideoInput *i ) : descr( d ), input( i ) {}

		virtual PluginResourceType      GetResourceType () const { return PluginResourceType::PRT_VIDEOINPUT; }

		//virtual  DefaultTextureDescriptor*     GetITextureDescriptor  () const override { return descr; }

		//virtual void Update() const override { input->Update(); }
	};

} }