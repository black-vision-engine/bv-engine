#pragma once
#include "Engine\Models\Plugins\Channels\PixelShader\DefaultTextureDescriptor.h"

namespace bv {

	class IVideoInputCore
	{
	public:
		virtual MemoryChunkConstPtr     GetBits             () const = 0;
		virtual UInt64                  GetUpdateID			() const = 0;

		virtual unsigned int            GetWidth			() const = 0;
		virtual unsigned int            GetHeight			() const = 0;
		virtual TextureFormat           GetFormat			() const = 0;

		virtual void					Update				() = 0; // FIXME(?): Viggith says it's so wrong
	};

	class IVideoInput : public IVideoInputCore
	{
	public:
		virtual model::DefaultTextureDescriptorPtr GetTexture		() const = 0; // FIXME(???): temporary(?) or not really?
																			// removing it would require changes in the engine or using animation instead of texture interface
																		// one could also switch from DefaultTextureDescriptor to ITextureDescriptor
	};

}