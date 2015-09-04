#include "SequenceAnimationUpdater.h"

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/ISequenceAnimationSource.h"
#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

// **********************************
//
SequenceAnimationUpdater::SequenceAnimationUpdater        ( Texture2D * out , const model::ISequenceAnimationSource * in )
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
	//@SEQUENCE  //not used?
   /* auto txFrame    = m_out->GetActiveTextureNum();
    auto srcFrame   = m_in->CurrentFrame();

    if( txFrame != srcFrame )
    {
        m_out->SetActiveTexture( srcFrame );
    }*/
}

} //bv

