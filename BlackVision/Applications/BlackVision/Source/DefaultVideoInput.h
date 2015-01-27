#pragma once
#include "Engine\Models\Plugins\Channels\PixelShader\DefaultAnimationDescriptor.h"
#include "Engine\Models\Plugins\Channels\PixelShader\DefaultTextureDescriptor.h"
#include "Engine\Models\Resources\IPluginResourceDescr.h"

namespace bv { namespace model {

	class DefaultVideoInputResourceDescr : public IVideoInputResourceDescr
	{
		DefaultTextureDescriptor* descr;

	public:
		DefaultVideoInputResourceDescr( DefaultTextureDescriptor* d ) : descr( d ) {}

		virtual PluginResourceType      GetResourceType () const { return PluginResourceType::PRT_VIDEOINPUT; }

		virtual  DefaultTextureDescriptor*     GetITextureDescriptor  () const { return descr; }
	};

	class IVideoInput
	{
	public:
		virtual MemoryChunkConstPtr     GetBits             () const = 0;
		virtual bool                    BitsChanged         () const = 0;
		virtual void                    ResetBitsChanged    () const = 0;

		virtual unsigned int            GetWidth			() const = 0;
		virtual unsigned int            GetHeight			() const = 0;
		virtual TextureFormat           GetFormat			() const = 0;

		virtual DefaultTextureDescriptor* GetTexture		() const = 0; // FIXME: temporary
		virtual void					Update				( TimeType t ) = 0; // FIXME: Viggith says it's so wrong
	};

	class DefaultVideoInput :
		IVideoInput
	{
		float m_fps;
		MemoryChunkPtr bits;
		DefaultTextureDescriptor* desc;
	
		void		DefaultVideoInput::GenerateBits( int x, int y );
	public:
		DefaultVideoInput( int x, int y, float fps );
		~DefaultVideoInput(void);

		virtual MemoryChunkConstPtr     GetBits             () const;
		virtual bool                    BitsChanged         () const;
		virtual void                    ResetBitsChanged    () const;

		virtual unsigned int            GetWidth			() const;
		virtual unsigned int            GetHeight			() const;
		virtual TextureFormat           GetFormat			() const;

		virtual DefaultTextureDescriptor* GetTexture		() const;
		virtual void					Update				( TimeType t );

		virtual DefaultTextureDescriptor* GetTextureDesc	() const; // THIS IS SO UGLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	};

} }