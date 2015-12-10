#include "InterlaceFullscreenEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// **************************
//
InterlaceFullscreenEffect::InterlaceFullscreenEffect  ( bool startEven, int height )
    : m_startEven( startEven )
    , m_height( height )
{
    m_startEvenValue = ValuesFactory::CreateValueInt( "startEven" );
    m_heightValue = ValuesFactory::CreateValueInt( "height" );

    SetStartEven( startEven );
    SetHeight( height );
}

// **************************
//
InterlaceFullscreenEffect::~InterlaceFullscreenEffect ()
{
}

// **************************
//
void                InterlaceFullscreenEffect::SetInterlaceTextures         ( Texture2DPtr tex0, Texture2DPtr tex1 )
{
    {tex0; tex1;}
}

// **************************
//
void                InterlaceFullscreenEffect::SetStartEven                 ( bool startEven )
{
    m_startEven = startEven;

    m_startEvenValue->SetValue( startEven );
}

// **************************
//
bool                InterlaceFullscreenEffect::GetStartEven                 () const
{
    return m_startEven;
}

// **************************
//
void                InterlaceFullscreenEffect::SetHeight                    ( int height )
{
    assert( height > 0 );

    m_height = height;

    m_heightValue->SetValue( height );
}

// **************************
//
int                 InterlaceFullscreenEffect::GetHeight                    () const
{
    return m_height;
}

// **************************
//
RenderableEntity *  InterlaceFullscreenEffect::CreateFullscreenQuad         () const
{
    return nullptr;
}

// **************************
//
PixelShader *       InterlaceFullscreenEffect::CreatePS                     () const
{
    return nullptr;    
}

} //bv
