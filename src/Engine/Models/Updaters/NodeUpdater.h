#pragma once

#include "IUpdater.h"

namespace bv {

class SceneNode;
class IModelNode;

class NodeUpdater : public IUpdater
{
private:

    SceneNode *         m_out;
    const IModelNode *  m_in;

public:

                    NodeUpdater     ( SceneNode * out, const IModelNode * in ); 
                    ~NodeUpdater    ();

    virtual void    Update          ( float t );

};

}
