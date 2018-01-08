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

private:

    SlotIndex               FindSourceByName    ( const std::string & name ) const;
    SlotIndex               FindEmptySlot       () const;
    Expected< SlotIndex >   AddSource           ( const InputEntry & entry );

    bool                IsValidIndex        ( SlotIndex idx ) const;
    bool                IsEmptySlot         ( SlotIndex idx ) const;
    bool                CanAddSource        ( InputSlot inputSlot, const std::string & name ) const;

public:

    ///@name TextureInputAsset creation
    ///@{
    TextureInputAssetConstPtr       CreateAsset     ( InputSlotsPtr thisPtr, TextureInputAssetDescConstPtr desc );

    ///@}

};




}	// bv


