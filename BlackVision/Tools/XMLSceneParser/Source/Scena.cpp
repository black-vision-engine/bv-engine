#include "Scena.h"

// *************************************
//
BlackNode * Scena::FindNode( BlackNode * LookFor, std::string name )
{
	BlackNode * node = NULL;

	for( unsigned int i = 0; i < LookFor->children.size(); ++i )
	{
		if( LookFor->children[ i ].name == name )
		{
			node = &LookFor->children[ i ];
			break;
		}

		node = FindNode( &LookFor->children[ i ], name );

        if (node != NULL )
        {
            break;
        }
	}

	return node;
}

void Scena::ParseMetaTags()
{
    for(unsigned int i=0 ; i< MetaTags.size();i++)
    {
        if(MetaTags[i]->key=="textures_directory")
        {
            Meta.DefaultTextureDirectory = MetaTags[i]->value;
        }
        if(MetaTags[i]->key=="scene_name")
        {
            Meta.SceneName = MetaTags[i]->value;
        }
    }
}
