#pragma once

#include "Engine/Interfaces/IUpdater.h"


namespace bv {

class SceneNode;

namespace model
{
    class IModelNode;
}

class NodeUpdater : public IUpdater
{
private:

    SceneNode *                 m_out;
    const model::IModelNode *   m_in;

public:

                    NodeUpdater     ( SceneNode * out, const model::IModelNode * in ); 
                    ~NodeUpdater    ();

    virtual void    DoUpdate        ( TimeType t );

};

}
