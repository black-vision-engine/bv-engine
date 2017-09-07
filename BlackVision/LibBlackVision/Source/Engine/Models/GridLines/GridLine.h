#pragma once

#include <string>

#include "Engine/Models/BasicNode.h"


namespace bv { namespace model
{

enum GridLineType
{
    TST_Vertical,
    TST_Horizontal,

    TST_TOTAL
};



enum GridLineAlignement
{
    TSA_GeometryCenter,     ///< Point ( 0.0, 0.0, 0.0 )
    TSA_WeightCenter,
    TSA_BoundingBoxCenter,
    TSA_BoundingBoxMajor,   ///< Maximal coordinate of bounding box
    TSA_BoundingBoxMinor,   ///< Minimal coordinate of bounding box

    TSA_TOTAL
};



class GridLine
{
private:

    GridLineType        m_type;
    std::string         m_name;
    float               m_position;

public:
    explicit GridLine    ( GridLineType type, const std::string & name );
    ~GridLine();


    void                SetPosition         ( float pos )   { m_position = pos; }
    float               GetPosition         ()              { return m_position; }
    const std::string & GetName             ()              { return m_name; }

    bool                AlignNode           ( model::BasicNodePtr node, GridLineAlignement alignement );
    void                Rename              ( const std::string & newName );

private:

    glm::vec3           ReferencePos        ( model::BasicNodePtr& node, GridLineAlignement alignement );
    glm::vec3           ComputeTranslation  ( glm::vec3 referencePosition );
    bool                UpdateTransform     ( model::BasicNodePtr& node, glm::vec3 translation );
};


}   // model


DECLARE_ENUM_SERIALIZATION( model::GridLineType )
DECLARE_ENUM_SERIALIZATION( model::GridLineAlignement )

}	// bv
