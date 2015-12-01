#include "IReplicationModifier.h"

namespace bv { namespace model
{

class ShiftReplicationModifier;

DEFINE_CONST_PTR_TYPE( ShiftReplicationModifier )

class ShiftReplicationModifier : public IReplicationModifier
{
public:

    virtual void                            Apply   ( const BasicNodeConstPtr & prev, const BasicNodePtr & next ) const override;

    static ShiftReplicationModifierConstPtr Create  ( const glm::vec3 & delta );

private:

    explicit                ShiftReplicationModifier ( const glm::vec3 & delta );

    glm::vec3               m_delta;

};

} // model
} // bv