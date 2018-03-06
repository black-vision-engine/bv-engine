#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/SceneModel.h"
#include "Mathematics/Interpolators/Interpolators.h"




namespace bv
{
    
namespace model 
{
    class BasicNode;
    class PluginsManager;
    class TimelineManager;
}



// ********************************
class TestParamFactory
{
public:

    static FloatInterpolator    ConstantValue( float val );

};


// ********************************
class TestScenesFactory
{
public:

    enum class TestSceneSelector : int
    {
        TSS_ONE_SOLID_COLOR_RECTANGLE,
        TSS_ONE_TEXTURED_RECTANGLE,
        TSS_TWO_TEXTURED_RECTANGLES,
        TSS_TEXT,
        TSS_ANIMATION_RECTANGLE,
        TSS_SOLID_RECTANGLE,
        TSS_GLOBAL_EFFECT_05,

        TSS_TOTAL
    };

public:

    static  model::BasicNodePtr		CreateSceneFromEnv              ( const std::string& scene, model::ITimeEvaluatorPtr timeline, const model::PluginsManager * pluginsManager );
    static  model::BasicNodePtr     CreateTestRandomNode            ( const std::string & name, const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreateTestScene                 ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, TestSceneSelector tss );

    static  model::BasicNodePtr     GlobalEffect05                  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     LightScatteringTestScene        ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     MultiShadowEffectTestScene      ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     TimerTestScene                  ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     BlurEffectTestScene             ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     ShadowEffectTestScene           ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     ImageMaskEffectTestScene        ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );

    static  model::BasicNodePtr     ScrollerNodeTestScene            ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     NewModelTestScene               ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     OlafTestScene                   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreedDeprecatedTestScene        ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreedPrimitivePieChartTestScene ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreedPrismTestScene             ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreedPieChartTestScene          ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreedPrismBugTestScene          ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreedCosineDemoScene            ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     NodeReplicatorTestScene         ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     MultiGlobalEffectTestScene      ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     CreedBasicGeometryTestScene     ( model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     MeshTestScene                   ( model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     EnvMappingTestScene             ( model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     BlendTextureTestScene           ( model::ITimeEvaluatorPtr timeEvaluator );
        
    static  model::BasicNodePtr     SimpeTextureTestScene           ();
    static  model::BasicNodePtr     SimpleMultiCCScene              ();
                            
                            
    //static  model::BasicNodePtr     AnotherTestScene                ();
                            
    static  model::BasicNodePtr     GreenRectTestScene              ();
    static  model::BasicNodePtr     TexturedRectTestScene           ();
    static  model::BasicNodePtr     NaiveTimerTestScene             ();
                            
    static  model::BasicNodePtr     SequenceAnimationTestScene      ();
    static  model::BasicNodePtr     NonGeometryParent               ();
                            
    static  model::BasicNodePtr     XMLTestScene                    ();
    //static  model::SceneModelPtr    CreateSerializedTestScene       ( const model::PluginsManager * pluginsManager );
                            
    static  model::BasicNodePtr     StackThemNow                    ( model::BasicNodePtr n0, model::BasicNodePtr n1 );

    static	model::BasicNodePtr		BasicShapesShowScene		    ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static	model::BasicNodePtr		BasicShapesTest				    ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );

    static  model::BasicNodePtr     AssetCacheTestScene             ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     RemoteEventsTestScene           ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     FontTestScene                   ( const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator );

    static  model::BasicNodePtr     FadeRectTestScene               ( model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     ExtrudeTestScene                ( model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     LineChartTestScene              ( model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     SVGTestScene                    ( model::ITimeEvaluatorPtr timeEvaluator );
    static  model::BasicNodePtr     Text3DTestScene                 ( model::ITimeEvaluatorPtr timeEvaluator );
};

// ********************************
class TestParametersFactory
{
public:
    
    static void CameraAnimation_ver1( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up );
    static void CameraAnimation_ver2( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up );
};

// ********************************
struct SceneExamples
{

    static model::BasicNodePtr BuildMockScene    ( model::BasicNodePtr parent = nullptr );
    static model::BasicNodePtr BuildMockScene2   ( model::BasicNodePtr parent = nullptr );
    static model::BasicNodePtr CreateSceneMock3  ( model::BasicNodePtr parent );

};
}
