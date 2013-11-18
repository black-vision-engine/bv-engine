#pragma once

#include "Engine/Interfaces/IUpdater.h"


namespace bv {

class TextureAnimatedSequence2D;

namespace model
{
    class ISequenceAnimationSource;
}

class SequenceAnimationUpdater : public IUpdater
{
private:

    TextureAnimatedSequence2D *              m_out;
    const model::ISequenceAnimationSource *  m_in;

public:

                    SequenceAnimationUpdater        ( TextureAnimatedSequence2D * out , const model::ISequenceAnimationSource * in ); 
                    ~SequenceAnimationUpdater       ();

    virtual void    Update                          ( TimeType t );

};

}
