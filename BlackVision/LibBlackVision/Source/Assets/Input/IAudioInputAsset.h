#pragma once


#include "Assets/Asset.h"




namespace bv
{





/**@brief Interface for assets using audio input slots.

@ingroup Assets
@ingroup InputSlots*/
class IAudioInputAsset : public Asset
{
private:
public:

    virtual void        AddReference        () const = 0;
    virtual void        ReleaseReference    () const = 0;

    virtual UInt64      GetUpdateID         () const = 0;

};

DEFINE_PTR_TYPE( IAudioInputAsset );
DEFINE_CONST_PTR_TYPE( IAudioInputAsset );


}	// bv




