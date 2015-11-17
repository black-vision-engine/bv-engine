#include "IVideoInput.h"

namespace bv {

    class ExampleVideoInput :
        public IVideoInput
    {
        float m_fps;
        MemoryChunkPtr bits;
        model::DefaultTextureDescriptor* desc;

        int m_maskAnd;
        int m_maskOr;

        UInt32 m_updateID;
    
        void		GenerateBits( int x, int y );
    public:
        ExampleVideoInput( int x, int y, float fps, int maskAnd = 0xffffffff, int maskOr = 0xff000000 );
        ~ExampleVideoInput(void);

        virtual MemoryChunkConstPtr     GetBits             () const;
        virtual UInt32                  GetUpdateID         () const;

        virtual unsigned int            GetWidth			() const;
        virtual unsigned int            GetHeight			() const;
        virtual TextureFormat           GetFormat			() const;

        virtual model::DefaultTextureDescriptor* GetTexture		() const;
        virtual void					Update				() override;

        virtual model::DefaultTextureDescriptor* GetTextureDesc	() const; // THIS IS SO UGLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    };

}