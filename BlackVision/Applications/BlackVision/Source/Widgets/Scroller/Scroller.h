#pragma once

#include "CoreDEF.h"
#include "Engine/Models/Interfaces/INodeLogic.h"
#include "ScrollerNodesStates.h"
#include <string>
#include <vector>
#include <map>

// forward references
namespace bv {

namespace mathematics {

struct Rect;
DEFINE_PTR_TYPE( Rect )
}

namespace model {

class BasicNode;
DEFINE_PTR_TYPE( BasicNode )

} // model
} // bv

namespace bv { namespace nodelogic {

class Scroller;

DEFINE_PTR_TYPE( Scroller )
DEFINE_CONST_PTR_TYPE( Scroller )

class Scroller : public model::INodeLogic, public std::enable_shared_from_this< Scroller >
{
public:

    enum ScrollDirection
    {
        SD_Up,
        SD_Down,
        SD_Left,
        SD_Right,
        
        SD_Total
    };

    enum OffscreenNodeBehavior
    {
        ONB_Looping,
        ONB_DeleteNode,
        ONB_SetNonActive,

        ONB_Total
    };

    enum ScrollerItemType
    {
        SIT_Enqued,
        SIT_OnScreen,
        SIT_OffScreen,
        SIT_All
    };

private:
    static const std::string        m_type;


	typedef std::map< bv::model::BasicNode *, Float32 > NodeFloatMap;
	typedef std::map< bv::model::BasicNode *, bool >	NodeBoolMap;

private:

    std::string                             m_scrollerNodePath;
    std::string                             m_sceneName;

	bool									m_isFinalized;
	bv::model::BasicNodePtr					m_parentNode;
	ScrollerNodesStates						m_nodesStates;
	NodeFloatMap							m_shifts;
	UInt64									m_currTime;
    BVProjectEditor *                       m_editor;

    // Smooth
    bool                                    m_smoothStart;
    bool                                    m_smoothPause;
    UInt64                                  m_smoothTime;
    UInt64                                  m_smoothStartTime;

    mathematics::RectPtr				    m_view;
	Float32									m_speed;
	Float32									m_interspace;
    
    ScrollDirection                         m_scrollDirection;
    OffscreenNodeBehavior                   m_offscreenNodeBehavior;

    bool                                    m_lowBufferNotified;
    Float32                                 m_lowBufferMultiplier;  /// Determines when to notify LowBuffer.

	bool									m_started;
    bool                                    m_paused;
    bool                                    m_enableEvents;

private:

    Float32     InitialShift        ( model::BasicNode * node );
    Float32     ShiftStep           ( model::BasicNode * prevNode, model::BasicNode * node );
    Float32     SignedShift         ( Float32 shift );
    Float32     Smooth              ( UInt64 time, Float32 shift );

	void		LayoutNodes			();
	void		UpdateTransforms	();
	void		UpdateVisibility	( bv::model::BasicNode * );
	void		SetActiveNode		( bv::model::BasicNode * );
	bool		IsActive			( bv::model::BasicNode * );
    bool        CheckLowBuffer      ();
    void        ShiftNodeToEnd      ( bv::model::BasicNode * n );

    void		OnNotifyVisibilityChanged       ( bv::model::BasicNode * n, bool visibility );
    void		OnNotifyNodeOffscreen           ( bv::model::BasicNode * n );
	void		NotifyVisibilityChanged         ( bv::model::BasicNode *, bool );
	void		NotifyNoMoreNodes	            ();
    void        NotifyLowBuffer                 ();


    bool        AddNode             ( bv::model::BasicNodePtr node );

public:
	explicit	Scroller            ( bv::model::BasicNodePtr parent, const mathematics::RectPtr & view );
				~Scroller			() {}

	void		AddNext				( bv::model::BasicNodePtr node );
    bool		AddNext				( Int32 nodeIdx );
    bool		AddNext				( const std::string& childNodeName );
	bool		Finalize			();
    bool        Unfinalize          ();



	void		SetSpeed			        ( Float32 speed );
	void		SetInterspace		        ( Float32 interspace );
    void        SetNodePath                 ( std::string nodePath );
    void        SetLowBufferMult            ( Float32 lowBufferVal );
    void        SetScrollDirection          ( ScrollDirection scrollDirection );
    void        SetEnableEvents             ( bool enable );
    bool        SetSmoothTime               ( Float32 time );
    Float32     GetSmoothTime               () const;
    void        SetOffscreenNodeBehavior    ( OffscreenNodeBehavior behavior );



	virtual void	            Initialize		()				override {}
	virtual void	            Update			( TimeType t )	override;
	virtual void	            Deinitialize	()				override {}


    virtual const std::string &                             GetType         () const override;
    static const std::string &                              Type            ();

    virtual model::IParameterPtr                            GetParameter    ( const std::string & name ) const override;
    virtual const std::vector< model::IParameterPtr > &     GetParameters   () const override;

    virtual void                                            Serialize       ( ISerializer& ser ) const override;
    static ScrollerPtr                                      Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode );
    static ScrollerPtr                                      Create	        ( bv::model::BasicNodePtr parent, const mathematics::RectPtr & view );

    virtual bool                                            HandleEvent     ( IDeserializer& eventSer, ISerializer& response, BVProjectEditor * editor ) override;

public:
    // Event handler funtions
	bool			Start			    ();
	bool			Stop			    ();
	bool		    Reset				();
    bool            Pause               ();
    bool            Clear               ();

    bool            SmoothStart         ();
    bool            SmoothPause         ();

    void            ListTypedItems      ( std::vector< bv::model::BasicNode * > & items, ISerializer & response, ScrollerItemType type );
    bool            GetItems            ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );

    bool            AddPreset           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );
    bool            AddPresetAndMessages( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );

    bool            GetStatus           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );

private:

    void            AddTexts            ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor, model::BasicNodePtr node );
    void            AddImages           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor, model::BasicNodePtr node );

    model::BasicNodePtr     CreatePreset    ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );
    bool                    AddPresetToScene( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor, model::BasicNodePtr node );
};

} 
} // bv
