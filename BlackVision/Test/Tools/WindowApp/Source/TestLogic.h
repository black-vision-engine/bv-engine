#pragma once

#include "Interfaces/IBasicLogic.h"
#include "BasicWindowApp.h"

namespace bv {

class TestLogic : public IBasicLogic
{
private:
	static bool			ms_appInitialized;

public:
	virtual     void    Initialize          ( Renderer * renderer );

	virtual     void    Render              ();
	virtual     void    Update              ( TimeType t );
	virtual     void    Key                 ( unsigned char c );

	virtual     void    Resize              ( UInt32 w, UInt32 h );

    virtual             ~TestLogic			();
};


} // bv
