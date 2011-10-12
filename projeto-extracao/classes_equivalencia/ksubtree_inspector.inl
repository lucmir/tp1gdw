#ifndef __KSUBTREE_EQUIVALENCE_CLASS_INL__
#define __KSUBTREE_EQUIVALENCE_CLASS_INL__

#include <list>
#include "my_hash_map.h"
//#include "subtree_inspector.h"

//#define DEBUG
#include "debug.h"
//#define KDEBUG 1

#define NONE 0
#define NUMERIC 1
#define ALFA_NUMERIC 2
#define LITERAL 3
#define CAPITAL 4

/** This is a quite hard algorithm. 
  * Please refer to "An Efficient Bottom-Up distance Algorithm Between Trees",
  * by Gabriel Valiente, for a similar approach  */

template <class T, class _HashFcn, class _EqualKey>
void edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::subtreeEquivClasses(ctree<T> &src, 
ctree<T> &dst, std::vector<uint> &rEquivClasses)
{

	std::vector<uint> &K(rEquivClasses);

	using namespace std;
	using namespace edtl;
	using namespace __gnu_cxx;

	char auxstr[100];
	string aux;
	
	
	hash_type dag_hash;
	//Associate each node to a class
	//std::vector<uint> K;
	K.clear();
	K.resize(src.c_size() + dst.c_size()+1);
	DEBUGP("K is sized %u\n", K.size());

	vector<uint> G;
	vector<vector<uint> > Garcs;

	DEBUGP("Adding source tree sized %u\n", src.c_size());
	addTree(src, 0, dag_hash,  G, Garcs, K);
	DEBUGP("Adding destination tree sized %u\n", dst.c_size());
	addTree(dst, src.c_size(), dag_hash, G, Garcs, K);
	this->LAST_CLASS = G.size() - 1;
	//Hash to associante each compactPath to and integer
	
	
	//cout << "Lista" << endl;
	//for(uint it = 0; it < K.size();it++)
	//	printf("K[%d]= %d\n",it, K[it]);
        
	
	/*
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
	*/
	
	
}

template <class T, class _HashFcn, class _EqualKey>
uint edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::get_node_hash(T &n) const {
				
		const char *__s = n.tagName().c_str();
		unsigned long __h = 0;
		
		if(! n.isTag()  && !n.isComment())
		{
			__h = (uint)n.getType();
			return __h;
		}

		for ( ; *__s; ++__s)
			__h = 5*__h + *__s;

		
		//if(n.tagName() == "div")
		//{
			//n.parseAttributes();
			/*
			if(n.attribute("class").first)
			{
				__s = n.attribute("class").second.c_str();
			}
			*/
		//}
		
		for ( ; *__s; ++__s)
			__h = 5*__h + *__s;

		DEBUGP("Got hash value %u\n", __h);
		return __h;
	}

template <class T, class _HashFcn, class _EqualKey>
void edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::subtree_pathEquivClasses(ctree<T> &src, 
ctree<T> &dst, 
std::vector<uint> &rEquivClasses)
{

	std::vector<uint> &PS(rEquivClasses);

	using namespace std;
	using namespace edtl;
	using namespace __gnu_cxx;

	char auxstr[100];
	string aux;
	
	path_hash_type path_hash;
	std::vector<uint> P;//paths
	P.clear();
	P.resize(src.c_size() + dst.c_size());
	P[0] = 0;
	compactPath first_hash_node;
	first_hash_node.parent_path_id = 0;
	first_hash_node.tag_name = " ";
	
	sprintf(auxstr, "%d, %s", first_hash_node.parent_path_id, first_hash_node.tag_name.c_str());
	aux = auxstr;
	
	uint hashkey = (unsigned int) hash< const char* >()( aux.c_str() );

	//path_hash.insert(make_pair(aux, 0));
	path_type path_item;
	path_item.path_num = 0;
	path_item.label = " ";
	path_item.parent_num = 0;

	path_hash_type::iterator path_hash_it = path_hash.insert(make_pair(hashkey, std::vector<struct path_type>())).first;
	path_hash_it->second.push_back(path_item);
	//path_hash[hashkey] = path_item;
	uint current_path_id = 0;
	//uint current_ps_id = 0;
	
	//Hash to associate each label to series of ids
	hash_type dag_hash; //the dag name comes from directed acyclic graph

	//Associate each node to a class
	std::vector<uint> K;
	K.resize(src.c_size() + dst.c_size()+1);
	DEBUGP("K is sized %u\n", K.size());

	vector<uint> G;
	vector<vector<uint> > Garcs;

	DEBUGP("Adding source tree sized %u\n", src.c_size());
	addTree(src, 0, dag_hash,  G, Garcs, K);
	DEBUGP("Adding destination tree sized %u\n", dst.c_size());
	addTree(dst, src.c_size(), dag_hash, G, Garcs, K);
	
	//Hash to associante each compactPath to and integer
	
	
	PSH_type PS_hash;
	//std::vector<uint> PS;//paths
	PS.resize(src.c_size() + dst.c_size());
	uint current_ps_id = 0;
	addTreePath(src, 0, path_hash, PS_hash, &current_path_id, &current_ps_id, K, P, PS);
	addTreePath(dst, src.c_size(), path_hash, PS_hash, &current_path_id, &current_ps_id, K, P, PS);
	LAST_CLASS = current_ps_id;
	
/*
	cout << "Lista1" << endl;
	uint it;
	for(it = 0; it < src.c_size();it++)
		printf("%s PS[%d]= %d\n",src[it].text().c_str(), it, PS[it]);
        cout << "Lista2" <<endl;
	for(it = 0; it < PS.size();it++)
		printf("%s PS[%d]= %d\n",dst[it].text().c_str(), it, PS[it+src.c_size()]);
*/
	/*
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
	*/
	
	
}

/////////////
template <class T, class _HashFcn, class _EqualKey>
void edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::path_equivClasses(ctree<T> &src, 
std::vector<uint> &rEquivClasses)
{

	std::vector<uint> &PS(rEquivClasses);

	using namespace std;
	using namespace edtl;
	using namespace __gnu_cxx;

	char auxstr[100];
	string aux;
	
	path_hash_type path_hash;
	std::vector<uint> P;//paths
	P.clear();
	P.resize(src.c_size());
	P[0] = 0;
	compactPath first_hash_node;
	first_hash_node.parent_path_id = 0;
	first_hash_node.tag_name = " ";
	
	sprintf(auxstr, "%d, %s", first_hash_node.parent_path_id, first_hash_node.tag_name.c_str());
	aux = auxstr;
	
	uint hashkey = (unsigned int) hash< const char* >()( aux.c_str() );

	//path_hash.insert(make_pair(aux, 0));
	path_type path_item;
	path_item.path_num = 0;
	path_item.label = " ";
	path_item.parent_num = 0;

	path_hash_type::iterator path_hash_it = path_hash.insert(make_pair(hashkey, std::vector<struct path_type>())).first;
	path_hash_it->second.push_back(path_item);
	//path_hash[hashkey] = path_item;
	uint current_path_id = 0;
	//uint current_ps_id = 0;
	
	//Hash to associate each label to series of ids
	hash_type dag_hash; //the dag name comes from directed acyclic graph

	//Associate each node to a class
	std::vector<uint> K;
	K.resize(src.c_size() +1);
	DEBUGP("K is sized %u\n", K.size());

	vector<uint> G;
	vector<vector<uint> > Garcs;

	DEBUGP("Adding source tree sized %u\n", src.c_size());
	addTree(src, 0, dag_hash,  G, Garcs, K);
	//DEBUGP("Adding destination tree sized %u\n", dst.c_size());
	//addTree(dst, src.c_size(), dag_hash, G, Garcs, K);
	
	//Hash to associante each compactPath to and integer
	
	
	PSH_type PS_hash;
	//std::vector<uint> PS;//paths
	PS.resize(src.c_size());
	uint current_ps_id = 0;
	addTreePath(src, 0, path_hash, PS_hash, &current_path_id, &current_ps_id, K, P, PS);
	//addTreePath(dst, src.c_size(), path_hash, PS_hash, &current_path_id, &current_ps_id, K, P, PS);
	LAST_CLASS = current_ps_id;
	
/*
	cout << "Lista1" << endl;
	uint it;
	for(it = 0; it < src.c_size();it++)
		printf("%s PS[%d]= %d\n",src[it].text().c_str(), it, PS[it]);
        cout << "Lista2" <<endl;
	for(it = 0; it < PS.size();it++)
		printf("%s PS[%d]= %d\n",dst[it].text().c_str(), it, PS[it+src.c_size()]);
*/
	/*
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
	*/
	
	
}

////////////



template <class T, class _HashFcn, class _EqualKey>
void edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::addTree(ctree<T> &src, 
							uint id_offset, 
							hash_type &dag_hash, 
							std::vector<uint> &G, 
							std::vector<std::vector<uint> >& Garcs, 
							std::vector<uint> &K) const 
{

	using namespace std;
        using namespace __gnu_cxx;                                                 
	
	
	//============ Determina as classes de equival�ncia finais K ====================
	std::vector<uint>::const_iterator sit = src.post_order_walk().begin();
	std::vector<uint>::const_iterator send = src.post_order_walk().end();
	
	//std::list<uint> fila;
	std::vector<uint> fila;
	
	vector<int> children_control;//verificar se podemos repassar a capacidade por aqui.
	children_control.resize(src.c_size());
	
	uint parent;
	//uint last_node = src.size() -1;
	//Captura as folhas de src tree
	for (; sit != send; ++sit)
	{
		children_control[*sit] = src.c_children(*sit).size();
		if( children_control[*sit] == 0)
		{
			      fila.push_back(*sit);
		}
		
	}	

	//for (; sit != send; ++sit)
	uint node_id;
	int fila_it = 0, fila_size = fila.size();
	
	//while(! fila.empty() )
	while(fila_it < fila_size )
	{
		node_id = fila[fila_it];
		//node_id = fila.front();
		//fila.pop_front();
		
		DEBUGP("Checking node %u (%u)\n", node_id, node_id + id_offset);
		typename hash_type::iterator dag_it;
		//pode mudar a funcao de comparacao aqui!!
		
		if(src[node_id].tagName()=="img" || src[node_id].tagName()=="a")
		{
			src[node_id].parseAttributes();
		}
		
		
		dag_it = dag_hash.find(src[node_id]);
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
				if (narcs != src.c_children(node_id).size()) continue;

				const std::vector<uint> &children(src.c_children(node_id));
				found = true;
				//compara se os nodos filhos s�o iguais
				for (uint j = 0; j < children.size() && j < narcs; ++j) 
				{
					assert(K.size() > children[j] + id_offset);
          if (src.c_children(children[j] + id_offset).size() == 0) {
            if (!src[children[j] + id_offset].isTag())
              continue; 
          }
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
				DEBUGP("Matched! K is sized %u sit->pid is %u and id_offset is %u\n", K.size(), node_id, id_offset);
				//If they match, add entry to K
				assert(K.size() > node_id + id_offset);
				assert(vit != vend);
				K[node_id + id_offset] = *vit;
				DEBUGP("Added entry %u->%u\n", node_id, *vit);
			}
		} 
		else 
		{
			DEBUGP("Creating new hash entry\n");
			dag_it = dag_hash.insert(make_pair(src[node_id], vector<uint>())).first; 
		}
		assert(dag_it != dag_hash.end());
		if (!found) 
		{
			//Add a new entry to graph
			DEBUGP("Adding new entry\n");
			G.push_back(node_id);
			Garcs.push_back(vector<uint>());
			dag_it->second.push_back(G.size() - 1);

			const std::vector<uint> &children(src.c_children(node_id));
			for (uint j = 0; j < children.size(); ++j) 
			{
				assert(K.size() > children[j] + id_offset);
				Garcs[G.size() - 1].push_back(K[children[j] + id_offset]);
			}
			K[node_id + id_offset] = G.size() - 1;
			DEBUGP("Added new entry %u->%u\n", node_id, G.size() - 1);
		}
		
		//if(node_id != last_node)
		if(node_id != 0)
		{	parent = src.c_parent(node_id);
			children_control[parent]--;
			if(children_control[parent] == 0)
			{
				fila.push_back(parent);
				fila_size++;
			}
		}
		
		fila_it++;
	}
}

/**
  This function is responsible for finding equivalence classes within a tree.
*/
/*
template <class T, class _HashFcn, class _EqualKey>
void edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::addTree_sf(ctree<T> &src,  //Tree
							uint id_offset, // the offset number, if it is not the first tree, give the total size of previous trees.
							hash_type &dag_hash, // hash of node labels and equivalence classes.
							std::vector<uint> &G, //vector of equivalence classes
							std::vector<std::vector<int> >& Garcs, //vector that contains the equivalence classes children (other equivalent classes)
							std::vector<uint> &K) const //The final vector with the equivalence classes for each node in the tree, pay atention to the index+offset.
{

	using namespace std;
        using namespace __gnu_cxx;                                                 
	
	
	std::vector<uint>::const_iterator sit = src.post_order_walk().begin();
	std::vector<uint>::const_iterator send = src.post_order_walk().end();
	
	std::vector<uint> fila;
	
	vector<int> children_control;//controla a quantidade de filhos que já foi processada pela fila. Se o número de filhos for 0, o novo é inserido na fila.
	children_control.resize(src.c_size());

	vector<int> children_ref;//mantém o novo número de filhos da arvore, após a remocao dos nodos indesejáveis.
	children_ref.resize(src.c_size());
	
	
	uint parent;
	//uint last_node = src.size() -1;

	//Captura o número de filhos de cada nodo da árvore
	for (; sit != send; ++sit)
	{
		children_control[*sit] = src.c_children(*sit).size();
		children_ref[*sit] = children_control[*sit];
	}	
	
	//sit = src.post_order_walk().begin();
	//send = src.post_order_walk().end();

	//cout << "Passo 1\n";

	uint pre_it = 0;
	uint pre_lim = src.c_size();
	
	for (; pre_it < pre_lim; ++pre_it)

	{
		#ifdef KDEBUG
		cout << "\n=============\n[" << pre_it <<"] Nodo: " << (src[pre_it]).text() << endl;
		cout << "Eh tag: ";
		if(src[pre_it].isTag())
		{cout << "Sim" <<endl;}
		else{ cout << "Nao" << endl;}
		cout << "Filhos: " << children_control[pre_it] <<endl<<endl<<endl;
		#endif

		
		//se for folha, ou uma das tags B, I, P, marca este nodo e seus filhos como -1(indesejáveis)
		if(children_control[pre_it]==0 || 
                   ( src[pre_it].isTag() && (strcmp(src[pre_it].tagName().c_str(),"b")==0 || strcmp(src[pre_it].tagName().c_str() , "p" ) == 0 || 
		     strcmp(src[pre_it].tagName().c_str(),"B")==0 || strcmp(src[pre_it].tagName().c_str() , "P" ) == 0  || strcmp(src[pre_it].tagName().c_str() , "i" ) == 0 || strcmp(src[pre_it].tagName().c_str() , "I" ) == 0)  
		     ) 
		     )
		{
		
			#ifdef KDEBUG
			//cout << "It 1\n";
			//if(src[pre_it].isTag())
			   {cout <<"[" << pre_it << "]"<<"Tag: " << src[pre_it].tagName() << endl;}
			#endif

			uint llim = pre_it + src.c_size(pre_it);
			//marca este nodo e filhos como indesejáveis(-1)
			for(int aux=pre_it;aux < llim; aux++)
			{
				children_ref[aux]=-1;
				children_control[aux]=-1;
			}

			#ifdef KDEBUG			
			cout << "control: Pai: ";
			cout << (string)src[src.c_parent(pre_it)] << endl;
			#endif
	
			//se o indice da árvore nao for 0, ou seja, nodo lambda, o número de filhos no nodo pai deve ser decrementado.
			if(pre_it!=0)
			  {children_control[src.c_parent(pre_it)]--;
			  children_ref[src.c_parent(pre_it)]--;
			  }
			
			//ajuste em pre_it.
			pre_it = llim-1;
			//cout << "Fim\n";
			continue;
		}
	}



	pre_it = 0;
	pre_lim = src.c_size();
	//Agora, todos os nodos com 0 filhos devem ser inseridos na fila.
	for (; pre_it < pre_lim; ++pre_it)
	{
	
		#ifdef KDEBUG
		cout << (string)src[pre_it] << " tem " << children_ref[pre_it] <<" filhos...\n"; 
		#endif

		if(children_control[pre_it]==0)
		{
			fila.push_back(pre_it);
			//children_ref[pre_it]=-1;
		}
		
	}

	//cout << "Passo 2\n";
	

	//for (; sit != send; ++sit)
	uint node_id;
	int fila_it = 0, fila_size = fila.size();
	
	//while(! fila.empty() )

	//iteraje na fila
	while(fila_it < fila_size )
	{
		#ifdef KDEBUG
		cout << "Iteragindo ...\n";
		#endif

		node_id = fila[fila_it];
		//node_id = fila.front();
		//fila.pop_front();
		
		DEBUGP("Checking node %u (%u)\n", node_id, node_id + id_offset);
		typename hash_type::iterator dag_it;
		//pode mudar a funcao de comparacao aqui!!
		
		//se for uma destas tags, pede para processar atributos. Atualmente, este if nao tem nenhum efeito futuro.
		if(src[node_id].tagName()=="img" || src[node_id].tagName()=="a" || src[node_id].tagName()=="div")
		{
			src[node_id].parseAttributes();
		}
		

		//verifica se o rótulo do nodo atual está no hash de classes de equivalencia
		dag_it = dag_hash.find(src[node_id]);
		#ifdef KDEBUG
		cout << "O nodo " << (string)src[node_id]<<" estah no hash??"<<endl;
		#endif

		bool found = false;
		//se achou o rótulo, verifica todas as classes de equivalencia que 
		if (dag_it != dag_hash.end()) 
		{
			#ifdef KDEBUG
			cout << "Sim" <<endl;
			#endif

			DEBUGP("Found hash_entry with %u candidates\n", dag_it->second.size());

			vector<uint>::const_iterator vit = dag_it->second.begin();
			vector<uint>::const_iterator vend = dag_it->second.end();
			assert(vit != vend);                           
			for (;vit != vend; ++vit) // iterage nas classes de equivalencia que tem o rótulo src[pre_it]
			{
				//assert(Garcs.size() > *vit);
				uint narcs = Garcs[*vit].size();	
				#ifdef KDEBUG
				cout << "A classe "<< *vit << " tem "<< narcs << " filhos."<<endl;
				#endif
				//if (narcs != src.c_children(node_id).size()) continue;

				int num_children = 0;
				if (children_ref[node_id]!=-1)
				   num_children = children_ref[node_id];
				 
				 #ifdef KDEBUG
				cout << "Este candidato " << (string)src[node_id] << " tem "<< children_ref[node_id] << "filhos."<<endl;
				#endif

				if (narcs != num_children) 
				{
					#ifdef KDEBUG
					cout << "Mas nao tem os mesmos filhos."<< endl;
					#endif
					continue;
					}			

				const std::vector<uint> &children(src.c_children(node_id));
				found = true;
				//compara se os nodos filhos sao iguais
				if(children_ref[node_id] != -1)
				{
					//for (uint j = 0; j < children.size() && j < narcs; ++j) 
					int ww = -1;
					for (uint j = 0; j < children.size(); ++j) 
					{
						//assert(K.size() > children[j] + id_offset);
						if(children_ref[children[j]] !=-1)
						{
							ww++;
							if(K[children[j] + id_offset] != Garcs[*vit][ww]) 
							{
								DEBUGP("Children mismatch\n");
								found = false;
								break;	
								}
						}
					}
				}
				if (found) break;
			}

			//se os filhos sao iguais, ele registra a classe de equivalencia atual como a sendo o do nodo corrente em node_id.
			if (found) 
			{
				DEBUGP("Matched! K is sized %u sit->pid is %u and id_offset is %u\n", K.size(), node_id, id_offset);
				//If they match, add entry to K
				//assert(K.size() > node_id + id_offset);
				//assert(vit != vend);
				K[node_id + id_offset] = *vit; //guarda a classe de equivalencia na posicao correspondente + id_offset.
				//DEBUGP("Added en	try %u->%u\n", node_id, *vit);
			}
		} 
		else 
		{

			//caso nao ache o rótulo no hash, cria uma nova entrada no mesmo hash.
			//DEBUGP("Creating new hash entry\n");
			#ifdef KDEBUG
			cout << "Nao encontrou"<<endl;
			#endif
			dag_it = dag_hash.insert(make_pair(src[node_id], vector<uint>())).first; 
			
						
			
			
			#ifdef KDEBUG
			dag_it = dag_hash.find(src[node_id]);
			if(dag_it != dag_hash.end())
			{
				cout << "Inseriu.\n";
			}
			else{cout << "Nao inseriu."<<endl;} 
			#endif
		}

		assert(dag_it != dag_hash.end());


		if (!found)  // se nao achou a classe de equivalência para o rótulo.
		{
			//Adiciona uma nova entrada nos vetores G e Garcs.
			//DEBUGP("Adding new entry\n");
			G.push_back(node_id+id_offset);//adiciona um novo nodo em G, o nodo que primeiro definiu a nova classe de equivalencia.
			Garcs.push_back(vector<uint>());//prepara para gravar as classes de equivalência filhas.
			
			dag_it->second.push_back(G.size()-1);//na entrada atual do hash para o rótulo atual, adiciona a classe de equivalência nova.
			
			//Atencao: um rótulo pode estar na raiz de mais de uma classe de equivalência, por isso que é preciso usar o dab_it->second //como 				um vetor. Para guardar todas as classes de equivalência para o rotulo atual.
				
			

			//iterage nos filhos, e guarda as respectivas classes de equivalência.
			const std::vector<uint> &children(src.c_children(node_id));
			for (uint j = 0; j < children.size(); ++j) 
			{
				assert(K.size() > children[j] + id_offset);
	
				//caso, o filho atual seja indesejado, ignora.
				if(children_ref[children[j]]!=-1)
				{
					Garcs[G.size() - 1].push_back(K[children[j] + id_offset]);
				}
			}

			//guarda a classe de equivalencia para o nodo node_id+id_offset em K
			K[node_id + id_offset] = G.size()-1;// it was G.size() -1 
			#ifdef KDEBUG
                        cout << "A classe de "<< (string)src[node_id] << " será "<< K[node_id+id_offset] << endl;		
			#endif

			//DEBUGP("Added new entry %u->%u\n", node_id, G.size()-1); // it was G.size() -1 
		}
		
		//if(node_id != last_node)
		//se nao for o primeiro nodo da árvore, nodo lamda, atualiza o número de filhos em children_control
		if(node_id != 0)
		{	
			parent = src.c_parent(node_id);
			children_control[parent]--;
			//caso todos os filhos dos pai atual for processado, adiciona da fila.
			if(children_control[parent] == 0)
			{
				fila.push_back(parent);
				fila_size++;
			}
		}
		
		fila_it++;
	}
}


template <class T, class _HashFcn, class _EqualKey> //Karane -- 04/12/2008
void edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::addTree_sf2(ctree<T> &src, 
							uint id_offset, 
							hash_sf_type &dag_hash, 
							std::vector<uint> &G, 
							std::vector<std::vector<uint> >& Garcs, 
							std::vector<uint> &K) const 
{

	using namespace std;
        using namespace __gnu_cxx;                                                 
	
	
	//============ Determina as classes de equival�ncia finais K ====================
	std::vector<uint>::const_iterator sit = src.post_order_walk().begin();
	std::vector<uint>::const_iterator send = src.post_order_walk().end();
	
	//std::list<uint> fila;
	std::vector<uint> fila;
	
	vector<int> children_control;//verificar se podemos repassar a capacidade por aqui.
	children_control.resize(src.c_size());
	
	uint parent;
	//uint last_node = src.size() -1;
	//Captura as folhas de src tree
	for (; sit != send; ++sit)
	{
		children_control[*sit] = src.c_children(*sit).size();
	}	
	
	
	//do not take leaves in account -- Karane
	//sit = src.post_order_walk().begin();
	//send = src.post_order_walk().end();

	//for (; sit != send; ++sit)
	//{
	//	if( src.c_children(*sit).size() == 0 && !(src[*sit].tagName()=="img"))
	//	
	//	{
	//		children_control[src.c_parent(*sit)]--;
	//		children_control[*sit]= -1;
	//		//fila.push_back(src.c_parent(*sit));
	//	}
		
	//}
	// end -- Karane
	

	sit = src.post_order_walk().begin();
	send = src.post_order_walk().end();
	for (; sit != send; ++sit)
	{
		if( children_control[*sit] == 0)
		{
			//children_control[src.c_parent(*sit)]--;      
			fila.push_back(*sit);
		}	
	}
	

	//for (; sit != send; ++sit)
	uint node_id;
	int fila_it = 0, fila_size = fila.size();
	
	//while(! fila.empty() )
	while(fila_it < fila_size )
	{
		node_id = fila[fila_it];
		//node_id = fila.front();
		//fila.pop_front();
		
		DEBUGP("Checking node %u (%u)\n", node_id, node_id + id_offset);
		typename hash_sf_type::iterator dag_it;
		//pode mudar a funcao de comparacao aqui!!
		
		if(src[node_id].tagName()=="img" || src[node_id].tagName()=="a" || src[node_id].tagName()=="div")
		{
			src[node_id].parseAttributes();
		}
		

		uint hashkey = (unsigned int) hash<uint>()( get_node_hash(src[node_id]) );
		
		//dag_it = dag_hash.find(src[node_id]);
		dag_it = dag_hash.find(hashkey);
		bool found = false;
		if (dag_it != dag_hash.end()) 
		{
			DEBUGP("Found hash_entry with %u candidates\n", dag_it->second.size());
			//Look if children match
			vector<uint>::const_iterator vit = dag_it->second.begin();
			vector<uint>::const_iterator vend = dag_it->second.end();
			assert(vit != vend);                           
			for (;vit != vend; ++vit) // in case of collision
			{
				assert(Garcs.size() > *vit);
				uint narcs = Garcs[*vit].size();	
				if (narcs != src.c_children(node_id).size()) continue;

				const std::vector<uint> &children(src.c_children(node_id));
				found = true;
				//compara se os nodos filhos s�o iguais
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
				DEBUGP("Matched! K is sized %u sit->pid is %u and id_offset is %u\n", K.size(), node_id, id_offset);
				//If they match, add entry to K
				assert(K.size() > node_id + id_offset);
				assert(vit != vend);
				K[node_id + id_offset] = *vit;
				DEBUGP("Added en	try %u->%u\n", node_id, *vit);
			}
		} 
		else 
		{
			DEBUGP("Creating new hash entry\n");
			//dag_it = dag_hash.insert(make_pair(src[node_id], vector<uint>())).first;
			dag_it = dag_hash.insert(make_pair(hashkey, vector<uint>())).first;  
		}
		assert(dag_it != dag_hash.end());
		if (!found) 
		{
			//Add a new entry to graph
			DEBUGP("Adding new entry\n");
			G.push_back(node_id);
			Garcs.push_back(vector<uint>());
			dag_it->second.push_back(G.size()-1);// it was G.size() -1 

			const std::vector<uint> &children(src.c_children(node_id));
			for (uint j = 0; j < children.size(); ++j) 
			{
				assert(K.size() > children[j] + id_offset);
				Garcs[G.size() - 1].push_back(K[children[j] + id_offset]);
			}
			K[node_id + id_offset] = G.size()-1;// it was G.size() -1 
			DEBUGP("Added new entry %u->%u\n", node_id, G.size()-1); // it was G.size() -1 
		}
		
		//if(node_id != last_node)
		if(node_id != 0)
		{	parent = src.c_parent(node_id);
			children_control[parent]--;
			if(children_control[parent] == 0)
			{
				fila.push_back(parent);
				fila_size++;
			}
		}
		
		fila_it++;
	}
}
*/

template <class T, class _HashFcn, class _EqualKey>
void edtl::KSubtreeInspector<T, _HashFcn, _EqualKey>::addTreePath(ctree<T> &src, 
							uint id_offset, 
							path_hash_type &path_hash,
							PSH_type &PS_hash,
							uint *current_path_id,  
							uint *current_ps_id,  
							std::vector<uint> &K,
							std::vector<uint> &P,
							std::vector<uint> &PS) const 
{
//Determina classes de paths
	using namespace __gnu_cxx;
	uint pre_it;
	path_hash_type::iterator phit;//, path_hash_it;
	//compactPath cpath;
	char auxstr[500];
	std::string aux;
	uint hashkey;
	bool found;
	
	for(pre_it = 1; pre_it < src.c_size();pre_it++)
	{       
		//cpath.parent_path_id = P[src.c_parent(pre_it)];
		//cpath.tag_name = src[pre_it].tagName();
		sprintf(auxstr , "%d, %s", P[src.c_parent(pre_it)+id_offset], src[pre_it].tagName().substr(0, 450).c_str());
		aux = auxstr;
		
		hashkey = (unsigned int) hash< const char* >()( aux.c_str() );
		phit = path_hash.find(hashkey);
		
		found = false;
		if(phit != path_hash.end())
		{
			std::vector<struct path_type>::iterator path_it = phit->second.begin();
			std::vector<struct path_type>::iterator path_end = phit->second.end();
			
			
			for(; path_it!=path_end; path_it++)
			{
				if(path_it->parent_num == P[src.c_parent(pre_it)+id_offset] &&
				path_it->label == src[pre_it].tagName())
				{found = true;		
				 break;
				}

			}
			
			if(found)
			   P[pre_it+id_offset] = path_it->path_num;

				//P[pre_it+id_offset] = phit->second;
		}
		else
		{
			phit = path_hash.insert(make_pair(hashkey, std::vector<struct path_type>())).first;

		}		
		if(!found)
		{
			(*current_path_id)++;
			
			////path_hash.insert(make_pair(aux, *current_path_id));
			//path_hash[hashkey] = *current_path_id;
			path_type path_item;

			path_item.parent_num =  P[src.c_parent(pre_it)+id_offset];
			path_item.label =  src[pre_it].tagName();
			path_item.path_num = *current_path_id;
			
			//path_hash_it = path_hash.insert(make_pair(hashkey, std::vector<struct path_type>())).first;
			
			if(phit==path_hash.end())
			{
				printf("Error path hash iterator!!!\n");
				exit(1);
			}
			phit->second.push_back(path_item);
			
			P[pre_it+id_offset] = *current_path_id;
		}
	}
	
	
//========== Adiciona classes de finais
	
	PSH_type::iterator pshit;
	
	
	//std::list<uint> queue;
	std::vector<uint> queue;
	
	std::vector<int> children_control;//verificar se podemos repassar a capacidade por aqui.
	children_control.resize(src.c_size());
	
	uint parent;
	//uint last_node = src.size() -1;
	//Captura as folhas de src tree      
	for (pre_it =  0; pre_it < src.c_size(); pre_it++)
	{
		children_control[pre_it] = src.c_children(pre_it).size();
		if( children_control[pre_it] == 0 )
		{
			      queue.push_back(pre_it);
		}
		
	}
	
	uint node_id;
	PSH_type::iterator psit;//, ps_ins_it;
	ps_type ps_item;
	//path_subtree_pair psh_pair;
	uint queue_size = queue.size();
	int queue_it = 0;
	
		
	//while(! queue.empty() )
	while( queue_it < queue_size )
	//while(queue_it < queue.size)
	{
		node_id = queue[queue_it];
		//node_id = queue.front();
		//queue.pop_front();
				
		
		//psh_pair.path_id = P[node_id];
		//psh_pair.subtree_id = K[node_id];
		sprintf(auxstr, "%d, %d", P[node_id+id_offset], K[node_id+id_offset]);
		aux = auxstr;
		
		hashkey = (unsigned int) hash< const char* >()( aux.c_str() );
		
		psit = PS_hash.find(hashkey);
		
		found = false;
		if(psit != PS_hash.end())
		{
			//PS[node_id +id_offset] = psit->second;
			
			std::vector<struct ps_type>::iterator ps_type_it = psit->second.begin();
			std::vector<struct ps_type>::iterator ps_type_end = psit->second.end();

			for(; ps_type_it!=ps_type_end; ps_type_it++)
			{
				if(ps_type_it->p_num == P[node_id+id_offset] && 
				ps_type_it->k_num == K[node_id+id_offset])
				{
					found = true;
					break;
				}
			}
			
		
		
			if(found)
				PS[node_id +id_offset] = ps_type_it->ps_num;
		
		}
		else{
			psit = PS_hash.insert(make_pair(hashkey, std::vector<struct ps_type>())).first;
		}
		
		if(!found)
		{
			(*current_ps_id)++;
			////PS_hash.insert(make_pair(aux, *current_ps_id));
			//PS_hash[hashkey] = *current_ps_id;
			
			//ps_ins_it = PS_hash.insert(make_pair(hashkey, std::vector<struct ps_type>())).first;
			
			ps_item.p_num =  P[node_id+id_offset];
			ps_item.k_num =  K[node_id+id_offset];
			ps_item.ps_num = *current_ps_id;
			
			if(psit==PS_hash.end())
			{
				printf("Error PS hash iterator!!!\n");
				exit(1);
			}
			psit->second.push_back(ps_item);			

			PS[node_id +id_offset] = *current_ps_id;
		}
		
		if(node_id != 0)
		{	parent = src.c_parent(node_id);
			children_control[parent]--;
			if(children_control[parent] == 0)
			{
				queue.push_back(parent);
				queue_size++;
			}
		}
		
		queue_it++;
	}

	
}

#endif
