#include "stdafx.h"

#include "HardDriveRawDataCache.h"

#include "System/Path.h"
#include "IO/FileIO.h"
#include "IO/DirIO.h"


static const std::string RAW_DATA_CACHE_DIR = "cache/raw_data_cache/";




#include "Memory/MemoryLeaks.h"



namespace bv {

// ******************************
// 
void		HardDriveRawDataCache::Initialize		()
{
	ScanCacheDir();
}

// ******************************
// 
void		HardDriveRawDataCache::ScanCacheDir	()
{
	if( !Dir::Exists( RAW_DATA_CACHE_DIR ) )
	{
		Dir::CreateDir( RAW_DATA_CACHE_DIR, true );
	}
	else
	{
		auto files = Dir::ListFiles( RAW_DATA_CACHE_DIR, "*" );

		for( auto f : files )
		{
			auto s = File::GetFileName( f );
			m_entries.insert( Hash( s ) );
		}
	}
}

// ******************************
// 
MemoryChunkConstPtr	HardDriveRawDataCache::Load	( const Hash & key ) const
{
	auto fileName = RAW_DATA_CACHE_DIR + key.Get();

	if( Path::Exists( fileName ) )
	{
		auto size	= File::Size( fileName );

		auto memoryChunk = MemoryChunk::Create( size );

		File::Read( memoryChunk->GetWritable(), fileName );

		return memoryChunk;
	}
	else
	{
		return nullptr;
	}
}

// ******************************
// 
HardDriveRawDataCache::HardDriveRawDataCache				()
{
	Initialize();
}

// ******************************
// 
HardDriveRawDataCache::~HardDriveRawDataCache				()
{}


// ******************************
// 
MemoryChunkConstPtr	HardDriveRawDataCache::Get		( const Hash & key ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

	auto it = m_entries.find( key );
	if( it != m_entries.end() )
	{
		return Load( key );
	}
	else
	{
		return nullptr;
	}
}

// ******************************
// 
bool HardDriveRawDataCache::Exists					( const Hash & key ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

	return m_entries.find( key ) != m_entries.end();
}

// ******************************
// 
bool HardDriveRawDataCache::Add					( const Hash & key, const MemoryChunkConstPtr & memory, bool rewriteIfExists )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

	auto fileName = RAW_DATA_CACHE_DIR + key.Get();

	auto exists = Path::Exists( fileName );

	if( !exists || rewriteIfExists )
	{
		File::Write( memory->Get(), memory->Size(), fileName, false );

        m_entries.insert( key );

		return true;
	}
	else
	{
		return false;
	}
}

} // bv