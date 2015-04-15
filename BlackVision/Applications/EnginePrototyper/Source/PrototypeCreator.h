#pragma once

#include "PrototypesForwardInc.h"


namespace  bv {

class Renderer;

IAppLogicPrototype * CreateDefaultPrototype             ( Renderer * renderer);

unsigned int        DefaultPrototypeWidth               ();
unsigned int        DefaultPrototypeHeight              ();
bool                DefaultPrototypeFullscreenSetting   ();

} // bv
