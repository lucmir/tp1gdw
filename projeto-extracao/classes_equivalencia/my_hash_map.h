#ifndef __MY_HASH_MAP_H__
#define __MY_HASH_MAP_H__

#if defined(HAVE___GNU_CXX__EXT_HASH_MAP)
	#include <ext/hash_map>
	#define MY_NS_HASH_MAP __gnu_cxx::hash_map
	#define MY_HASH_MAP hash_map
	#define MY_NS_HASH __gnu_cxx::hash
	#define MY_HASH hash
#elif defined(HAVE_STD__HASH_MAP)
	#include <hash_map>
	#define MY_HASH_MAP hash_map
	#define MY_NS_HASH_MAP std::hash_map
	#define MY_HASH hash
	#define MY_NS_HASH std::hash
#else
	#define HASH_MAP UNAVAILABLE
	#warning "No usable hash map configured. Code will not compile."
#endif

#endif //__MY_HASH_MAP_H__
