#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Mathematics/Transform/MatTransform.h"


namespace bv { namespace model {

class ParamTransform;
DEFINE_PTR_TYPE( ParamTransform )

// *******************************************
class ParamTransform : public AbstractModelParameter
{
private:

    TransformF m_transformModel;

public:

    explicit            ParamTransform  ( const std::string & name, const TransformF & transform, ITimeEvaluatorPtr evaluator );

    void                Serialize       ( ISerializer& doc ) const;

    void                SetCurveType    ( CurveType type );
    CurveType           GetCurveType    ();

    virtual void        SetWrapPostMethod ( WrapMethod method );
    virtual void        SetWrapPreMethod ( WrapMethod method );
    virtual WrapMethod  GetWrapPostMethod ();
    virtual WrapMethod  GetWrapPreMethod ();

    virtual int         GetNumKeys      ();


    inline  void        SetRotation     ( const glm::vec3 & rotAxis, float angle, TimeType t );
    inline  void        SetScale        ( const glm::vec3 & scale, TimeType t );
    inline  void        SetTranslation  ( const glm::vec3 & translation, TimeType t );
    inline  void        SetCenter       ( const glm::vec3 & center, TimeType t );

    inline  void        RemoveRotation      ( TimeType t );
    inline  void        RemoveScale         ( TimeType t );
    inline  void        RemoveTranslation   ( TimeType t );
    inline  void        RemoveCenter        ( TimeType t );

    inline  bool        MoveRotation        ( TimeType t, TimeType newTime );
    inline  bool        MoveScale           ( TimeType t, TimeType newTime );
    inline  bool        MoveTranslation     ( TimeType t, TimeType newTime );
    inline  bool        MoveCenter          ( TimeType t, TimeType newTime );

    inline  TransformF& Transform       ();

    inline  glm::mat4   Evaluate        () const;

    virtual VoidPtr     QueryParamTyped () override;

    const TransformF &  GetTransformF   () const
    {
        return m_transformModel;
    }

    static  ModelParamType  Type        ()
    {
        return ModelParamType::MPT_TRANSFORM;
    }

};

class ParamTransformVec;
DEFINE_PTR_TYPE(ParamTransformVec)


// *******************************************
class ParamTransformVec : public AbstractModelParameter
{
private:

    std::vector< TransformF >   m_transformModelVec;

public:

    explicit            ParamTransformVec   ( const std::string & name, const TransformF & transform, ITimeEvaluatorPtr evaluator );
    explicit            ParamTransformVec   ( const std::string & name, const ITimeEvaluatorPtr evaluator );

    void                Serialize           ( ISerializer& doc ) const;
    void                SetCurveType        ( CurveType type );
    CurveType           GetCurveType        ();

    virtual void        SetWrapPostMethod   ( WrapMethod method );
    virtual void        SetWrapPreMethod    ( WrapMethod method );
    virtual WrapMethod  GetWrapPostMethod   ();
    virtual WrapMethod  GetWrapPreMethod    ();

    virtual int         GetNumKeys          ();


    void                AppendTransform     ( const TransformF & transform );
    void                InsertTransform     ( unsigned int transformNum, const TransformF & transform );

    inline unsigned int NumTransforms       () const;

    inline  void        SetRotation         ( unsigned int transformNum, const glm::vec3 & rotAxis, float angle, TimeType t );
    inline  void        SetScale            ( unsigned int transformNum, const glm::vec3 & scale, TimeType t );
    inline  void        SetTranslation      ( unsigned int transformNum, const glm::vec3 & translation, TimeType t );
    inline  void        SetCenter           ( unsigned int transformNum, const glm::vec3 & center, TimeType t );

    inline  void        RemoveRotation      ( unsigned int transformNum, TimeType t );
    inline  void        RemoveScale         ( unsigned int transformNum, TimeType t );
    inline  void        RemoveTranslation   ( unsigned int transformNum, TimeType t );
    inline  void        RemoveCenter        ( unsigned int transformNum, TimeType t );

    inline  bool        MoveRotation        ( unsigned int transformNum, TimeType t, TimeType newTime );
    inline  bool        MoveScale           ( unsigned int transformNum, TimeType t, TimeType newTime );
    inline  bool        MoveTranslation     ( unsigned int transformNum, TimeType t, TimeType newTime );
    inline  bool        MoveCenter          ( unsigned int transformNum, TimeType t, TimeType newTime );

    inline  TransformF& Transform           ( unsigned int transformNum );

    inline  glm::mat4   Evaluate            ( unsigned int transformNum ) const;

    virtual VoidPtr     QueryParamTyped     () override;

    static  ModelParamType  Type        ()
    {
        return ModelParamType::MPT_TRANSFORM_VEC;
    }

};

} //model
} //bv

#include "CompositeTypedParameters.inl"
