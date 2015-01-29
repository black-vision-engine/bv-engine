namespace bv { namespace model {

	class DefaultVideoInputResourceDescr : public IVideoInputResourceDescr
	{
		DefaultTextureDescriptor* descr;

	public:
		DefaultVideoInputResourceDescr( DefaultTextureDescriptor* d ) : descr( d ) {}

		virtual PluginResourceType      GetResourceType () const { return PluginResourceType::PRT_VIDEOINPUT; }

		virtual  DefaultTextureDescriptor*     GetITextureDescriptor  () const { return descr; }
	};

} }