#include "PrototypeCreator.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// ****************************
//
IAppLogicPrototype * CreateDefaultPrototype     ( Renderer * renderer )
{
    { renderer; }
    return new SimpleVAOPrototype0();
    // return new MemManagementInspector( renderer );
}

} // bv
