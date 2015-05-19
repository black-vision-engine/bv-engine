#pragma once


#include "IndexedGeometry.h"

namespace bv { namespace model {





class HelperSmoothMesh
{
private:

protected:
	void privateSmooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	void tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh );
	void moveVerticies( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, IndexedGeometry& resultMesh );
	bool findVertex( const std::vector<glm::vec3>& verticies, glm::vec3 vertex, INDEX_TYPE& index );
public:
	HelperSmoothMesh(void);
	~HelperSmoothMesh(void);

	void smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	IndexedGeometry smooth( IndexedGeometry& mesh, std::vector<INDEX_TYPE>& edges, unsigned int tesselation );
};

} //model
} //bv