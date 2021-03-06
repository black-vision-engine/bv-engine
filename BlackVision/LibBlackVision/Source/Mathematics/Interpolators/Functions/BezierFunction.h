#pragma once

#include "IEvaluator.h"

#include "Serialization/BV/BVDeserializeContext.h"
#include "Exceptions/Serialization/SerializationException.h"

namespace bv
{



// *******************************
//
template< class TimeValueT, class ValueT >
class BezierEvaluator : public IEvaluator< TimeValueT, ValueT >
{
    typedef Key< TimeValueT, ValueT > Key;
    FRIEND_TEST_ACCESSOR( BezierEvaluator )
public: // FIXME
    
    Key key1, key2;
    Key v1, v2;
    TimeValueT  m_tolerance;
    CurveType   m_curveType;

public:
    virtual EvaluatorType           GetType         () const override { return EvaluatorType::ET_BEZIER; }
    virtual CurveType               GetCurveType    () const override { return m_curveType; }

    BezierEvaluator( CurveType curve, Key k1, Key k2, Key v1_, Key v2_, TimeValueT tolerance ) : m_curveType( curve ), key1( k1 ), key2( k2 ), v1( v1_ ), v2( v2_ ), m_tolerance( tolerance ) {}
    
    // ***********************
    //
    void SetV2( Key v2 ) { this->v2 = v2; }
    
    // ***********************
    //
    virtual void                        SetValue        ( TimeValueT t, ValueT v ) override
    {
        if( key1.t == t )
            key1.val = v;
        else if( key2.t == t )
            key2.val = v;
        else
            assert( false );
    }

    // ***********************
    //
    ValueT                              Evaluate        ( TimeValueT t ) const override 
    {
        assert( key1.t <= t && t <= key2.t );

        Key A = key1,
            B = key1 + v1,
            C = key2 + v2,
            D = key2;

        for( ; ; ) 
        {
            Key middle = 1.f/8 * A + 3.f/8 * B + 3.f/8 * C + 1.f/8 * D;
            
            if( fabs( middle.t - t ) < m_tolerance )
                return middle.val;
            else
            {
                if( t < middle.t )
                {
                    A = A;
                    B = 1.f/2 * A + 1.f/2 * B;
                    C = 1.f/4 * A + 1.f/2 * B + 1.f/4 * C;
                    D = middle;
                }
                else
                {
                    A = middle;
                    B = 1.f/4 * B + 1.f/2 * C + 1.f/4 * D;
                    C = 1.f/2 * C + 1.f/2 * D;
                    D = D;               
                }
            }
        }
    }

    // ***********************
    //
    virtual void                        Serialize       ( ISerializer & ser ) const override
    {
        ser.EnterChild( "interpolation" );
            ser.SetAttribute( "type", Convert::T2String( m_curveType ) );
        
            ser.EnterChild( "v1" );
                SerializationHelper::SerializeAttribute( ser, v1.t, "dt" );
                SerializationHelper::SerializeAttribute( ser, v1.val, "dval" );
            ser.ExitChild();
        
            ser.EnterChild( "v2" );
                SerializationHelper::SerializeAttribute( ser, v2.t, "dt" );
                SerializationHelper::SerializeAttribute( ser, v2.val, "dval" );
            ser.ExitChild();
        
        ser.ExitChild();
    }

    // ***********************
    // Note: if there's only one error in xml, we use default values.
    virtual void                        Deserialize     ( const IDeserializer & deser )
    {
        if( ValidateCurveType( deser, m_curveType ) )
        {
            bool somethingFailed = false;

            Expected< TimeValueT > t1;
            Expected< TimeValueT > t2;
            Expected< ValueT > val1;
            Expected< ValueT > val2;

            if( deser.EnterChild( "v1" ) )
            {
                t1 = Convert::String2T< TimeValueT >( deser.GetAttribute( "dt" ) );
                val1 = Convert::String2T< ValueT >( deser.GetAttribute( "dval" ) );

                if( !t1.IsValid() )     { somethingFailed = true; Warn< SerializationException >( deser, "Invalid dt1." ); }
                if( !val1.IsValid() )   { somethingFailed = true; Warn< SerializationException >( deser, "Invalid dval1." ); }

                deser.ExitChild();
            }
            else
            {
                Warn< SerializationException >( deser, "No v1 marker found. Bezier will use default values." );
                somethingFailed = true;
            }


            if( deser.EnterChild( "v2" ) )
            {
                t2 = Convert::String2T< TimeValueT >( deser.GetAttribute( "dt" ) );
                val2 = Convert::String2T< ValueT >( deser.GetAttribute( "dval" ) );

                if( !t2.IsValid() )     { somethingFailed = true; Warn< SerializationException >( deser, "Invalid dt2." ); }
                if( !val2.IsValid() )   { somethingFailed = true; Warn< SerializationException >( deser, "Invalid dval2." ); }

                deser.ExitChild();
            }
            else
            {
                Warn< SerializationException >( deser, "No v2 marker found. Bezier will use default values." );
                somethingFailed = true;
            }


            if( !somethingFailed )
            {
                v1.t = t1.GetVal();
                v2.t = t2.GetVal();

                v1.val = val1.GetVal();
                v2.val = val2.GetVal();
            }
        }
    }
};

}
