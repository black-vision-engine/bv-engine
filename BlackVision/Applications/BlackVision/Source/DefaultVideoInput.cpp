#include "DefaultVideoInput.h"
#include "Engine\Models\Resources\TextureLoader.h"

namespace bv { namespace model {

DefaultVideoInput::DefaultVideoInput( int x, int y, float fps )
{	
	GenerateBits( x, y );

	desc = new DefaultTextureDescriptor( ResourceHandleConstPtr( new ResourceHandle( bits, bits->Size(), new TextureExtraData( x, y, 32, TextureFormat::F_A8R8G8B8, TextureType::T_2D )  )),
		"Tex0", TextureWrappingMode::TWM_MIRROR, TextureWrappingMode::TWM_MIRROR, TextureFilteringMode::TFM_POINT, glm::vec4( 1, 1, 1, 1), DataBuffer::Semantic::S_TEXTURE_DYNAMIC );

	m_fps = fps;
}


DefaultVideoInput::~DefaultVideoInput(void)
{
}

void VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie             ( const DefaultVideoInput* input)
{ // FIXME: this is so wrong; we need some kind of Update() in interface
	auto nonConstInput = (DefaultVideoInput*)input;
	nonConstInput->Update(0);
}

void		DefaultVideoInput::GenerateBits( int x, int y )
{
	SizeType size = x * y * 4;
	char * mem = new char[ size ];

	for( int i = 0; i < size; i++ ) mem[i] = rand();

	MemoryChunkPtr chunk = std::make_shared < MemoryChunk >( mem, size );
	bits = chunk;
}

MemoryChunkConstPtr     DefaultVideoInput::GetBits             () const
{
	VeryHackishMethodToUpdateConstVideoInputBeacuseLifeSucksSoMuchAndIWantToDie( this );
	return bits;
}

bool                    DefaultVideoInput::BitsChanged         () const
{
	return true; // ;)
}

void                    DefaultVideoInput::ResetBitsChanged    () const
{
	return; // :P
}

unsigned int            DefaultVideoInput::GetWidth			() const
{
	return desc->GetWidth();
}

unsigned int            DefaultVideoInput::GetHeight			() const
{
	return desc->GetHeight();
}

TextureFormat           DefaultVideoInput::GetFormat			() const
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

class DefaultVideoInputWrapper : public DefaultTextureDescriptor
{
	const DefaultVideoInput* input;
public:
	DefaultVideoInputWrapper( const DefaultVideoInput* i ) : DefaultTextureDescriptor( *i->GetTextureDesc() ), input( i ) {}

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

DefaultTextureDescriptor* DefaultVideoInput::GetTexture		() const
{
	return new DefaultVideoInputWrapper( this );
}

void					DefaultVideoInput::Update				( TimeType t )
{
	GenerateBits( GetWidth(), GetHeight() ); //hahaha ;)
}

DefaultTextureDescriptor* DefaultVideoInput::GetTextureDesc	() const
{
	return desc;
}


// *******************************
//
IVideoInputResourceDescrConstPtr   QueryVideoInputResourceDescr   ( IPluginResourceDescrConstPtr resDescr )
{
	if ( resDescr->GetResourceType() == PluginResourceType::PRT_VIDEOINPUT )
	{
		return std::static_pointer_cast< const IVideoInputResourceDescr >( resDescr );
	}

	return nullptr;
}


} }