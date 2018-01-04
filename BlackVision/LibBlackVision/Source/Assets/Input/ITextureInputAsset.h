#pragma once


#include "Assets/Asset.h"




namespace bv
{



/**@brief Interface for assets using input slots.

This is interface to be used in model. Model shouldn't see any engine objects.

Objects using input slots should always add and release references when they use input.
Note that this isn't shared pointer like behavior. Underlying slot can always be released
even if someone uses it. Reference counting can only help in optimizing input usage
(for example video input can stop loading coming frames from video card).

@ingroup Assets
@ingroup InputSlots*/
class ITextureInputAsset : public Asset
{
private:
public:

    virtual void        AddReference        () const = 0;
    virtual void        ReleaseReference    () const = 0;

};


}	// bv


