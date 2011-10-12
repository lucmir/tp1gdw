#ifndef __CTREE_INL__
#define __CTREE_INL__

#include "ctree.h"
#include <functional>
#include <strstream>
#include <map>
#include <cassert>

//#define DEBUG
#include "debug.h"


template <class T, class tree_node_allocator>
edtl::ctree<T, tree_node_allocator>::ctree() : tree<T, tree_node_allocator>() {}

template <class T, class tree_node_allocator>
edtl::ctree<T, tree_node_allocator>::ctree(const iterator_base &b) : tree<T, tree_node_allocator>(b) 
{
	this->compile();
}

template <class T, class tree_node_allocator>
edtl::ctree<T, tree_node_allocator>::ctree(const tree<T, tree_node_allocator> &rhs) : tree<T, tree_node_allocator>(rhs)
{
	this->compile();
}

template <class T, class tree_node_allocator>
edtl::ctree<T, tree_node_allocator>::ctree(const ctree<T, tree_node_allocator> &rhs) : tree<T, tree_node_allocator>(rhs)
{
	this->compile();
}

template <class T, class tree_node_allocator>
void edtl::ctree<T, tree_node_allocator>::compile()
{
	
	using namespace std;

	DEBUGP("Compiling tree sized %u\n", this->size());
	std::map<tree_node_<T> *, uint> __idmap;
	
	DEBUGP("Resizing cdata\n");
	c_data_.resize(this->size());
	
	typename tree<T, tree_node_allocator>::pre_order_iterator it = this->begin();
	typename tree<T, tree_node_allocator>::pre_order_iterator end = this->end();

	DEBUGP("Walking preorder\n");
	uint id = 0;
	c_depth_ = 0;
	while (it != end) 
	{
		uint __depth = depth(it);
		c_data_[id].id_ = id;
		c_data_[id].depth_ = __depth;
		c_data_[id].data_ = &(*it);
		c_depth_ = c_depth_ < __depth ? __depth : c_depth_;//there is something wrong here
		__idmap.insert(make_pair(it.node, id));
		//c_data_[id].keyroot = true;//temporary keyroot - it will be fixed later-- Karane
		c_data_[id].node = it.node;// Karane -- 24/11/2008
		
		++it, ++id;
	}
	
	DEBUGP("Reserving postorder\n");
	post_order_walk_.clear();
	post_order_walk_.reserve(c_data_.size());
	
	DEBUGP("Walking postorder\n");
	//uint index = 0;
	typename tree<T, tree_node_allocator>::post_order_iterator pit = begin_post();
	typename tree<T, tree_node_allocator>::post_order_iterator pend = end_post();
	
	for (; pit != pend; ++pit)
	{
		assert(__idmap.find(pit.node) != __idmap.end());
		uint id = __idmap[pit.node];

		c_data_[id].size_ = 1;
		c_data_[id].children_.clear(); 
		//c_data_[id].children_.resize(this->number_of_children(pit));//--karane 30/03/2006

		typename tree<T, tree_node_allocator>::sibling_iterator sib = this->begin(pit);
		typename tree<T, tree_node_allocator>::sibling_iterator send = this->end(pit);
		for(; sib != send; ++sib)
		{
			assert(__idmap.find(sib.node) != __idmap.end());
			uint cid = __idmap[sib.node];
			c_data_[id].size_ += c_data_[cid].size_;
			c_data_[id].children_.push_back(cid);
			c_data_[cid].parent_ = id;
		}
		if (c_data_[id].children_.size() > c_max_degree_) c_max_degree_ = c_data_[id].children_.size();

		post_order_walk_.push_back(id);
	}
}

template <class T, class tree_node_allocator>
uint edtl::ctree<T, tree_node_allocator>::c_max_degree() const
{
	return this->c_max_degree_;
}

template <class T, class tree_node_allocator>
uint edtl::ctree<T, tree_node_allocator>::c_size(uint id) const
{
	assert(id < c_data_.size());
	return this->c_data_[id].size_;
}

template <class T, class tree_node_allocator>
uint edtl::ctree<T, tree_node_allocator>::c_depth(uint id) const
{
	assert(id < c_data_.size());
	return this->c_data_[id].depth_;
}

template <class T, class tree_node_allocator>
uint edtl::ctree<T, tree_node_allocator>::c_parent(uint id) const
{
	assert(id < c_data_.size());
	return this->c_data_[id].parent_;
}



template <class T, class tree_node_allocator>
uint edtl::ctree<T, tree_node_allocator>::c_depth() const
{
	return this->c_depth_;
}

/*
//Karane
template <class T, class tree_node_allocator>
bool edtl::ctree<T, tree_node_allocator>::c_isKeyRoot(uint id) const
{
	//return this->c_depth_;
	return this->c_data_[id].keyroot;
}

//Karane
template <class T, class tree_node_allocator>
void edtl::ctree<T, tree_node_allocator>::c_setKeyRoot(uint id)
{
	//return this->c_depth_;
	this->c_data_[id].keyroot = true;
}

//Karane
template <class T, class tree_node_allocator>
void edtl::ctree<T, tree_node_allocator>::c_resetKeyRoot(uint id)
{
	//return this->c_depth_;
	this->c_data_[id].keyroot = false;
}
*/

template <class T, class tree_node_allocator>
void edtl::ctree<T, tree_node_allocator>::c_prepairKeyRoots()
{
	this->c_data_[id].keyroot = false;
}

/*
Retorna os filhos do nodo id.
*/
template <class T, class tree_node_allocator>
const std::vector<uint>& edtl::ctree<T, tree_node_allocator>::c_children(uint id) const
{
	assert(id < c_data_.size());
	return this->c_data_[id].children_;
}

template <class T, class tree_node_allocator>
const std::vector<uint>& edtl::ctree<T, tree_node_allocator>::post_order_walk() const
{
	assert(post_order_walk_.size() == c_size());
	return this->post_order_walk_;
}

template <class T, class tree_node_allocator>
const T& edtl::ctree<T, tree_node_allocator>::operator[](uint id) const
{
	assert(id < c_data_.size());
	return *(this->c_data_[id].data_);
}

template <class T, class tree_node_allocator>
T& edtl::ctree<T, tree_node_allocator>::operator[](uint id) 
{
	return *(this->c_data_[id].data_);
}

template <class T, class tree_node_allocator>
tree_node_<T>* edtl::ctree<T, tree_node_allocator>::getTreeNode(uint id) 
{
	return (this->c_data_[id].node);
}

template <class T>
std::ostream &operator<<(std::ostream &stream, const edtl::ctree<T> &tr)
{
	stream << (std::string)(tr);	
	return stream;
}

template <class T, class tree_node_allocator>
edtl::ctree<T, tree_node_allocator>::operator std::string() const
{
	using namespace std;

	ostrstream ret;
	//ret << "-----" << endl;
	
	for(uint i = 0; i < this->c_size(); ++i)
	{

		DEBUGP("Depth is %u\n", c_depth(i));
		for (uint j = 0; j < c_depth(i); ++j) ret << "  ";
		//ret << i << ":";

		
		string name = (string)(this->operator[](i));
		
		if((this->operator[](i)).isTag())
		 ret << "<" << name << ">";
		 else ret << name;
		 
		ret << endl << flush;
	}
	//ret << "-----" << endl << ends;
	string str = ret.str();
	ret.freeze(0);
	return str;
}

#endif
