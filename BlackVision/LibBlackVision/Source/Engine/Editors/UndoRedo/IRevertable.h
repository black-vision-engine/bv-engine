#pragma once

#include "CoreDEF.h"


namespace bv
{


class BVProjectEditor;

class IRevertable
{
private:

public:

    virtual ~IRevertable() {}


    virtual bool        Undo    ( BVProjectEditor * editor ) = 0;
    virtual bool        Redo    ( BVProjectEditor * editor ) = 0;
};

DEFINE_UPTR_TYPE( IRevertable )

}	// bv
