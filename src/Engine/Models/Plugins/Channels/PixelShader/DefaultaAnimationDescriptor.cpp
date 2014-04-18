#include "DefaultAnimationDescriptor.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Graphics/Resources/Texture.h"


namespace bv { namespace model {

// *******************************
//
DefaultAnimationDescriptor::DefaultAnimationDescriptor        ()
{
    //FIXME: implement BitsChanged
}

// *******************************
//
DefaultAnimationDescriptor::DefaultAnimationDescriptor        ( const std::string & name, unsigned int w, unsigned int h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc )
{
    SetName( name );
    SetWidth( w );
    SetHeight( h );
    SetFormat( fmt );
    SetWrappingModeX( wmx );
    SetWrappingModeY( wmy );
    SetFilteringMode( fm );
    SetBorderColor( bc );
}

// *******************************
//
DefaultAnimationDescriptor::~DefaultAnimationDescriptor       ()
{
    for( auto data : m_frames )
    {
        delete[] data;
    }
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::NumTextures         () const
{
    return m_frames.size();    
}

// *******************************
//
const char *            DefaultAnimationDescriptor::GetBits             ( unsigned int idx ) const
{
    assert( idx < NumTextures() );

    return m_frames[ idx ];
}

// *******************************
//
bool                    DefaultAnimationDescriptor::BitsChanged         ( unsigned int * frameNum ) const
{
    //FIXME: implement
    assert( false && "Implement" );
    return false;
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::CurrentFrame        () const
{
    return m_curFrame;
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::PreviousFrame       () const
{
    return m_lastFrame;
}

// *******************************
//
const std::string       DefaultAnimationDescriptor::GetName             () const
{
    return m_params.GetName();
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::GetWidth            () const
{
    return m_params.GetWidth();
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::GetHeight           () const
{
    return m_params.GetHeight();
}

// *******************************
//
TextureFormat           DefaultAnimationDescriptor::GetFormat           () const
{
    return m_params.GetFormat();
}

// *******************************
//
TextureWrappingMode     DefaultAnimationDescriptor::GetWrappingModeX    () const
{
    return m_params.GetWrappingModeX();
}

// *******************************
//
TextureWrappingMode     DefaultAnimationDescriptor::GetWrappingModeY    () const
{
    return m_params.GetWrappingModeY();
}

// *******************************
//
TextureFilteringMode    DefaultAnimationDescriptor::GetFilteringMode    () const
{
    return m_params.GetFilteringMode();
}

// *******************************
//
glm::vec4               DefaultAnimationDescriptor::BorderColor         () const
{
    return m_params.BorderColor();
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetBits             ( unsigned int idx, const char * data, TextureFormat fmt, unsigned int w, unsigned int h )
{
    assert( w == GetWidth() );
    assert( h == GetHeight() );
    assert( fmt == GetFormat() );
    assert( idx < NumTextures() );

    delete[] m_frames[ idx ];
    
    unsigned int dataSize = w * h * Texture::GetPixelSize( fmt );

    m_frames[ idx ] = new char[ dataSize ];

    memcpy( m_frames[ idx ], data, dataSize );
}

// *******************************
//
void                     DefaultAnimationDescriptor::AddBits            ( const char * data, TextureFormat fmt, unsigned int w, unsigned int h )
{
    assert( w == GetWidth() );
    assert( h == GetHeight() );
    assert( fmt == GetFormat() );

    unsigned int dataSize = w * h * Texture::GetPixelSize( fmt );

    char * newData = new char[ dataSize ];

    memcpy( newData, data, dataSize );

    m_frames.push_back( newData );
}

// *******************************
//
void                     DefaultAnimationDescriptor::SetBitsChanged      ( unsigned int frameNum, bool bitsChanged )
{
    //FIXME: implement
    assert( false && "Implement" );
}

// *******************************
//
void                     DefaultAnimationDescriptor::SetCurrentFrame     ( unsigned int frameNum )
{
    m_lastFrame = m_curFrame;
    m_curFrame = frameNum;
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetName             ( const std::string & name )
{
    m_params.SetName( name );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetWidth            ( unsigned int w )
{
    m_params.SetWidth( w );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetHeight           ( unsigned int h )
{
    m_params.SetHeight( h );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetFormat           ( TextureFormat fmt )
{
    m_params.SetFormat( fmt );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetWrappingModeX    ( TextureWrappingMode wm )
{
    m_params.SetWrappingModeX( wm );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetWrappingModeY    ( TextureWrappingMode wm )
{
    m_params.SetWrappingModeY( wm );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetFilteringMode    ( TextureFilteringMode fm )
{
    m_params.SetFilteringMode( fm );
}

// *******************************
//
void                    DefaultAnimationDescriptor::SetBorderColor      ( const glm::vec4 & bc )
{
    m_params.SetBorderColor( bc );
}

// *******************************
//
DefaultAnimationDescriptor * DefaultAnimationDescriptor::LoadAnimation  ( const std::vector< std::string > & frames, const std::string & name )
{
    if ( frames.size() == 0 )
    {
        return nullptr;
    }

    unsigned int w = 0;
    unsigned int h = 0;
    TextureFormat fmt = TextureFormat::F_A8R8G8B8;
    const char * data = nullptr;

    auto numBytes = LoadFrame( frames[ 0 ], &fmt, &w, &h, &data );

    if ( numBytes == 0 )
    {
        return nullptr;
    }

    printf( "Loading animation\n" );

    DefaultAnimationDescriptor * retDesc = new DefaultAnimationDescriptor( name, w, h, fmt, TextureWrappingMode::TWM_REPEAT, TextureWrappingMode::TWM_REPEAT, TextureFilteringMode::TFM_LINEAR, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

    retDesc->AddBits( data, fmt, w, h );
    delete[] data;
    data = nullptr;

    for ( unsigned int i = 1; i < frames.size(); ++i )
    {
        unsigned int lw = 0;
        unsigned int lh = 0;
        TextureFormat lfmt = TextureFormat::F_A8R8G8B8;

        auto numBytes = LoadFrame( frames[ 0 ], &lfmt, &lw, &lh, &data );

        if( lfmt != fmt || lw != w || lh != h )
        {
            delete[] data;
            delete retDesc;
            
            retDesc = nullptr;
            
            break;
        }
        else
        {
            retDesc->AddBits( data, lfmt, lw, lh );
            delete[] data;
            data = nullptr;
        }

        printf( "\rLoaded %d out of %d total frames                ", i + 1, frames.size() );
    }

    printf( "\n" );

    return retDesc;
}

// *******************************
//
unsigned int                 DefaultAnimationDescriptor::LoadFrame       ( const std::string & frame, TextureFormat * fmt, unsigned int * w, unsigned int * h, const char ** ppData )
{
    Resource texture( "some name", frame );

    TextureLoader texLoader;
    ResourceHandle * handle = texLoader.LoadResource( &texture );

    if ( handle == nullptr )
    {
        return 0;
    }

    auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    *fmt = texExtra->GetFormat();
    *w   = texExtra->GetWidth();
    *h   = texExtra->GetHeight();

    auto size = handle->GetSize();
    *ppData = handle->GetData();

    handle->SetData( nullptr );

    delete handle;

    return size;
}

} //model
} //bv
