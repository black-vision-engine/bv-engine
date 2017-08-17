#include "stdafxBVApp.h"

#include "ExampleVideoInput.h"
//#include "Engine\Models\Resources\TextureLoader.h"

namespace bv {

ExampleVideoInput::ExampleVideoInput( int x, int y, float fps, int maskAnd, int maskOr )
{	
	m_fps = fps;
    m_maskAnd = maskAnd;
    m_maskOr = maskOr;

	GenerateBits( x, y );

	auto asset = TextureAsset::Create( SingleTextureAsset::Create( bits, "key", x, y, TextureFormat::F_A8R8G8B8, false ), nullptr );

	desc = std::make_shared< model::DefaultTextureDescriptor >( asset, "Tex0", DataBuffer::Semantic::S_TEXTURE_DYNAMIC );
	//desc->SetSamplerState( model::SamplerStateModelPtr::Create( TextureWrappingMode::TWM_MIRROR, TextureWrappingMode::TWM_MIRROR, TextureWrappingMode::TWM_MIRROR, TextureFilteringMode::TFM_LINEAR, glm::vec4( 1, 1, 1, 1 ) ) ); 
}


ExampleVideoInput::~ExampleVideoInput(void)
{
}

void VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie             ( const ExampleVideoInput* input)
{ // FIXME: this is so wrong; we need some kind of Update() in interface
	auto nonConstInput = (ExampleVideoInput*)input;
	nonConstInput->Update();
}

void		ExampleVideoInput::GenerateBits( int x, int y )
{
	SizeType size = x * y * 4;
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

	m_updateID++;
}

MemoryChunkConstPtr     ExampleVideoInput::GetBits             () const
{
	VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie( this );
	return bits;
}

UInt64                  ExampleVideoInput::GetUpdateID         () const
{
	return m_updateID;
}

unsigned int            ExampleVideoInput::GetWidth			() const
{
	return (unsigned int)desc->GetWidth();
}

unsigned int            ExampleVideoInput::GetHeight			() const
{
	return (unsigned int)desc->GetHeight();
}

TextureFormat           ExampleVideoInput::GetFormat			() const
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

class ExampleVideoInputWrapper : public model::DefaultTextureDescriptor
{
	const ExampleVideoInput* input;
public:
	ExampleVideoInputWrapper( const ExampleVideoInput* i ) : DefaultTextureDescriptor( *i->GetTextureDesc() ), input( i ) {}

	virtual MemoryChunkVector GetBits() const override
	{ 
        auto v = new MemoryChunkVector();
        v->push_back( input->GetBits() );
        return *v;
	}

	virtual UInt64                  GetUpdateID         () const override
	{
		return input->GetUpdateID();
	}

};

model::DefaultTextureDescriptorPtr ExampleVideoInput::GetTexture		() const
{
	return std::make_shared< ExampleVideoInputWrapper >( this );
}

void					ExampleVideoInput::Update				()
{
	GenerateBits( GetWidth(), GetHeight() ); //hahaha ;)
}

model::DefaultTextureDescriptorPtr ExampleVideoInput::GetTextureDesc	() const
{
	return desc;
}


}