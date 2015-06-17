#include "TestVideoInput.h"
//#include "Engine\Models\Resources\TextureLoader.h"

namespace bv {

TestVideoInput::TestVideoInput( int x, int y, float fps, int maskAnd, int maskOr )
{	
	m_fps = fps;
    m_maskAnd = maskAnd;
    m_maskOr = maskOr;

	GenerateBits( x, y );

    auto asset = new TextureAsset( SingleTextureAsset::Create( bits, "key", x, y, TextureFormat::F_A8R8G8B8, false ), nullptr );

    desc = new model::DefaultTextureDescriptor( TextureAssetConstPtr( asset ),
		//"Tex0", TextureWrappingMode::TWM_MIRROR, TextureWrappingMode::TWM_MIRROR, TextureFilteringMode::TFM_LINEAR, glm::vec4( 1, 1, 1, 1), DataBuffer::Semantic::S_TEXTURE_DYNAMIC );
        "Tex0", TextureWrappingMode::TWM_MIRROR, TextureWrappingMode::TWM_MIRROR, TextureFilteringMode::TFM_POINT, glm::vec4( 1, 1, 1, 1), DataBuffer::Semantic::S_TEXTURE_DYNAMIC );
}


TestVideoInput::~TestVideoInput(void)
{
}

void VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie2             ( const TestVideoInput* input)
{ // FIXME: this is so wrong; we need some kind of Update() in interface
	auto nonConstInput = (TestVideoInput*)input;
	nonConstInput->Update();
}


void		TestVideoInput::RefreshData( unsigned char *data)
{
	
    /*for(int i=1920*100;i<1920*800;i++)
    {
        data[i]=(unsigned char)16*8;
    }*/

	MemoryChunkPtr chunk = std::make_shared < MemoryChunk >( (char*)data, 1920*1080*4 );
	bits = chunk;
}

void		TestVideoInput::GenerateBits( int x, int y )
{
    {x;}
    {y;}
	SizeType size = 1920 * 1080 * 4;
	char * mem = new char[ size ];

    unsigned int intMaskAnd = m_maskAnd;
    unsigned int intMaskOr = m_maskOr;

	for( SizeType i = 0; i < size; i++ )
    {
        unsigned int rgbChan = i%4;
        unsigned char maskAnd = ( intMaskAnd >> (rgbChan*8) ) & 0xff;
        unsigned char maskOr = ( intMaskOr >> (rgbChan*8) ) & 0xff;
        mem[i] = char( rand() & maskAnd | maskOr );
    }

	MemoryChunkPtr chunk = std::make_shared < MemoryChunk >( mem, size );
	bits = chunk;
}

MemoryChunkConstPtr     TestVideoInput::GetBits             () const
{
	VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie2( this );
	return bits;
}

bool                    TestVideoInput::BitsChanged         () const
{
	return true; // ;)
}

void                    TestVideoInput::ResetBitsChanged    () const
{
	return; // :P
}

unsigned int            TestVideoInput::GetWidth			() const
{
	return (unsigned int)desc->GetWidth();
}

unsigned int            TestVideoInput::GetHeight			() const
{
	return (unsigned int)desc->GetHeight();
}

TextureFormat           TestVideoInput::GetFormat			() const
{
	return desc->GetFormat();
}

// FIXME: everything below is to remove!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class TestVideoInputWrapper : public model::DefaultTextureDescriptor
{
	const TestVideoInput* input;
public:
	TestVideoInputWrapper( const TestVideoInput* i ) : DefaultTextureDescriptor( *i->GetTextureDesc() ), input( i ) {}

	virtual MemoryChunkVector GetBits() const override
	{ 
        auto v = new MemoryChunkVector();
        v->push_back( input->GetBits() );
        return *v;
	}

	virtual bool                    BitsChanged         () const override
	{
		return input->BitsChanged();
	}

	virtual void                    ResetBitsChanged    () const override
	{
		return input->ResetBitsChanged();
	}
};

model::DefaultTextureDescriptor* TestVideoInput::GetTexture		() const
{
	return new TestVideoInputWrapper( this );
}

void					TestVideoInput::Update				()
{
	//GenerateBits( GetWidth(), GetHeight() ); //hahaha ;)
    int i = 0;
    i=1;
}

model::DefaultTextureDescriptor* TestVideoInput::GetTextureDesc	() const
{
	return desc;
}


}