#pragma once


#include "IAudioInputAsset.h"
#include "AudioInputAssetDesc.h"

#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Engine/Graphics/InputSlots/InputSlots.h"

#include "Engine/Audio/Resources/AudioBuffer.h"

#include "Engine/Events/InnerEvents/InputSlots/SlotAddedEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/SlotRemovedEvent.h"

#include "InputSlotBinding.h"


namespace bv
{

class AudioInputAsset;
DEFINE_CONST_PTR_TYPE( AudioInputAsset )
DEFINE_PTR_TYPE( AudioInputAsset )



/**@brief Gets data from input slot.
@ingroup Assets
@ingroup InputSlots*/
class AudioInputAsset : public IAudioInputAsset, public std::enable_shared_from_this< AudioInputAsset >
{
    friend AudioInputAssetConstPtr InputSlots::CreateAsset( InputSlotsPtr, AudioInputAssetDescConstPtr );
private:

    static const std::string		uid;

protected:

    InputSlotsPtr           m_slots;
    InputSlotBinding        m_binding;

    audio::AudioBufferPtr   m_audio;

    mutable std::atomic< UInt32 >   m_numReferences;    ///< Remembers number of references to update it in new asset when texture under slot changes.

protected:

    explicit            AudioInputAsset   ( InputSlotsPtr slots, InputSlotBinding binding );

    virtual void        EvaluateSlot        ();

    virtual void        OnSlotAdded         ( SlotAddedEventPtr evt );
    virtual void        OnSlotRemoved       ( SlotRemovedEventPtr evt );

    void                HandleEvents        ( IEventPtr evt );
    bool                ShouldReevaluate    ( SlotEventBase * evt );

public:

    virtual             ~AudioInputAsset  ();

    static AudioInputAssetPtr       Create          ( InputSlotsPtr slots, InputSlotBinding binding );

    virtual VoidConstPtr            QueryThis       () const override;
    virtual const std::string &	    GetUID			() const override;

    static const std::string &	    UID				();


public:

    ///@name Inherited from IAudioInputAsset
    ///@{

    virtual void                    AddReference        () const override;
    virtual void                    ReleaseReference    () const override;

    virtual UInt32                  GetFrequency        () const override;
    virtual AudioFormat             GetFormat           () const override;
    virtual MemoryChunkConstPtr     GetFrame            () const override;

    ///@}


};


// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string &   GetAssetDescUID< AudioInputAsset >()
{
    return AudioInputAssetDesc::UID();
}


}	// bv
