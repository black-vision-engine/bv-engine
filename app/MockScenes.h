#pragma once

#include "Mathematics\Interpolators\Interpolators.h"

namespace bv
{
    
namespace model 
{
    class BasicNode;
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

    static  model::BasicNode *      SimpeTextureTestScene   ();
    static  model::BasicNode *      SimpeTextTestScene      ();
    static  model::BasicNode *      SimpleMultiCCScene      ();


    static  model::BasicNode *      AnotherTestScene        ();
    static  model::BasicNode *      AnimatedTestScene       ();

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