#include "stdafx.h"

#include "EffectsOperations.h"

#include "Engine/Editors/BVProjectEditor.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// ***********************
//
SetEffectOperation::SetEffectOperation  ( model::BasicNodePtr parent, model::IModelNodeEffectPtr oldEffect, model::IModelNodeEffectPtr newEffect )
    : m_parent( parent ), m_oldEffect( oldEffect ), m_newEffect( newEffect )
{
}

// ***********************
//
bool            SetEffectOperation::Undo                ( BVProjectEditor * editor )
{
    return editor->SetNodeEffect( m_parent, m_oldEffect );
}

// ***********************
//
bool            SetEffectOperation::Redo                ( BVProjectEditor * editor )
{
    return editor->SetNodeEffect( m_parent, m_newEffect );
}

}