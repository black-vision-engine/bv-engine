#include "Scena.h"
BlackNode* Scena::FindNode(BlackNode *LookFor,string name)
{
	BlackNode* node=NULL;
	for(int i=0;i<LookFor->children.size();i++)
	{
		if(LookFor->children[i].name==name)
		{
			node = &LookFor->children[i];
			break;
		}
		node = FindNode(&LookFor->children[i],name);
		if(node!=NULL)break;
	}
	return node;

}


