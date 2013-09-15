#pragma once

namespace bv
{
class BasicNode;

struct SceneExamples
{
    static BasicNode* BuildMockScene( BasicNode * parent = nullptr );
    static BasicNode* BuildMockScene2( BasicNode * parent = nullptr );
	static BasicNode* CreateSceneMock3( BasicNode * parent );
};
}