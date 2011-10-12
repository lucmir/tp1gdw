#ifndef __KSUBTREE_EQUIVALENCE_CLASS__
#define __KSUBTREE_EQUIVALENCE_CLASS__

#include "ctree.h"
#include "my_hash_map.h"
#include <vector>
#include <strstream>
#include <ext/hash_map>




namespace edtl {

struct compactPath
		{
		  uint parent_path_id;
		  std::string tag_name;
		};
		
struct path_subtree_pair
		{
			uint path_id;
			uint subtree_id;
		};

template <class T, class _HashFcn = MY_NS_HASH<T>, class _EqualKey = std::equal_to<T> >
class KSubtreeInspector 
{
	/*private:
		typedef MY_NS_HASH_MAP<T, std::vector<uint>, _HashFcn, _EqualKey> hash_type;

		std::vector<uint> K_acum;
		hash_type dag_hash;
		std::vector<uint> Graph;
		std::vector<uint> Graph_arcs;
		*/
	private:
		uint get_node_hash(T &n) const;
	public:
		typedef MY_NS_HASH_MAP<T, std::vector<uint>, _HashFcn, _EqualKey> hash_type;
		
		typedef MY_NS_HASH_MAP<uint, std::vector<uint>, MY_NS_HASH<uint>, std::equal_to<uint> > hash_sf_type;// Karane - 04/12/2008

		uint LAST_CLASS;
		void subtreeEquivClasses(ctree<T> &src, ctree<T> &dst, std::vector<uint> &rEquivClasses);
		void subtree_pathEquivClasses(ctree<T> &src, ctree<T> &dst, std::vector<uint> &rEquivClasses);
		
		//std::vector<uint>& get
			
	//protected:
	
		
		
		//typedef MY_NS_HASH_MAP<uint, uint, MY_NS_HASH<uint>, std::equal_to<uint> > path_hash_type;
		struct path_type
		{
			uint parent_num;
			std::string label;
			uint path_num;
		};
		
		typedef MY_NS_HASH_MAP<uint, std::vector<struct path_type>, MY_NS_HASH<uint>, std::equal_to<uint> > path_hash_type;
		

		//hash_map<unsigned int, vector<uint> > path_hash_type;
		
		//typedef MY_NS_HASH_MAP<uint, uint, MY_NS_HASH<uint>, std::equal_to<uint> > PSH_type;
		struct ps_type
		{
			uint p_num, k_num;
			uint ps_num;
		};
		typedef MY_NS_HASH_MAP<uint, std::vector<struct ps_type>, MY_NS_HASH<uint>, std::equal_to<uint> > PSH_type;
	
		//hash_map<unsigned int, vector<uint> > PSH_type;

		//void addTree(const ctree<T> &src, uint id_offset, hash_type &dag_hash, std::vector<uint> &G, std::vector<std::vector<uint> >& Garcs, std::vector<uint> &K) const;
		void addTree(ctree<T> &src, uint id_offset, hash_type &dag_hash, 
							std::vector<uint> &G, 
							std::vector<std::vector<uint> >& Garcs, 
							std::vector<uint> &K) const;

/*
		void addTree_sf(ctree<T> &src, 
							uint id_offset, 
							hash_type &dag_hash, 
							std::vector<uint> &G, 
							std::vector<std::vector<uint> >& Garcs, 
							std::vector<uint> &K) const;

		void addTree_sf2(ctree<T> &src, 
							uint id_offset, 
							hash_sf_type &dag_hash, 
							std::vector<uint> &G, 
							std::vector<std::vector<uint> >& Garcs, 
							std::vector<uint> &K) const ;
*/
		
		void addTreePath(ctree<T> &src, uint id_offset, path_hash_type &path_hash,
							PSH_type &PS_hash,
							uint *current_path_id,  
							uint *current_ps_id,  
							std::vector<uint> &K,
							std::vector<uint> &P,
							std::vector<uint> &PS) const;

		void path_equivClasses(ctree<T> &src,	std::vector<uint> &rEquivClasses);
		
		


};

} //namespace edtl

#include "ksubtree_inspector.inl"

#endif
