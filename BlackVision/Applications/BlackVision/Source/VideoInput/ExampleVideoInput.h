#include "Engine/Models/Plugins/Simple/VideoInput/IVideoInput.h"

namespace bv {

    class ExampleVideoInput :
        public IVideoInput
    {
        float m_fps;
        MemoryChunkPtr bits;
        model::DefaultTextureDescriptorPtr desc;

        int m_maskAnd;
        int m_maskOr;

        UInt64 m_updateID;
    
        void		GenerateBits( int x, int y );
    public:
        ExampleVideoInput( int x, int y, float fps, int maskAnd = 0xffffffff, int maskOr = 0xff000000 );
        ~ExampleVideoInput(void);

        virtual MemoryChunkConstPtr     GetBits             () const;
        virtual UInt64                  GetUpdateID         () const;

        virtual unsigned int            GetWidth			() const;
        virtual unsigned int            GetHeight			() const;
        virtual TextureFormat           GetFormat			() const;

        virtual model::DefaultTextureDescriptorPtr GetTexture		() const;
        virtual void					Update				() override;

        virtual model::DefaultTextureDescriptorPtr GetTextureDesc	() const; // THIS IS SO UGLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    };

}