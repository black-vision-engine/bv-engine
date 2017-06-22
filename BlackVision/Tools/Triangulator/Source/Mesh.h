#pragma once

#include "glm.hpp"

#include <vector>


class Mesh
{
public:
	typedef std::vector< glm::vec3 > MeshSegment;

private:
protected:

	std::vector< MeshSegment >		m_segments;
	int								m_current;
	bool							m_singleSegment;

public:
	explicit	Mesh();
	explicit	Mesh( bool singleSegment );


    void		AddPoint	( float x, float y, float z );
	void		AddPoint	( glm::vec3 & point );
    void		Begin		();
    void		End			();

	bool		IsSingleSegment	()	{ return m_singleSegment; }

	std::vector< MeshSegment > &		GetMeshSegments	()		{ return m_segments; }
};

