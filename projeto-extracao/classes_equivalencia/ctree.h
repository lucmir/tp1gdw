#ifndef __CTREE_H__
#define __CTREE_H__

#include <htmlcxx/html/tree.h>
#include <iostream>
#include <vector>

namespace edtl 
{

template <class T, class tree_node_allocator = std::allocator<tree_node_<T> > >
class ctree : public tree<T, tree_node_allocator> 
{
	public:
		ctree();
		ctree(const iterator_base &b);
		ctree(const tree<T, tree_node_allocator> &rhs);
		ctree(const ctree<T, tree_node_allocator> &rhs);
		virtual ~ctree() {}
		virtual void compile();
		inline uint c_max_degree() const;
		inline uint c_size(uint id = 0) const;
		inline uint c_depth(uint id) const;
		inline uint c_parent(uint id) const;
		inline uint c_depth() const;
		inline const std::vector<uint>& c_children(uint id) const;
		inline T& operator[](uint id);
		inline const T& operator[](uint id) const;

		const std::vector<uint> &post_order_walk() const;
		
		//Karane -- Variavel
		//std::map<tree<T, tree_node_allocator>, uint>() __id_map;
		//std::map<tree_node_<T> *, uint> __idmap;
		
		//Karane
		//inline bool c_isKeyRoot(uint id) const;
		//inline void c_setKeyRoot(uint id);
		//inline void c_resetKeyRoot(uint id);
		
		void c_prepairKeyRoots();
		//void printNodes(std::vector<int> nodes, typename std::ostrstream &ret);
		//void printNodes(std::vector<uint> nodes, typename std::ostrstream &ret);
		tree_node_<T> *getTreeNode(uint id); //Karane -- 24/11/2008

		operator std::string() const;
	protected:
		uint c_max_degree_;
		uint c_depth_;
		class c_data_type
		{
			//private:
				
			public:
			uint size_;
			uint depth_;
			uint parent_;
			std::vector<uint> children_;
			uint id_;
			T *data_;
			tree_node_<T> *node;
	
		};
		std::vector<c_data_type> c_data_;
		std::vector<uint> post_order_walk_;
	private:
		
};


} //namespace edtl
template <class T>
std::ostream &operator<<(std::ostream &stream, const edtl::ctree<T> &);
#include "ctree.inl"

#endif
