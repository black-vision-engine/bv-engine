#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Editors/BVProject.h"

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

    BVProject *					m_project;
    Renderer *					m_renderer;

    model::IPluginPtr			m_copiedPlugin;

    model::ITimeEvaluatorPtr	m_timeEvaluator;
    model::TimelineManager *	m_timelineManager;
    
    Int32						m_stepOffset;
    Int32						m_lastStep;

    static const std::string	COL_NODE;
    static const std::string	TEX_NODE;
    static const std::string	ANIM_NODE;
    static const std::string	GRAD_NODE;
    static const std::string	TXT_NODE;
    static const std::string	TMR_NODE;
    static const std::string	GEOM_NODE;
    static const std::string	VSD_NODE;

    static const std::string	SCENE_NAME;
    static const std::string	SCENE_NAME1;
    static const std::string	EMPTY_SCENE;
    
    static const std::string	TIMELINE_NAME;
    static const std::string	TIMELINE_NAME1;
    
public:
                            TestScene				( BVProject * scene, Renderer * renderer );
                            ~TestScene				();

    bool					TestEditor				( TimeType time );

private:

    void					InitTestEditor				();

    void					InitTestModelSceneEditor	();

    void					InitTimelinesTest			();

    void					InitAssetsTest				();

    void                    InitCopyNodeTest            ();

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
    void					InitTexturedGeometryTest	();
    void					InitAnimatedGeometryTest	();
    void					InitGradientGeometryTest	();

    void					InitVideoStreamDecoderTest	();
    
    void					InitOrderTest				( const OrderTestCase & test );
    void					SwapPlugins					( const std::string & rootPlugin, UInt32 rootIdx, const std::string & childName, const std::string & childPlugin,  UInt32 childIdx );
    void					CopyPlugin					( UInt32 rootIdx, const std::string & rootPlugin, const std::string & childName, const std::string & childPlugin );
    void					RestoreRoot					( UInt32 rootIdx, const std::string & childPlugin );

    void                    Wait                        ( UInt32 sec );
};

DEFINE_PTR_TYPE( TestScene )
DEFINE_CONST_PTR_TYPE( TestScene )


} // bv
