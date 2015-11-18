#pragma once

#include "Interfaces/IBasicLogic.h"
#include "BasicWindowApp.h"

#include "Scenes/TestScene.h"

namespace bv {

class TestLogic : public IBasicLogic
{
private:
	static bool			ms_appInitialized;

#ifdef _DEBUG
	static bool			ms_debugConsole;
#endif

	TestScenePtr					m_scene;

    model::TimelineManagerPtr        m_timelineManager;
    model::OffsetTimeEvaluatorPtr   m_globalTimeline;

	Renderer *						m_renderer;

	TimeType						m_time;

public:
	TestLogic			( Renderer * renderer );
    ~TestLogic			();
	
	virtual     void    Initialize          () override;

	virtual     void    Render              () override;
	virtual     void    Update              ( TimeType t ) override;
	virtual     void    Key                 ( unsigned char c ) override;

	virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

	static IBasicLogicUnqPtr	Create			( Renderer * renderer );

private:

	void				Draw				( SceneNode * node );
};


} // bv
