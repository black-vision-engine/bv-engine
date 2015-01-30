#include "DefaultAnimationDescriptor.h"

#include "Engine/Models/Resources/ModelTextureManager.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Graphics/Resources/Texture.h"


namespace bv { namespace model {

// *******************************
//
DefaultAnimationDescriptor::DefaultAnimationDescriptor        ()
{
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
    SetCurrentFrame( 0 ); //current
    SetCurrentFrame( 0 ); //previous
}

// *******************************
//
DefaultAnimationDescriptor::~DefaultAnimationDescriptor       ()
{
}

// *******************************
//
unsigned int            DefaultAnimationDescriptor::NumTextures         () const
{
    return (unsigned int) m_frames.size();    
}

// *******************************
//
MemoryChunkConstPtr      DefaultAnimationDescriptor::GetBits             ( unsigned int idx ) const
{
    assert( idx < NumTextures() );

    return m_frames[ idx ]->GetData();
}

// *******************************
//
bool                    DefaultAnimationDescriptor::BitsChanged         () const
{
    //FIXME: implement
    assert( false );
    return false;
}

// *******************************
//
bool                    DefaultAnimationDescriptor::BitsChanged         ( unsigned int frameNum ) const
{
    //FIXME: implement
    assert( false && "Implement" );
    return m_frameBiteChanged[ frameNum ];
}

// *******************************
//
void                    DefaultAnimationDescriptor::ResetBitsChanged    ( unsigned int frameNum ) const
{
    SetBitsChanged( frameNum, false );
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
void                    DefaultAnimationDescriptor::SetBits             ( unsigned int idx, ResourceHandleConstPtr handle )
{
    auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    assert( texExtra->GetWidth() == GetWidth() );
    assert( texExtra->GetHeight() == GetHeight() );
    assert( texExtra->GetFormat() == GetFormat() );
    assert( idx < NumTextures() );

//    delete[] m_frames[ idx ];
    
  //  unsigned int dataSize = GetWidth() * GetHeight() * Texture::GetPixelSize( GetFormat() );

    m_frames[ idx ] = handle;
//    m_frames[ idx ] = new char[ dataSize ];

//    memcpy( m_frames[ idx ], data, dataSize );
}

// *******************************
//
void                     DefaultAnimationDescriptor::AddBits            ( ResourceHandleConstPtr handle )
{
    auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    assert( texExtra->GetWidth() == GetWidth() );
    assert( texExtra->GetHeight() == GetHeight() );
    assert( texExtra->GetFormat() == GetFormat() );

    m_frames.push_back( handle );
}

// *******************************
//
void                     DefaultAnimationDescriptor::SetBitsChanged      ( unsigned int frameNum, bool bitsChanged ) const
{
    //FIXME: implement
    assert( false && "Implement" );
    m_frameBiteChanged[ frameNum ] = bitsChanged;
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

    auto handle = LoadFrame( frames[ 0 ] );

    if ( handle == nullptr )
    {
        return nullptr;
    }

    printf( "Loading animation\n" );

    auto extraKind = handle->GetExtra()->GetResourceExtraKind();
    assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

    auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
    assert( texExtra->GetType() == TextureType::T_2D );

    auto fmt = texExtra->GetFormat();
    auto w  = (unsigned int) texExtra->GetWidth();
    auto h = (unsigned int) texExtra->GetHeight();

    DefaultAnimationDescriptor * retDesc = new DefaultAnimationDescriptor( name, w, h, fmt, TextureWrappingMode::TWM_CLAMP_BORDER, TextureWrappingMode::TWM_CLAMP_BORDER, TextureFilteringMode::TFM_LINEAR, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

    retDesc->AddBits( handle );

    for ( unsigned int i = 1; i < frames.size(); ++i )
    {
        auto handle = LoadFrame( frames[ i ] );

        auto extraKind = handle->GetExtra()->GetResourceExtraKind();
        assert( extraKind == model::ResourceExtraKind::RE_TEXTURE );

        auto texExtra = static_cast< const model::TextureExtraData * >( handle->GetExtra() );
        assert( texExtra->GetType() == TextureType::T_2D );

        unsigned int lw = (unsigned int) texExtra->GetWidth();
        unsigned int lh = (unsigned int) texExtra->GetHeight();
        TextureFormat lfmt = texExtra->GetFormat();

        if( lfmt != fmt || lw != w || lh != h )
        {
            delete retDesc;
            
            retDesc = nullptr;
            
            break;
        }
        else
        {
            retDesc->AddBits( handle );
        }

        printf( "\rLoaded %d out of %d total frames                ", i + 1, frames.size() );
    }

    printf( "\n" );

    return retDesc;
}

// *******************************
//
ResourceHandleConstPtr              DefaultAnimationDescriptor::LoadFrame       ( const std::string & frame )
{
//    Resource texture( "some name", frame );

    //TextureLoader texLoader;
    //ResourceHandle * handle = texLoader.LoadResource( &texture );

    auto handle = TextureManager::Get().GetTexture( frame );

    if ( handle == nullptr )
    {
        return nullptr;
    }

    return handle;
}

} //model
} //bv
