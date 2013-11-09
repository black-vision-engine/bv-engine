#pragma once
#include "BlackTree.h"

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
		model::BasicNode* BuildTree(string path);
		void SendTree(BlackNode &CurrentNode,int depth=0);
		

	};

}