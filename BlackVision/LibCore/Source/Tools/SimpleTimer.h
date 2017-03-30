#pragma once

#include "CoreDEF.h"

#define QPF_TIMER

      
namespace bv
{

class SimpleTimer
{
private:

	mutable std::mutex	m_mutex;

    bool				m_paused;
    unsigned long		m_startMillis;

    unsigned long		m_startPause;
    unsigned long		m_totalPausedTime;

#ifdef QPF_TIMER
	UInt64				m_timerFrequency;
#endif
public:

                            SimpleTimer     ();
                            ~SimpleTimer    ();

    void                    Start           ();
	void					Reset			();

    void                    PauseOnAsync    ( UInt64 t );
    void                    Pause           ();
    void                    UnPause         ();

    inline TimeType         ElapsedTime     () const
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		return MillisToTime( ElapsedMillis() );
	}

	inline unsigned long    ElapsedMillis   () const
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		if( m_paused )
		{
			return m_startPause - m_startMillis - m_totalPausedTime;
		}

		return QueryMillis() - m_startMillis - m_totalPausedTime;
	}

    static unsigned long    GetTickCount    ();
    static unsigned long    TimeGetTime     ();

private:

	unsigned long           QueryMillis() const;

    inline TimeType         MillisToTime    ( unsigned long millis ) const
    {
        return TimeType( millis ) * TimeType( 0.001 );
    }

};

} //bv
