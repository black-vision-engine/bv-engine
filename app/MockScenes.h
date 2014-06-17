#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/BasicNode.h"
#include "Mathematics/Interpolators/Interpolators.h"

#include "xml/TreeBuilder.h"



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

    static  model::BasicNodePtr     NewModelTestScene           ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );
                            
    static  model::BasicNodePtr     SimpeTextureTestScene       ();
    static  model::BasicNodePtr     SimpleMultiCCScene          ();
                            
                            
    static  model::BasicNodePtr     AnotherTestScene            ();
                            
    static  model::BasicNodePtr     AnimatedTestScene           ();
    static  model::BasicNodePtr     GreenRectTestScene          ();
    static  model::BasicNodePtr     TexturedRectTestScene       ();
    static  model::BasicNodePtr     NaiveTimerTestScene         ();
                            
    static  model::BasicNodePtr     TestSceneVariableTopology   ();
    static  model::BasicNodePtr     SequenceAnimationTestScene  ();
    static  model::BasicNodePtr     NonGeometryParent           ();
                            
	static  model::BasicNodePtr     XMLTestScene                ();
                            
    static  model::BasicNodePtr     StackThemNow                ( model::BasicNodePtr n0, model::BasicNodePtr n1 );

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
