#pragma once

#include <hash_map>

#include "Memory/AVFrame.h"



namespace bv { 

   
class AVOutputsData
{
public:

    typedef unsigned int VideoCardID;

    typedef std::hash_map< VideoCardID, AVFramePtr >    AVFramePtrMap;

	typedef AVFramePtrMap::const_iterator				const_iterator;

private:

    mutable AVFramePtrMap   m_frames;

private:

    // Disable copying (when delete keyword is not present)
    AVOutputsData               ( const AVOutputsData & src );
    AVOutputsData               ( const AVOutputsData && src );

    AVOutputsData & operator =  ( const AVOutputsData & src );

public:


	const_iterator	begin()			const { return m_frames.begin(); }
	const_iterator	end()			const { return m_frames.end(); }

                    AVOutputsData   ();

    AVFramePtr      GetAVFrame      ( unsigned int videoOutputId ) const;
    void            SetAVFrame      ( unsigned int videoOutputId, AVFramePtr frame );



    unsigned int    GetNumEntries   () const;

};


} //bv

