#pragma once

#include "Engine/Models/UndoRedo/IRevertable.h"


namespace bv {

namespace model {

class IModelNode;
DEFINE_PTR_TYPE( IModelNode );
//class SceneModel;
//DEFINE_PTR_TYPE( SceneModel );
class IPlugin;
DEFINE_PTR_TYPE( IPlugin );

} // model

class AddPluginOperation : public IRevertable
{
    model::IModelNodePtr    m_parent;
    model::IPluginPtr       m_plugin;
    int                     m_idx;

public:
                            AddPluginOperation  ( model::IModelNodePtr parent, model::IPluginPtr plugin, int idx );

    virtual bool            Undo                ( BVProjectEditor * editor );
    virtual bool            Redo                ( BVProjectEditor * editor );
};

class DeletePluginOperation : public IRevertable
{
    model::IModelNodePtr    m_parent;
    model::IPluginPtr       m_plugin;
    int                     m_idx;

public:
                            DeletePluginOperation ( model::IModelNodePtr parent, model::IPluginPtr plugin, int idx );

    virtual bool            Undo                ( BVProjectEditor * editor );
    virtual bool            Redo                ( BVProjectEditor * editor );
};

} // bv
