#pragma once

#include "Engine/Models/UndoRedo/IRevertable.h"


namespace bv {

namespace model {

class BasicNode;
DEFINE_PTR_TYPE( BasicNode );
//class SceneModel;
//DEFINE_PTR_TYPE( SceneModel );
class IPlugin;
DEFINE_PTR_TYPE( IPlugin );

} // model

class AddPluginOperation : public IRevertable
{
    model::BasicNodePtr     m_parent;
    model::IPluginPtr       m_plugin;
    int                     m_idx;

public:
                            AddPluginOperation  ( model::BasicNodePtr parent, model::IPluginPtr plugin, int idx );

    virtual bool            Undo                ( BVProjectEditor * editor );
    virtual bool            Redo                ( BVProjectEditor * editor );
};

class DeletePluginOperation : public IRevertable
{
    model::BasicNodePtr     m_parent;
    model::IPluginPtr       m_plugin;
    int                     m_idx;

public:
                            DeletePluginOperation ( model::BasicNodePtr parent, model::IPluginPtr plugin, int idx );

    virtual bool            Undo                ( BVProjectEditor * editor );
    virtual bool            Redo                ( BVProjectEditor * editor );
};

} // bv
