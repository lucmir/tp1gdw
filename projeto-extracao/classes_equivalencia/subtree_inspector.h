#ifndef __SUBTREE_EQUIVALENCE_CLASS__
#define __SUBTREE_EQUIVALENCE_CLASS__

#include "ctree.h"
#include "my_hash_map.h"
#include <vector>
#include <strstream>

namespace edtl {

template <class T, class _HashFcn = MY_NS_HASH<T>, class _EqualKey = std::equal_to<T> >
class SubtreeInspector 
{
	public:

		void equivalenceClasses(const ctree<T> &src, const ctree<T> &dst, std::vector<uint> &rEquivClasses) const;

	protected:
		typedef MY_NS_HASH_MAP<T, std::vector<uint>, _HashFcn, _EqualKey> hash_type;

		void addTree(const ctree<T> &src, uint id_offset, hash_type &dag_hash, std::vector<uint> &G, std::vector<std::vector<uint> >& Garcs, std::vector<uint> &K) const;
};

} //namespace edtl

#include "subtree_inspector.inl"

#endif
