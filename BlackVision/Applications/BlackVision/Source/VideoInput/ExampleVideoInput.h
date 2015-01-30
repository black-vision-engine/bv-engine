#include "IVideoInput.h"

namespace bv {

	class ExampleVideoInput :
		public IVideoInput
	{
		float m_fps;
		MemoryChunkPtr bits;
		model::DefaultTextureDescriptor* desc;
	
		void		GenerateBits( int x, int y );
	public:
		ExampleVideoInput( int x, int y, float fps );
		~ExampleVideoInput(void);

		virtual MemoryChunkConstPtr     GetBits             () const;
		virtual bool                    BitsChanged         () const;
		virtual void                    ResetBitsChanged    () const;

		virtual unsigned int            GetWidth			() const;
		virtual unsigned int            GetHeight			() const;
		virtual TextureFormat           GetFormat			() const;

		virtual model::DefaultTextureDescriptor* GetTexture		() const;
		virtual void					Update				() override;

		virtual model::DefaultTextureDescriptor* GetTextureDesc	() const; // THIS IS SO UGLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	};

}