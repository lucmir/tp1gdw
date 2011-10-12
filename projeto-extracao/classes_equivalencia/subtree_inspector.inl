#ifndef __SUBTREE_EQUIVALENCE_CLASS_INL__
#define __SUBTREE_EQUIVALENCE_CLASS_INL__

#include <queue>
#include "my_hash_map.h"
#include "subtree_inspector.h"

//#define DEBUG
#include "debug.h"

/** This is a quite hard algorithm. 
  * Please refer to "An Efficient Bottom-Up distance Algorithm Between Trees",
  * by Gabriel Valiente, for a similar approach  */

template <class T, class _HashFcn, class _EqualKey>
void edtl::SubtreeInspector<T, _HashFcn, _EqualKey>::equivalenceClasses(const ctree<T> &src, const ctree<T> &dst, std::vector<uint> &rEquivClasses) const
{

	std::vector<uint> &K(rEquivClasses);

	using namespace std;
	using namespace edtl;

	//Hash to associate each label to series of ids
	hash_type dag_hash; //the dag name comes from directed acyclic graph

	//Associate each node to a class
	K.resize(src.c_size() + dst.c_size());
	DEBUGP("K is sized %u\n", K.size());

	vector<uint> G;
	vector<vector<uint> > Garcs;

	DEBUGP("Adding source tree sized %u\n", src.c_size());
	addTree(src, 0, dag_hash, G, Garcs, K);
	DEBUGP("Adding destination tree sized %u\n", dst.c_size());
	addTree(dst, src.c_size(), dag_hash, G, Garcs, K);

	#ifdef DEBUG
	cerr << "Source K is: " << endl;
	for(uint i = 0; i < src.c_size(); ++i) cerr << K[i] << " ";
	cerr << endl;
	cerr << "Destination K is: " << endl;
	for(uint i = 0; i < dst.c_size(); ++i) cerr << K[src.c_size() + i] << " ";
	cerr << endl;

	cerr << "Identical subtrees are:" << endl;
	for (uint sit = 0; sit < src.c_size(); ++sit)
	{
		for (uint dit = 0; dit < dst.c_size(); ++dit)
		{
			assert(K.size() > src.c_size() + dit);
			if (K[sit] == K[src.c_size() + dit])
			{
				cerr << sit << " == " << dit << " class " << K[sit] << endl;
				assert(src.c_size(sit) == dst.c_size(dit));
				assert(src.c_children(sit).size() == dst.c_children(dit).size());
					//assert((T)(*sit) == (T)(*dit));
			}
		}
	}
	#endif
}

template <class T, class _HashFcn, class _EqualKey>
void edtl::SubtreeInspector<T, _HashFcn, _EqualKey>::addTree(const ctree<T> &src, uint id_offset, hash_type &dag_hash, std::vector<uint> &G, std::vector<std::vector<uint> >& Garcs, std::vector<uint> &K) const 
{

	using namespace std;

	std::vector<uint>::const_iterator sit = src.post_order_walk().begin();
	std::vector<uint>::const_iterator send = src.post_order_walk().end();

	for (; sit != send; ++sit)
	{
		DEBUGP("Checking node %u (%u)\n", *sit, *sit + id_offset);
		typename hash_type::iterator dag_it;
		dag_it = dag_hash.find(src[*sit]);
		bool found = false;
		if (dag_it != dag_hash.end()) 
		{
			DEBUGP("Found hash_entry with %u candidates\n", dag_it->second.size());
			//Look if children match
			vector<uint>::const_iterator vit = dag_it->second.begin();
			vector<uint>::const_iterator vend = dag_it->second.end();
			assert(vit != vend);
			for (;vit != vend; ++vit) 
			{
				assert(Garcs.size() > *vit);
				uint narcs = Garcs[*vit].size();	
				if (narcs != src.c_children(*sit).size()) continue;

				const std::vector<uint> &children(src.c_children(*sit));
				found = true;
				//compara se os nodos filhos são iguais
				for (uint j = 0; j < children.size() && j < narcs; ++j) 
				{
					assert(K.size() > children[j] + id_offset);
					if (K[children[j] + id_offset] != Garcs[*vit][j]) 
					{
						DEBUGP("Children mismatch\n");
						found = false;
						break;	
					}
				}
				if (found) break;
			}
			if (found) 
			{
				DEBUGP("Matched! K is sized %u sit->pid is %u and id_offset is %u\n", K.size(), *sit, id_offset);
				//If they match, add entry to K
				assert(K.size() > *sit + id_offset);
				assert(vit != vend);
				K[*sit + id_offset] = *vit;
				DEBUGP("Added entry %u->%u\n", *sit, *vit);
			}
		} 
		else 
		{
			DEBUGP("Creating new hash entry\n");
			dag_it = dag_hash.insert(make_pair(src[*sit], vector<uint>())).first; 
		}
		assert(dag_it != dag_hash.end());
		if (!found) 
		{
			//Add a new entry to graph
			DEBUGP("Adding new entry\n");
			G.push_back(*sit);
			Garcs.push_back(vector<uint>());
			dag_it->second.push_back(G.size() - 1);

			const std::vector<uint> &children(src.c_children(*sit));
			for (uint j = 0; j < children.size(); ++j) 
			{
				assert(K.size() > children[j] + id_offset);
				Garcs[G.size() - 1].push_back(K[children[j] + id_offset]);
			}
			K[*sit + id_offset] = G.size() - 1;
			DEBUGP("Added new entry %u->%u\n", *sit, G.size() - 1);
		}
	}
}

#endif
