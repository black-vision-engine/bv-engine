#include "PluginGeometryRing.h"

namespace bv
{

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string PluginGeometryRingPD::pluginName( "geometry.ring" );

// *************************************
//
PluginGeometryRingPD::PluginGeometryRingPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
PluginGeometryRing::PluginGeometryRing( const FloatInterpolator & innerRadius, const FloatInterpolator & outerRadius,
										const FloatInterpolator & startAngle, const FloatInterpolator & endAngle, int numSegments )
										: m_InnerRadius( innerRadius )
										, m_OuterRadius( outerRadius )
										, m_StartAngle( startAngle )
										, m_EndAngle( endAngle )
										, m_numSegments( numSegments )
{
	TriStripRep();
}

void				PluginGeometryRing::TriStripRep             ()
{
	float innerRadius = m_InnerRadius.evaluate(0.f);
	float outerRadius = m_OuterRadius.evaluate(0.f);



	float angleStart = m_StartAngle.evaluate(0.f);
	float angleEnd = m_EndAngle.evaluate(0.f);

	glm::vec2 xyInner = glm::vec2( innerRadius * cos(angleStart), innerRadius * sin(angleStart) );
	glm::vec2 xyOuter = glm::vec2( outerRadius * cos(angleStart), outerRadius * sin(angleStart) );


	m_vertices.push_back(glm::vec3(xyOuter.x, xyOuter.y, 0.f));
	m_vertices.push_back(glm::vec3(xyInner.x, xyInner.y, 0.f));

	float angleDev = angleEnd - angleStart;

	float step = angleDev / float(m_numSegments);

	int i = 0;

	for(float ang = angleStart + step; i < m_numSegments; ++i, ang += step)
	{
		glm::vec2 xyInner = glm::vec2( innerRadius * cos(ang), innerRadius * sin(ang) );
		glm::vec2 xyOuter = glm::vec2( outerRadius * cos(ang), outerRadius * sin(ang) );

		m_vertices.push_back(glm::vec3(xyOuter.x, xyOuter.y, 0.f));
		m_vertices.push_back(glm::vec3(xyInner.x, xyInner.y, 0.f));
	}

    m_adjacency = Adjacency::A_TRIANGLE_STRIP;
}

// *************************************
//
bool                PluginGeometryRing::IsTimeInvariant     () const
{
	return true;
}

// *************************************
//
PluginGeometryRing::Adjacency PluginGeometryRing::AdjacencyType       () const
{
	return m_adjacency;
}

// *************************************
//
void                PluginGeometryRing::ConvertToTraingles  ()
{

}

// *************************************
//
void                PluginGeometryRing::ConvertToMesh       ()
{
}

// *************************************
//
bool                PluginGeometryRing::IsCW                () const
{
	return !m_ccw;
}

// *************************************
//
bool                PluginGeometryRing::IsCCW               () const
{
	return m_ccw;
}

// *************************************
//
void                PluginGeometryRing::SetCW               ()
{

}

// *************************************
//
void                PluginGeometryRing::SetCCW              ()
{
}

// *************************************
//
bool                PluginGeometryRing::IsVertexNumConst    () const
{
	return true;
}

// *************************************
//
bool                PluginGeometryRing::IsAdjacencyConst    () const
{
	return true;
}

// *************************************
//
bool                PluginGeometryRing::IsVertexTypeConst   () const
{
	return true;
}

// *************************************
//
void				PluginGeometryRing::Update				(float t)
{

}


//PluginGeometry::Role                PluginGeometryRing::GetRole             () const
//{
//	return Role::R_SOURCE;
//}
//
//void                PluginGeometryRing::SetRole             ( Role role ) 
//{
//
//}

} // bv
