#pragma once
#include "BlackTree.h"
#include "Engine/Models/BasicNode.h"

namespace bv
{
	namespace model 
	{
		class BasicNode;
	}

	class TreeBuilder
	{
	public:
		TreeBuilder(void);
		~TreeBuilder(void);
		model::BasicNodePtr BuildTree(string path);
		void SendTree(BlackNode &CurrentNode,int depth=0);
		

	};

}