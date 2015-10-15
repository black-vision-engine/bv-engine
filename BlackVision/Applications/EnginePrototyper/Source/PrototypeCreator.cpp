#include "PrototypeCreator.h"

#include <cassert>

#include "Prototypes/VAO/SimpleVAOPrototype0.h"
#include "Prototypes/FBOProfiling/FBOProfilingPrototype0.h"
#include "Prototypes/FBOProfiling/FBOProfilingPrototype1.h"
#include "Prototypes/FBOProfiling/FBOProfilingPrototype2.h"
#include "Prototypes/FBOProfiling/FBOProfilingPrototype3.h"
#include "Prototypes/Engine/MemManagement/MemManagementInspector.h"
#include "Prototypes/Engine/MemManagement/PboUpdatesInspector.h"
#include "Prototypes/DrawingCommands/DrawingPrototype.h"
#include "Prototypes/Engine/Multipass/MultipassTest.h"

namespace  bv {

bool			CreatePrototype			()
{
	bool success = false;
	
	success = BasicWindowApp::RegisterInitializer( &SimpleVAOPrototype0::Create, "SimpleVAOPrototype0", 800, 600 );

	//success = BasicWindowApp::RegisterInitializer( &PboUpdatesInspector::Create, "PboUpdatesInspector", 800, 600 );

	//success = BasicWindowApp::RegisterInitializer( &MemManagementInspector::Create, "MemManagementInspector", 800, 600 );

	//success = BasicWindowApp::RegisterInitializer( &FBOProfilingPrototype0::Create, "FBOProfilingPrototype0", 800, 600 );
	//success = BasicWindowApp::RegisterInitializer( &FBOProfilingPrototype1::Create, "FBOProfilingPrototype1", 800, 600 );
	//success = BasicWindowApp::RegisterInitializer( &FBOProfilingPrototype2::Create, "FBOProfilingPrototype2", 800, 600 );
	//success = BasicWindowApp::RegisterInitializer( &FBOProfilingPrototype3::Create, "FBOProfilingPrototype3", 800, 600 );

	//success = BasicWindowApp::RegisterInitializer( &DrawingPrototype::Create, "DrawingPrototype", 800, 600 );

	//success = BasicWindowApp::RegisterInitializer( &MultipassTest::Create, "MultipassTest", 800, 600 );

	BasicWindowApp::RegisterConsoleInitializer();

	assert( success );
	
	return success;
}

} // bv
