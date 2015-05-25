#pragma once

#include <vector>
#include "Mathematics/glm_inc.h"


namespace bv { namespace model {


typedef unsigned short INDEX_TYPE;

class IndexedGeometry
{
private:
	std::vector<INDEX_TYPE>		indicies;
	std::vector<glm::vec3>			verticies;
public:
	IndexedGeometry() {}
	~IndexedGeometry() {}
	IndexedGeometry( IndexedGeometry&& geometry ) : indicies( std::move( geometry.indicies) ), verticies( std::move( geometry.verticies ) ) {}

	IndexedGeometry& operator=( IndexedGeometry&& geometry )
	{
		if( this != &geometry )
		{
			indicies = std::move( geometry.indicies );
			verticies = std::move( geometry.verticies );
		}

		return *this;
	}

	std::vector<INDEX_TYPE>& getIndicies() { return indicies; }
	std::vector<glm::vec3>& getVerticies() { return verticies; }
};


}
}