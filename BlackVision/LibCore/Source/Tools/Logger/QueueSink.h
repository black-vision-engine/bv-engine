#include "DataTypes/QueueConcurrent.h"
#include "Logger.h"

#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sinks/frontend_requirements.hpp>



namespace bv
{

typedef boost::log::sinks::combine_requirements<boost::log::sinks::concurrent_feeding>::type frontend_requirements;

class QueueSink :   public boost::log::sinks::basic_sink_backend<frontend_requirements>
{
private:
    QueueConcurrent<LogMsg>&        m_queue;
public:
    QueueSink( QueueConcurrent<LogMsg>& queue )
        :   m_queue( queue )    {}

    void consume( boost::log::record_view const& rec );
};

} //bv
