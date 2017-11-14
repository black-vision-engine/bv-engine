#pragma once

#include "Engine/Graphics/InputSlots/Logic/IInputHandler.h"

#include "VideoCardManager.h"


namespace bv
{

// ***********************
/// @ingroup EngineInputs
class VideoInputHandler : public IInputHandler
{
private:

    videocards::VideoCardManager *      m_videoCardManager;

public:

    explicit                VideoInputHandler       ( videocards::VideoCardManager * videoCardMan );

    virtual void            UnregisterInputs        () override;
    virtual void            ProcessInputs           () override;

    virtual void            RegisterInputs          ( InputSlotsPtr inputSlots );

};

DEFINE_PTR_TYPE( VideoInputHandler )


}	// bv

