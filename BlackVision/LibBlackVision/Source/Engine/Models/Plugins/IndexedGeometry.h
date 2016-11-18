#pragma once

#include <vector>
#include "CoreDEF.h"
#include "Mathematics/glm_inc.h"


namespace bv { namespace model {


typedef UInt32 IndexType;

class IndexedGeometry
{
private:
	std::vector<IndexType>			indicies;
	std::vector<glm::vec3>			verticies;
public:
	IndexedGeometry()   {}
	~IndexedGeometry()  {}
    IndexedGeometry( IndexedGeometry & geometry )   : indicies( geometry.indicies ), verticies( geometry.verticies ) {}
	IndexedGeometry( IndexedGeometry && geometry ) : indicies( std::move( geometry.indicies) ), verticies( std::move( geometry.verticies ) ) {}

    IndexedGeometry& operator=( IndexedGeometry & geometry )
    {
        if( this != &geometry )
        {
            indicies = geometry.indicies;
            verticies = geometry.verticies;
        }

        return *this;
    }

	IndexedGeometry& operator=( IndexedGeometry && geometry )
	{
		if( this != &geometry )
		{
			indicies = std::move( geometry.indicies );
			verticies = std::move( geometry.verticies );
		}

		return *this;
	}

	std::vector< IndexType > &       GetIndicies     () { return indicies; }
	std::vector< glm::vec3 > &        GetVerticies    () { return verticies; }
};


}
}
