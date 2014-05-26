#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"

#include "xml/TreeBuilder.h"
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

    static  model::BasicNode *      NewModelTestScene           ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator );

    static  model::BasicNode *      SimpeTextureTestScene       ();
    static  model::BasicNode *      SimpleMultiCCScene          ();


    static  model::BasicNode *      AnotherTestScene            ();
    
    static  model::BasicNode *      AnimatedTestScene           ();
    static  model::BasicNode *      GreenRectTestScene          ();
    static  model::BasicNode *      TexturedRectTestScene       ();
    static  model::BasicNode *      NaiveTimerTestScene         ();

    static  model::BasicNode *      TestSceneVariableTopology   ();
    static  model::BasicNode *      SequenceAnimationTestScene  ();
    static  model::BasicNode *      NonGeometryParent           ();

	static  model::BasicNode *      XMLTestScene                ();

    static  model::BasicNode *      StackThemNow                ( model::BasicNode * n0, model::BasicNode * n1 );

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

    static model::BasicNode* BuildMockScene    ( model::BasicNode * parent = nullptr );
    static model::BasicNode* BuildMockScene2   ( model::BasicNode * parent = nullptr );
	static model::BasicNode* CreateSceneMock3  ( model::BasicNode * parent );

};
}
