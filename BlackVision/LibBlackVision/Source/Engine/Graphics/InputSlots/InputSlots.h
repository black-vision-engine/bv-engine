#pragma once


#include "InputSlot.h"

#include <vector>
#include <mutex>


namespace bv
{

typedef SizeType SlotIndex;

class TextureInputAsset;
class TextureInputAssetDesc;
DEFINE_CONST_PTR_TYPE( TextureInputAssetDesc )
DEFINE_CONST_PTR_TYPE( TextureInputAsset )

class AudioInputAsset;
class AudioInputAssetDesc;
DEFINE_CONST_PTR_TYPE( AudioInputAssetDesc )
DEFINE_CONST_PTR_TYPE( AudioInputAsset )

class InputSlots;
DEFINE_PTR_TYPE( InputSlots )



/**@defgroup EngineInputs Inputs
@ingroup Engine

@brief Engine video or audio input mechanism.

Input slots provide mechanism to access global signal sources (for example textures or audio).
Unlike normal textures form files, such sources can be generated internally by engine or
supllied by other mechanism like video card inputs.
*/



// ***********************
/// @ingroup EngineInputs
class InputSlots
{
    friend class InputAssetLoader;
private:

    static const SlotIndex       sInvalidIdx;

private:

    std::vector< InputEntry >       m_slots;
    mutable std::recursive_mutex    m_lock;

    InputSlot                       m_fallbackSlot;

public:

    explicit    InputSlots    ();


    Expected< SlotIndex >   RegisterSource      ( InputSlot inputSlot, const std::string & name );
    bool                    UnregisterSource    ( SlotIndex slotIdx );
    bool                    UnregisterSource    ( const std::string & name );

    Expected< InputSlot >   AccessSource        ( const std::string & name );

    void                    ReferenceSource     ( const std::string & name );
    void                    ReleaseSource       ( const std::string & name );

    Expected< InputSlot >   AccessSource        ( SlotIndex slotIdx );

    void                    ReferenceSource     ( SlotIndex slotIdx );
    void                    ReleaseSource       ( SlotIndex slotIdx );

    InputSlot               GetFallbackSlot     () { return m_fallbackSlot; }

private:

    SlotIndex               FindSourceByName    ( const std::string & name ) const;
    SlotIndex               FindEmptySlot       () const;
    Expected< SlotIndex >   AddSource           ( const InputEntry & entry );

    bool                IsValidIndex        ( SlotIndex idx ) const;
    bool                IsEmptySlot         ( SlotIndex idx ) const;
    bool                CanAddSource        ( InputSlot inputSlot, const std::string & name ) const;

private:

    Texture2DPtr            CreateFallbackTexture   ();
    audio::AudioBufferPtr   CreateFallbackAudio     ();

    MemoryChunkPtr          GenerateFallbackTexture ( UInt32 width, UInt32 height, UInt32 bpp );

public:

    ///@name InputAsset creation
    ///@{
    TextureInputAssetConstPtr       CreateAsset     ( InputSlotsPtr thisPtr, TextureInputAssetDescConstPtr desc );
    AudioInputAssetConstPtr         CreateAsset     ( InputSlotsPtr thisPtr, AudioInputAssetDescConstPtr desc );

    ///@}

};




}	// bv


