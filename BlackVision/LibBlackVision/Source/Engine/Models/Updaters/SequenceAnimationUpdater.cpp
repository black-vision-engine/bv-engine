#include "SequenceAnimationUpdater.h"

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"


namespace bv {

// **********************************
//
SequenceAnimationUpdater::SequenceAnimationUpdater        ( Texture2DSequenceImpl * out , const model::ISequenceAnimationSource * in )
    : m_out( out )
    , m_in( in )
{
    assert( out );
    assert( in );
}

// **********************************
//
SequenceAnimationUpdater::~SequenceAnimationUpdater       ()
{
    //FIXME: anything here? 
    //FIXME: Yes.
    //FIXME: Really?
    //FIXME: Fuk Off :P
}

// **********************************
//
void    SequenceAnimationUpdater::DoUpdate                ()
{
    SizeType txFrame    = m_out->GetActiveTextureNum();
    SizeType srcFrame   = m_in->CurrentFrame();

    if( txFrame != srcFrame )
    {
        m_out->SetActiveTexture( srcFrame );
    }
}

} //bv

