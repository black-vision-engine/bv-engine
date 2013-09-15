#pragma once

#include "Mathematics\Interpolators\Interpolators.h"

namespace bv
{
class BasicNode;

// ********************************
class TestScenesFactory
{
public:

    static  BasicNode *     SimpeTextureTestScene();

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

    static BasicNode* BuildMockScene    ( BasicNode * parent = nullptr );
    static BasicNode* BuildMockScene2   ( BasicNode * parent = nullptr );
	static BasicNode* CreateSceneMock3  ( BasicNode * parent );

};
}