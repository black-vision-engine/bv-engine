#pragma once


#include "Assets/Asset.h"
#include "CoreEnums.h"



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

    virtual UInt32      GetFrequency        () const = 0;
    virtual AudioFormat GetFormat           () const = 0;

    virtual UInt64      LastAudioUpdate     () const = 0;

    virtual MemoryChunkConstPtr     GetFrame() const = 0;
};

DEFINE_PTR_TYPE( IAudioInputAsset );
DEFINE_CONST_PTR_TYPE( IAudioInputAsset );


}	// bv




