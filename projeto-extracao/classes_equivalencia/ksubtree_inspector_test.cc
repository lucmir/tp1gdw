
#include "config.h"
#include "ksubtree_inspector.h"
#include "my_hash_map.h"
#include <htmlcxx/html/ParserDom.h>
#include <string>
#include <utility>
#include <iostream>
#include <ext/hash_map>

//#define DEBUG
#include "debug.h"

using namespace std;
using namespace htmlcxx;
using namespace edtl;

using namespace std;
using namespace __gnu_cxx;


#define myassert(x) \
	do {\
		if(!(x)) fprintf(stderr, "Test at %s:%d failed!\n", __FILE__, __LINE__);\
	} while(0)

#ifdef HAVE___GNU_CXX__EXT_HASH_MAP
namespace __gnu_cxx {
#else
namespace std {
#endif
template <> struct MY_HASH<HTML::Node>
{
	size_t operator()(const HTML::Node &n) const {
		const char *__s = n.text().c_str();
		unsigned long __h = 0;
		for ( ; *__s; ++__s)
			__h = 5*__h + *__s;

		DEBUGP("Got hash value %u\n", __h);
		return size_t(__h);
	}
};


/*
template <> struct MY_HASH<std::string>
{
	size_t operator()(const std::string &n) const {
		const char *__s = n.c_str();
		unsigned long __h = 0;
		for ( ; *__s; ++__s)
			__h = 5*__h + *__s;

		DEBUGP("Got hash value %u\n", __h);
		return size_t(__h);
	}
};
*/

template<> struct hash< std::string > {
 size_t operator()( const std::string& x ) const {
          return hash< const char* >()( x.c_str() );
          }
 };

}

/*
namespace std {

template <> struct std::equal_to<std::string>
{
	bool operator()(const std::string &a, const std::string &b) const {
		return a == b;
	}
};

}
*/

/*
namespace __gnu_cxx {
template<> struct hash< std::string >                                                       
  {                                                                                           
    size_t operator()( const std::string& x ) const                                           
    {                                                                                         
      return hash< const char* >()( x.c_str() );                                              
    }                                                                                         
  };        

}
*/
struct node_comparator
{
	bool operator()(const HTML::Node &a, const HTML::Node &b) const
	{
		DEBUGP("Comparing labels %s and %s\n", a.text().c_str(), b.text().c_str());
		return strcmp(a.text().c_str(), b.text().c_str()) == 0;
	}
};


	
vector<pair<string, string> > read_files_from_repository(const string &basename, uint limit);
/*
class dag_test 
{
	public:

			
		bool test() {
			string html2 =	"<HTML><BODY>"
				"<UL>"
				"<B></B>"	
				"<LI>"
				"<LI>"
				"<LI>"
				"</UL>"
				"</BODY></HTML>";

			string html1 =	"<HTML><BODY>"
				"<UL>"
				"<B>List begins <I>here</I></B>"	
				"<LI>"	
				"<LI>"	
				"<LI>"	
				"</UL>"
				"</BODY></HTML>";

			HTML::ParserDom parser;
			DEBUGP("Parsing trees 1...\n");
			tree<HTML::Node> tr1 = parser.parseTree(html1);
			DEBUGP("Parsing trees 2...\n");
			tree<HTML::Node> tr2 = parser.parseTree(html2);
			DEBUGP("Creating rich trees...\n");
			ctree<HTML::Node> rtr1(tr1);
			assert(rtr1.size() == tr1.size());
			ctree<HTML::Node> rtr2(tr2);
			assert(rtr2.size() == tr2.size());
			cerr << "Tree 1" << tr1 << endl;
			cerr << "Tree 2" << tr2 << endl;
			vector<uint> K;

			SubtreeInspector<HTML::Node, MY_NS_HASH<HTML::Node>, node_comparator> inspec;
			inspec.equivalenceClasses(rtr1, rtr2, K);
			for(uint i = 0; i < rtr1.size(); ++i) {
				for (uint j = rtr1.size(); j < K.size(); ++j)
					{
						DEBUGP("Checking if %u and %u are identical\n", i, j - rtr2.size());
						assert(K.size() > i);
						assert(K.size() > j);
						if (K[i] == K[j])
						{
							cerr << "Subtrees " << i << ':' << j - rtr1.size() << " are identical" << endl;
						}
					}
			}

			cerr << "Comparing identical trees "<< endl;
			inspec.equivalenceClasses(rtr1, rtr1, K);
			for(uint i = 0; i < tr1.size(); ++i) {
				assert(K.size() > i + tr1.size());
				if (K[i] != K[i + tr1.size()])
				{
					cerr << "Algorithm failed. Look for bugs." << endl;
					return false;
				}
			}
			
			cerr << "----------" << endl;


			return true;
		}
};

bool multi_level_test()
{
	HTML::ParserDom parser;
	ctree<HTML::Node> tr1(parser.parseTree("<HTML><B><I>davi</I></B></HTML>"));
	ctree<HTML::Node> tr2(parser.parseTree("<HTML><I>davi</I></HTML>"));
	vector<uint> K;
	SubtreeInspector<HTML::Node, MY_NS_HASH<HTML::Node>, node_comparator> inspec;
	inspec.equivalenceClasses(tr1, tr2, K);
	for(uint i = 0; i < tr1.size(); ++i) {
		for (uint j = tr1.size(); j < K.size(); ++j)
		{
			DEBUGP("Checking if %u and %u are identical\n", i, j - tr2.size());
			myassert(K.size() > i);
			myassert(K.size() > j);
			if (K[i] == K[j])
			{
				cerr << "Subtrees " << i << ':' << j - tr1.size() << " are identical" << endl;
			}
		}
	}
	return true;
}

*/

struct class_info{
	string texto;
	string subtree_str;
	uint tree_number;
	uint prefix;
	uint count;
};


int main(int argc, char **argv) {        /*
	cerr << "Multilevel subtree equivalence tests... " << endl;
	myassert(multi_level_test());
	cerr << "Subtree equivalence tests... " << endl;
	*/
	//dag_test dt;
	//return dt.test() == 0;
		
	HTML::ParserDom parser;
	ctree<HTML::Node> tr1(parser.parseTree("<XML><TEXTO>Karane</TEXTO>"
			"<B><TITULO>Culinaria</TITULO></B>"
			"<B><TITULO>Politica</TITULO></B>"
			"<B><TITULO>Economia</TITULO></B>"
			"</XML>"));
	ctree<HTML::Node> tr2(parser.parseTree("<XML><TEXTO>Karane</TEXTO>"
			"<B><TITULO>Culinaria</TITULO></B>"
			"<B><TITULO>Ciencia</TITULO></B>"
			"<B><TITULO>Economia</TITULO></B>"
			"</XML>"));
	vector<uint> K;
	KSubtreeInspector<HTML::Node, MY_NS_HASH<HTML::Node>, node_comparator> inspec;
	inspec.subtreeEquivClasses(tr1, tr2, K);
	cout << "Tree 1: \n" << (tree<HTML::Node>)tr1 << endl;
	cout << "Tree 2: \n" << (tree<HTML::Node>)tr2 << endl;
	uint i;
	for( i = 0; i < tr1.size(); ++i) {
		for (uint j = tr1.size(); j < K.size(); ++j)
		{
			DEBUGP("Checking if %u and %u are identical\n", i, j - tr2.size());
			myassert(K.size() > i);
			myassert(K.size() > j);
			if (K[i] == K[j])
			{
				cerr << "Subtrees " << i << ':' << j - tr1.size() << " are identical" << endl;
			}
		}
	}

	ctree<HTML::Node> tr3(parser.parseTree("<XML><TEXTO>Karane</TEXTO>"
			"<B><TITULO>Aço</TITULO></B>"
			"<B><TITULO>Musica</TITULO></B>"
			"<B><TITULO>Economia</TITULO></B>"
			"</XML>"));

	KSubtreeInspector<HTML::Node, MY_NS_HASH<HTML::Node>, node_comparator>::hash_type dag_hash;
	hash_map<uint, struct class_info> class_stat;
	struct class_info info;
	vector<uint> G;
	std::vector<std::vector<uint> > Garcs;
	//vector<uint> K;
	K.resize(30000);
	char texto[1000];
	//hashkey = (unsigned int) hash< const char* >()( paths[i].c_str() );
	hash_map<uint, struct class_info>::iterator class_stat_it;
	//BH, o método abaixo adiciona uma árvore na identificação das classes de equivalência
	//O importante é vc deixar o dag_hash, o G, Garcs e o K limpos na primeira chamada de addTree
	//O segundo argumento é o offset para preencher K, logo, quando vc chamar pela segunda vez, ponha o tamanho
	//da primeira árvores, depois, |T1|+|T2|, depois, |T1|+|T2|+|T3|, etc
	inspec.addTree(tr1, (unsigned)0, dag_hash, G, Garcs, K);
	//BH, não ligue para a iteração abaixo
	for(i = 0; i< tr1.size(); i++)
	{
		
		sprintf(texto, "%d", K[i] );
		uint hashkey = (unsigned int) hash< const char* >()(texto);
		class_stat_it = class_stat.find(hashkey);
		if(class_stat_it != class_stat.end())
		{
			info = class_stat[hashkey];
			info.count++;
			}
		else{
			info.tree_number = 1;
			info.texto = texto;
			info.prefix = i;
			info.count  = 1;
			info.subtree_str = (string)tr1[i]; 
		}
		class_stat[hashkey] = info;
	}
	// BH, da segunda chamada em diante, vc deve usar o mesmos parâmetros
	// Só tenha cuidado com o tamanho que vc criou as variáveis. Ex.: K.resize(3000)
	// aqui botei o offset 0 por um motivo, mas a princípio, seria tr1.size()
	
	inspec.addTree(tr2, (unsigned)0, dag_hash, G, Garcs, K);	
	for(i = 0; i< tr2.size(); i++)
	{
		
		sprintf(texto, "%d", K[i+tr1.size()] );
		//printf("%s\n", texto);
		uint hashkey = (unsigned int) hash< const char* >()(texto);
		class_stat_it = class_stat.find(hashkey);
		if(class_stat_it != class_stat.end())
		{
			info = class_stat[hashkey];
			info.count++;
			}
		else{
			info.tree_number = 2;
			info.texto = texto;
			info.prefix = i;
			info.count  = 1;
			info.subtree_str = (string)tr1[i]; 
			printf("pre: %d pós:%d texto:%s\n",info.prefix,texto,info.subtree_str.c_str());

		}
		class_stat[hashkey] = info;
	}

	printf("=====================\n");
	//BH, Mais uma chamada
	inspec.addTree(tr3, (unsigned)0, dag_hash, G, Garcs, K);	
	for(i = 0; i< tr3.size(); i++)
	{
		
		sprintf(texto, "%d", K[i+tr1.size()+tr2.size()] );
		
		uint hashkey = (unsigned int) hash< const char* >()(texto);
		class_stat_it = class_stat.find(hashkey);
		if(class_stat_it != class_stat.end())
		{
			info = class_stat[hashkey];
			info.count++;
			}
		else{
			info.tree_number = 1;
			info.texto = texto;
			info.prefix = i;
			info.count  = 1;
			info.subtree_str = (string)tr1[i]; 
			printf("pre: %d pós:%d texto:%s\n",info.prefix,texto,info.subtree_str.c_str());

		}
		class_stat[hashkey] = info;
	}
	hash_map<uint, struct class_info>::iterator class_it;
	class_it = class_stat.begin();
	for(; class_it != class_stat.end(); class_it++)
	{
		info = class_it->second;
		if(info.count == 3)
		{
			cout << "["<< info.texto << "]" << info.subtree_str << "\n";
		}
	}
	return 1;
}
