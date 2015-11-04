#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/BVScene.h"

#include "BVGL.h"

namespace bv {

struct OrderTestCase
{
	std::string					node;
	std::string					test;
	std::vector< std::string >	plugins;

	OrderTestCase	( const std::string & node, const std::string & test, const std::vector< std::string > & plugins );
};

class TestScene
{
private:
	std::vector< std::function< void() > > m_testSteps;

    BVScenePtr					m_scene;
	Renderer *					m_renderer;
	model::TimelineManager *	m_timelineManager;
	model::ITimeEvaluatorPtr	m_timeEvaluator;
	
	Int32						m_stepOffset;
	Int32						m_lastStep;

	static const std::string	COL_NODE;
	static const std::string	TEX_NODE;
	static const std::string	ANIM_NODE;
	static const std::string	GRAD_NODE;
	static const std::string	TXT_NODE;
	static const std::string	TMR_NODE;
	static const std::string	GEOM_NODE;

public:
							TestScene				( Renderer * renderer, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
							~TestScene				();

	void					Restart					();

	BVScenePtr				GetScene				();
	
	void					TestModelSceneEditor	();
	void					TestModelNodeEditor		( TimeType time );

	BVScenePtr				ColoredRectanglesScene	();
	
private:
	void					InitTestModelNodeEditor		();

	void					InitBasicColorPluginTest	();
	void					InitOrderColorPluginTest	();

	void					InitBasicTexturePluginTest	();
	void					InitOrderTexturePluginTest	();

	void					InitBasicAnimationPluginTest();
	void					InitOrderAnimationPluginTest();

	void					InitBasicGradientPluginTest	();
	void					InitOrderGradientPluginTest	();

	void					InitColoredTextTest			();
	void					InitGradientTextTest		();

	void					InitColoredTimerTest		();
	void					InitGradientTimerTest		();

	void					InitColoredGeometryTest		();

	void					InitOrderTest				( const OrderTestCase & test );
	void					SwapPlugins					( const std::string & rootPlugin, UInt32 rootIdx, const std::string & childName, const std::string & childPlugin,  UInt32 childIdx );
};

DEFINE_PTR_TYPE( TestScene )
DEFINE_CONST_PTR_TYPE( TestScene )


} // bv
