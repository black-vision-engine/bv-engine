#pragma once

#include "Interfaces/IBasicLogic.h"
#include "BasicWindowApp.h"

namespace bv {

class TestLogic : public IBasicLogic
{
private:
	static bool			ms_appInitialized;

#ifdef _DEBUG
	static bool			ms_debugConsole;
#endif

public:
	TestLogic			( Renderer * renderer, audio::AudioRenderer * audioRenderer );
    ~TestLogic			();
	
	virtual     void    Initialize          () override;

	virtual     void    Render              () override;
	virtual     void    Update              ( TimeType t ) override;
	virtual     void    Key                 ( unsigned char c ) override;

	virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

	static IBasicLogicUnqPtr	Create			( Renderer * renderer, audio::AudioRenderer * audioRenderer );
};


} // bv
