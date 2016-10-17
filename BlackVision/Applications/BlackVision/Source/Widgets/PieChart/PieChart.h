#pragma once

#include "Widgets/NodeLogicBase.h"
#include "Engine/Models/SceneModel.h"


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
    DEFINE_CONST_PTR_TYPE( PieSliceDesc )

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
        static const std::string    UPDATE_PIECHART;
        static const std::string    UPDATE_PIESLICE;
        static const std::string    ADD_PIESLICE;
        static const std::string    REMOVE_PIESLICE;
    };

    struct PLUGIN
    {
        static const std::string    TRANSFORM;
        static const std::string    CYLINDER;
        static const std::string    COLOR;
        static const std::string    MATERIAL;
        static const std::string    TEXT;
    };

private:

    model::BasicNodeWeakPtr         m_parentNode;
    model::ITimeEvaluatorPtr        m_timeEval;

    std::vector< PieSliceDescPtr >  m_slicesDesc;

    PieChartType                    m_chartType;
    bool                            m_textEnabled;

    Float32                         m_totalPercent;


public:

    explicit                        PieChart            ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, PieChartType chartType, bool textEnabled );
                                    ~PieChart           ();

    static PieChartPtr              Create              ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, PieChartType chartType, bool textEnabled );

    virtual void                    Serialize           ( ISerializer & ser ) const override;
    static PieChartPtr              Create              ( const IDeserializer & deser, model::BasicNodeWeakPtr parent );

    virtual void                    Initialize          ()              override {}
    virtual void                    Update              ( TimeType t )  override;
    virtual void                    Deinitialize        ()              override {}

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();

    virtual bool                    HandleEvent         ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor ) override;

private:

    void                            AddSlice            ( model::SceneModelPtr scene, PieSliceDescPtr sliceDesc, UInt32 idx, BVProjectEditor * editor );
    bool                            RemoveSlice         ( model::SceneModelPtr scene, UInt32 sliceDescIdx, BVProjectEditor * editor );

    model::BasicNodePtr             CreateSlice         ( PieSliceDescPtr sliceDesc, UInt32 idx );
    void                            AddShaderPlugin     ( model::BasicNodePtr node );
    
    void                            AddLabelNode        ( model::BasicNodePtr node, Float32 percent );
    void                            UpdateLabelNode     ( model::BasicNodePtr node, Float32 percent );
    void                            SetLabelTransform   ( model::BasicNodePtr node, model::BasicNodePtr textNode );
    void                            SetLabelText        ( model::BasicNodePtr node, Float32 percent );

    void                            UpdateChart         ();
    Float32                         UpdateSlice         ( model::BasicNodePtr node, PieSliceDescPtr sliceDesc, Float32 angle );

    std::string                     LabelNodeName       ( const std::string & parentNodeName );
    std::string                     SliceNodeName       ( const std::string & parentNodeName, UInt32 idx );

};

} //nodelogic
} //bv
