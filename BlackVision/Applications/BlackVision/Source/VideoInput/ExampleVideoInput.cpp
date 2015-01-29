#include "ExampleVideoInput.h"
#include "Engine\Models\Resources\TextureLoader.h"

namespace bv {

ExampleVideoInput::ExampleVideoInput( int x, int y, float fps )
{	
	GenerateBits( x, y );

	desc = new model::DefaultTextureDescriptor( model::ResourceHandleConstPtr( new model::ResourceHandle( bits, bits->Size(), new model::TextureExtraData( x, y, 32, TextureFormat::F_A8R8G8B8, TextureType::T_2D )  )),
		"Tex0", TextureWrappingMode::TWM_MIRROR, TextureWrappingMode::TWM_MIRROR, TextureFilteringMode::TFM_LINEAR, glm::vec4( 1, 1, 1, 1), DataBuffer::Semantic::S_TEXTURE_DYNAMIC );

	m_fps = fps;
}


ExampleVideoInput::~ExampleVideoInput(void)
{
}

void VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie             ( const ExampleVideoInput* input)
{ // FIXME: this is so wrong; we need some kind of Update() in interface
	auto nonConstInput = (ExampleVideoInput*)input;
	nonConstInput->Update(0);
}

void		ExampleVideoInput::GenerateBits( int x, int y )
{
	SizeType size = x * y * 4;
	char * mem = new char[ size ];

	for( int i = 0; i < size; i++ ) mem[i] = char( rand() );

	MemoryChunkPtr chunk = std::make_shared < MemoryChunk >( mem, size );
	bits = chunk;
}

MemoryChunkConstPtr     ExampleVideoInput::GetBits             () const
{
	VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie( this );
	return bits;
}

bool                    ExampleVideoInput::BitsChanged         () const
{
	return true; // ;)
}

void                    ExampleVideoInput::ResetBitsChanged    () const
{
	return; // :P
}

unsigned int            ExampleVideoInput::GetWidth			() const
{
	return desc->GetWidth();
}

unsigned int            ExampleVideoInput::GetHeight			() const
{
	return desc->GetHeight();
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

	virtual MemoryChunkConstPtr GetBits() const override
	{ 
		return input->GetBits(); 
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

model::DefaultTextureDescriptor* ExampleVideoInput::GetTexture		() const
{
	return new ExampleVideoInputWrapper( this );
}

void					ExampleVideoInput::Update				( TimeType t )
{
	GenerateBits( GetWidth(), GetHeight() ); //hahaha ;)
}

model::DefaultTextureDescriptor* ExampleVideoInput::GetTextureDesc	() const
{
	return desc;
}

}