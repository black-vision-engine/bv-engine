#pragma once


#include "IndexedGeometry.h"

namespace bv { namespace model {



class HelperSmoothMesh
{
private:

protected:
	void privateSmooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	void tesselate( IndexedGeometry& mesh, IndexedGeometry& resultMesh );
	void moveVerticies( IndexedGeometry& mesh, std::vector<unsigned short>& edges, IndexedGeometry& resultMesh );

public:
	HelperSmoothMesh(void);
	~HelperSmoothMesh(void);

	void smooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation, IndexedGeometry& resultMesh );
	IndexedGeometry smooth( IndexedGeometry& mesh, std::vector<unsigned short>& edges, unsigned int tesselation );
};

} //model
} //bv