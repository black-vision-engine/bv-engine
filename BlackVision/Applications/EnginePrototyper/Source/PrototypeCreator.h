#pragma once

#include "PrototypesForwardInc.h"
#include "Application/WindowedApplication.h" // enum WindowMode

namespace  bv {

class Renderer;

IAppLogicPrototype * CreateDefaultPrototype             ( Renderer * renderer);

unsigned int        DefaultPrototypeWidth               ();
unsigned int        DefaultPrototypeHeight              ();
bool                DefaultPrototypeFullscreenSetting   ();
WindowMode          DefaultPrototypeWindowModeSetting   ();

} // bv
