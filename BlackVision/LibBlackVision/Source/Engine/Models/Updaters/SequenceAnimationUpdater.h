#pragma once

#include "Engine/Interfaces/IUpdater.h"


namespace bv {

class Texture2D;

namespace model
{
    class ISequenceAnimationSource;
}

class SequenceAnimationUpdater : public IUpdater
{
private:

    Texture2D *                     m_out;
    const model::ISequenceAnimationSource *     m_in;

public:

                    SequenceAnimationUpdater        ( Texture2D * out , const model::ISequenceAnimationSource * in ); 
                    ~SequenceAnimationUpdater       ();

    virtual void    DoUpdate                        ();

};

}
