#pragma once

#include <cassert>

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Mathematics/Transform/MatTransform.h"


namespace bv { namespace model {

// *******************************************
class ParamTransform : public AbstractModelParameter
{
private:

    TransformF m_transformModel;

public:

    explicit            ParamTransform  ( const std::string & name, const TransformF & transform, const ITimeEvaluator * evaluator = nullptr );

    inline  void        SetRotation     ( const glm::vec3 & rotAxis, float angle, TimeType t );
    inline  void        SetScale        ( const glm::vec3 & scale, TimeType t );
    inline  void        SetTranslation  ( const glm::vec3 & translation, TimeType t );

    inline  TransformF& Transform       ();

    inline  glm::mat4   Evaluate        ( TimeType t ) const;

    virtual void *      QueryParamTyped () override;

    const TransformF &  GetTransformF   () const
    {
        return m_transformModel;
    }

    static  ModelParamType  Type        ()
    {
        return ModelParamType::MPT_TRANSFORM;
    }

};

// *******************************************
class ParamTransformVec : public AbstractModelParameter
{
private:

    std::vector< TransformF >   m_transformModelVec;

public:

    explicit            ParamTransformVec   ( const std::string & name, const TransformF & transform, const ITimeEvaluator * evaluator = nullptr );
    explicit            ParamTransformVec   ( const std::string & name, const ITimeEvaluator * evaluator = nullptr );

    void                AppendTransform     ( const TransformF & transform );

    inline unsigned int NumTransforms       () const;

    inline  void        SetRotation         ( unsigned int transformNum, const glm::vec3 & rotAxis, float angle, TimeType t );
    inline  void        SetScale            ( unsigned int transformNum, const glm::vec3 & scale, TimeType t );
    inline  void        SetTranslation      ( unsigned int transformNum, const glm::vec3 & translation, TimeType t );

    inline  TransformF& Transform           ( unsigned int transformNum );

    inline  glm::mat4   Evaluate            ( unsigned int transformNum, TimeType t ) const;

    virtual void *      QueryParamTyped     () override;

    static  ModelParamType  Type        ()
    {
        return ModelParamType::MPT_TRANSFORM_VEC;
    }

};

} //model
} //bv

#include "CompositeTypedParameters.inl"
