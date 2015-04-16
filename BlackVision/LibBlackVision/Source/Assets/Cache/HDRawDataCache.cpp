#include "HDRawDataCache.h"

#include "IO/FileIO.h"

static const std::string RAW_DATA_CACHE_DIR = "cache/raw_data_cache";

namespace bv {

// ******************************
// 
void		DHRawDataCache::Initialize		()
{
	ScanCacheDir();
}

// ******************************
// 
void		DHRawDataCache::ScanCacheDir	()
{
	if( !File::DirExists( RAW_DATA_CACHE_DIR ) )
	{
		File::CreateDir( RAW_DATA_CACHE_DIR );
	}
	else
	{
		auto files = File::ListFileInDir( RAW_DATA_CACHE_DIR );

		for( auto f : files )
		{
			m_entries.insert( Hash::FromString( f ) );
		}
	}
}

// ******************************
// 
MemoryChunkConstPtr	DHRawDataCache::Load	( const Hash & key ) const
{
	auto fileName = RAW_DATA_CACHE_DIR + key.Get();

	if( File::Exists( fileName ) )
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
DHRawDataCache::DHRawDataCache				()
{
	Initialize();
}

// ******************************
// 
DHRawDataCache::~DHRawDataCache				()
{}

// ******************************
// 
DHRawDataCache & DHRawDataCache::GetInstance()
{
	static DHRawDataCache instance = DHRawDataCache();
	return instance;
}

// ******************************
// 
MemoryChunkConstPtr	DHRawDataCache::Get		( const Hash & key ) const
{
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
bool DHRawDataCache::Add					( const Hash & key, const MemoryChunkConstPtr & memory, bool rewriteIfExists )
{
	auto fileName = RAW_DATA_CACHE_DIR + key.Get();

	auto exists = File::Exists( fileName );

	if( !exists || rewriteIfExists )
	{
		File::Write( memory->Get(), memory->Size(), fileName, false );

		return true;
	}
	else
	{
		return false;
	}
}

} // bv