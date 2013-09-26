#pragma once

#include "Mathematics\Interpolators\Interpolators.h"

namespace bv
{
    
namespace model 
{
    class BasicNode;
}

// ********************************
class TestScenesFactory
{
public:

    static  model::BasicNode *     SimpeTextureTestScene();

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