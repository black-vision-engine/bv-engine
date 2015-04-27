        template<>
        inline void                             AddSimpleParam< float > ( std::string name, const float& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< FloatInterpolator, float, ModelParamType::MPT_FLOAT, ParamType::PT_FLOAT1, ParamFloat >
                ( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< int > ( std::string name, const int& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< IntInterpolator, int, ModelParamType::MPT_INT, ParamType::PT_INT, ParamInt >
                ( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< glm::vec3 > ( std::string name, const glm::vec3& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3, ParamType::PT_FLOAT3, ParamVec3 >
                ( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< glm::vec2 > ( std::string name, const glm::vec2& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< Vec2Interpolator, glm::vec2, ModelParamType::MPT_VEC2, ParamType::PT_FLOAT2, ParamVec2 >
                ( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

        template<>
        inline void                             AddSimpleParam< glm::vec4 > ( std::string name, const glm::vec4& defaultValue, bool addValue, bool isState ) const
        {
            AddParam< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4, ParamType::PT_FLOAT4, ParamVec4 >
                ( m_lastParamValModel, m_lastTimeEvaluator, name, defaultValue, addValue, isState );
        }

