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
