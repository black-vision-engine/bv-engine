#pragma once

#include "Engine/Interfaces/IUpdater.h"


namespace bv {

class Texture2DSequenceImpl;

namespace model
{
    class ISequenceAnimationSource;
}

class SequenceAnimationUpdater : public IUpdater
{
private:

    Texture2DSequenceImpl *                     m_out;
    const model::ISequenceAnimationSource *     m_in;

public:

                    SequenceAnimationUpdater        ( Texture2DSequenceImpl * out , const model::ISequenceAnimationSource * in ); 
                    ~SequenceAnimationUpdater       ();

    virtual void    DoUpdate                        ();

};

}
