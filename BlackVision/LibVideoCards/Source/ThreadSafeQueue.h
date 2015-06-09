#pragma once

#include <tchar.h>

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <process.h>
#include "win_sock.h"
#include <memory>
#include <boost\circular_buffer.hpp>
namespace bv
{

namespace videocards{
template <class T, std::size_t N>
class ThreadSafeQueue {

public:
	typedef T value_type;
	boost::circular_buffer<value_type>	q_;

	ThreadSafeQueue() : q_(N){}

	void push(value_type data)
	{
		std::unique_lock<std::mutex> lk(mtx_);
        //std::cerr<< "WAITING  on  push \n";
		cv_pop_.wait(lk, [=]{ return !q_.full(); });
        //std::cerr<< "Pushing \n";
		q_.push_back(data);
		cv_push_.notify_one();
	}

	void pushInnerVector(value_type data)
	{
		std::unique_lock<std::mutex> lk(mtx_);
		cv_pop_.wait(lk, [=]{ return !q_.full() });
		q_.push_back(data);
		cv_push_.notify_one();
	}

	value_type pop()
	{
		std::unique_lock<std::mutex> lk(mtx_);
        //std::cerr<< "WAITING  on  pop \n";
		cv_push_.wait(lk, [=]{ return !q_.empty(); });
		value_type result = q_.front();
        //std::cerr<< "Poping \n";
		q_.pop_front();
		cv_pop_.notify_one();
		return result;
	}

	void popInnerVector()
	{
		std::unique_lock<std::mutex> lk(mtx_);
        //std::cerr<< "WAITING  on  pop \n";
		cv_push_.wait(lk, [=]{ return !q_.empty(); });
        //std::cerr<< "Poping \n";
		q_.pop_front();
		cv_pop_.notify_one();
	}

	value_type getLast()
	{
		std::unique_lock<std::mutex> lk(mtx_);
		cv_push_.wait(lk, [=]{ return !q_.empty(); }); 
		cv_push_.notify_one();       
		return q_.front();
	}

	void clear()
	{
		std::unique_lock<std::mutex> lk(mtx_);
		while (!q_.empty())q_.pop_back();
		cv_pop_.notify_one();    
		//cv_pop_.notify_all();
	}


    int getSize()
    {
        return q_.size();
    }

private:
	std::mutex							mtx_;
	std::condition_variable				cv_push_;
	std::condition_variable				cv_pop_;
};
}
}