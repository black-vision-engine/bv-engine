#include "DataTypes/QueueConcurrentLimited.h"

#include <iostream>
#include <thread>



#define MaxElementsInQueueu 7


struct Food
{
    int             Dinner;
    int             Tomatoes;
    int             Wine;
    std::string     Message;
    std::thread::id ID;
};


bv::QueueConcurrentLimited< Food > limitedQueue( MaxElementsInQueueu );


void        Produce( unsigned int timeout )
{
	typedef std::chrono::high_resolution_clock Time;

	auto start_time = Time::now();

    while( std::chrono::duration_cast<std::chrono::milliseconds>( Time::now() - start_time ).count() < timeout )
    {
        Food food;
        food.ID = std::this_thread::get_id();
        food.Message = "Producer Id: ";

        limitedQueue.WaitAndPush( std::move( food ) );

        std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
    }
	std::cout << "daying " << std::chrono::duration_cast<std::chrono::milliseconds>( Time::now() - start_time ).count() << std::endl;
}


void        Consume()
{
    while( true )
    {
        Food food;
		if( !limitedQueue.WaitAndPop( food ) )
		{
			break;
		}

        std::cout << food.Message << food.ID << std::endl;
    }
}



int main()
{
    const int numConsumers = 3;
    const int numProducers = 10;


    std::thread consumers[ numConsumers ];
    std::thread producers[ numProducers ];
    // spawn 10 threads:
    for( int i = 0; i < numProducers; ++i )
        producers[ i ] = std::thread( Produce, 1000 * ( i + 1 ) );

    for( int i = 0; i < numConsumers; ++i )
        consumers[ i ] = std::thread( Consume );

    for( auto& th : producers ) th.join();

	limitedQueue.EnqueueEndMessage();

	for( auto& th : consumers ) th.join();

}


