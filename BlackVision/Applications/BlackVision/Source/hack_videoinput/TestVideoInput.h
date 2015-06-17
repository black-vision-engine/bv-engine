#pragma once
#include "../VideoInput/IVideoInput.h"

namespace bv {

    class TestVideoInput :
        public IVideoInput
    {
        float m_fps;
        MemoryChunkPtr bits;
        model::DefaultTextureDescriptor* desc;

        int m_maskAnd;
        int m_maskOr;
    
        void		GenerateBits( int x, int y );
    public:

        void		RefreshData(unsigned char * data);
        TestVideoInput( int x, int y, float fps, int maskAnd = 0xffffffff, int maskOr = 0xff000000 );
        ~TestVideoInput(void);

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