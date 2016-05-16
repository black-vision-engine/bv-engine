#pragma once

#include "Widgets/NodeLogicBase.h"
#include "Engine/Models/BasicNode.h"

#include "CoreDEF.h"


namespace bv { namespace nodelogic {

class PieChart;

DEFINE_PTR_TYPE( PieChart )
DEFINE_CONST_PTR_TYPE( PieChart )


class PieChart:  public model::NodeLogicBase, public std::enable_shared_from_this< PieChart >
{
public:

    enum class PieChartType : int
    {
		PST_COLOR = 0,
        PST_MATERIAL,
        
        PST_TOTAL
	};

    struct PieSliceDesc;
    DEFINE_PTR_TYPE( PieSliceDesc )

    struct PieSliceDesc : public ISerializable
    {

	    Float32     percent;
	    Float32     offset;

	                                PieSliceDesc    ( Float32 p, Float32 o );

        virtual void                Serialize       ( ISerializer & ser ) const override;
        static PieSliceDescPtr      Create          ( const IDeserializer & deser );

    };

private:

    static const std::string        m_type;

    struct ACTION 
    {
        static const std::string    LOAD;
        static const std::string    UPDATE_PIECHART;
        static const std::string    ADD_PIESLICE;
        static const std::string    REMOVE_PIESLICE;
    };

private:

	model::BasicNodePtr				m_parentNode;
    model::ITimeEvaluatorPtr        m_timeEval;

    std::vector< PieSliceDescPtr >                      m_slicesDesc;
    std::map< PieSliceDescPtr, model::BasicNodePtr >    m_slices;

    PieChartType                    m_chartType;

public:

	explicit                        PieChart            ( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval, const std::vector< PieSliceDescPtr > & slicesDesc, PieChartType chartType );
	                                ~PieChart           ();

	static PieChartPtr              Create              ( model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval, const std::vector< PieSliceDescPtr > & slicesDesc, PieChartType chartType );

    virtual void                    Serialize           ( ISerializer & ser ) const override;
    static PieChartPtr              Create              ( const IDeserializer & deser, model::BasicNodePtr parent );

	virtual void	                Initialize		    ()				override {}
	virtual void	                Update			    ( TimeType t )	override;
	virtual void	                Deinitialize	    ()				override {}

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();

    virtual bool                    HandleEvent         ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor ) override;

    void                            Load                ( IDeserializer & eventSer, BVProjectEditor * editor );

private:

    void                            SetType             ( model::BasicNodePtr node );
    void                            AddText             ( model::BasicNodePtr node, Float32 percent );

};

} //nodelogic
} //bv
