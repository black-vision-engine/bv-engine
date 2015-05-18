#pragma once

#include <vector>
#include "Mathematics/glm_inc.h"

namespace bv { namespace model {


class IndexedGeometry
{
private:
	std::vector<unsigned short>		indicies;
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

	std::vector<unsigned short>& getIndicies() { return indicies; }
	std::vector<glm::vec3>& getVerticies() { return verticies; }
};


class HelperSmoothMesh
{
private:

protected:
	void private_smooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	void tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh );
	void move_verticies( IndexedGeometry& mesh, std::vector<unsigned short>& edges, IndexedGeometry& resultMesh );

public:
	HelperSmoothMesh(void);
	~HelperSmoothMesh(void);

	void smooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
};

} //model
} //bv