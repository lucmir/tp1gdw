#include <string>
#include <htmlcxx/html/ParserDom.h>
#include <iostream>
#include <fstream>
#include <functional>

//#include <edtl/rtdm.h>
#include "ksubtree_inspector.h"
#include <ext/hash_map>
#include <map>
#include <utility>
#include <list>
#include <errno.h>

#include <string.h>

using namespace std;
using namespace edtl;
using namespace htmlcxx;
using namespace __gnu_cxx;


#ifdef HAVE___GNU_CXX__EXT_HASH_MAP
namespace __gnu_cxx {
#else
namespace std {
#endif

#define TOP_CLASSES_LIST_SIZE 10
#define CLASS_FREQ_DECAY_LIMITE 0.1

/**
	Define a funcao de hash para no HTML::Node.
*/

template <> struct MY_HASH<HTML::Node>
{
	size_t operator()(const HTML::Node &n) const 
	{
		const char *__s = NULL;
		if (n.isComment()) return 1;
		if (n.isTag()) __s = n.tagName().c_str();
		else __s = n.text().c_str();
		unsigned long __h = 0;
		for ( ; *__s; ++__s)
			__h = 5*__h + *__s;


		return size_t(__h);
		//return __h; // Karane
	}
	
	size_t operator() (const string &str) const
	{
		
	}
};

template<> struct hash< std::string > {
 size_t operator()( const std::string& x ) const {
          return hash< const char* >()( x.c_str() );
          }
 };

} //namespace

namespace std
{
template <>
struct std::equal_to<HTML::Node>
{
	bool operator()(const HTML::Node &a, const HTML::Node &b) const
	{
		if (a.isTag() && b.isTag()) return a.tagName() == b.tagName();
		if (a.isComment() && b.isComment()) return true;
		if (a.isTag() || b.isTag()) return false;
		if (a.isComment() || b.isComment()) return false;
		return a.text() == b.text();
	}
};


} //namespace std
	

tree<HTML::Node> loadHTMLFile(string f1) 
{

	if(! (f1.find(".html",0) > 0))
	{
		cerr << "Both files must be of HTML(.html) type."<<endl;
		exit(1);
	}
	
	ifstream file1(f1.c_str());
	if(!file1.is_open()) {
		cerr << "Unable to open file1: " << f1.c_str() << endl;
		cerr << strerror(errno) << endl;
		exit(1);
	}

	string html1;
	while(1) {
		char buf[BUFSIZ];
		file1.read(buf, BUFSIZ);
		if(file1.gcount() == 0) {
			break;
		}
		html1.append(buf, file1.gcount());
	}

	tree<HTML::Node> tr1 = HTML::ParserDom().parseTree(html1);
	return tr1;
}


//define a funcao de comparacao para o mesmo hash. Quando dois elementos sao iguais? Veja abaixo.
struct node_comparator
{
	bool operator()(const HTML::Node &a, const HTML::Node &b) const
	{
	  //DEBUGP("Comparing labels %s and %s\n", a.text().c_str(), b.text().c_str());
		//		return strcmp(a.text().c_str(), b.text().c_str()) == 0;
		if (a.isTag() && b.isTag()) return a.tagName() == b.tagName();
		if (a.isComment() && b.isComment()) return true;
		if (a.isTag() || b.isTag()) return false;
		if (a.isComment() || b.isComment()) return false;
		return a.text() == b.text();
	}
};


	
vector<pair<string, string> > read_files_from_repository(const string &basename, uint limit);

/************************/
void print(const tree<HTML::Node> &tr) {
	tree<HTML::Node>::pre_order_iterator it = tr.begin();	
	tree<HTML::Node>::pre_order_iterator end = tr.end();	

	int rootdepth = tr.depth(it);
	uint c = 0;
	while(it != end) {
		for(int i=0; i < tr.depth(it) - rootdepth; ++i) cout << "  ";
		cout << c << ":" << tr.depth(it) <<":"<<(*it).text()  << endl;
		++it;
		++c;
	}
	
	//return stream;
}


//declaration
void __printNodes(std::vector<uint> nodes, edtl::ctree<HTML::Node> *ct, std::string &out_file);


//imprime toda a subarvore em *ct, a partir do indice ind para a string out_file.
void printNodes(edtl::ctree<HTML::Node> *ct, uint ind, std::string &out_file)
{
                string name = (string)(ct->operator[](ind));
		string whole_text = (ct->operator[](ind)).text();
		//if(name == "br")
		//	   cout << name <<" ---> textarea has "<< (ct->c_children(i)).empty()<< " children." << endl;
		
		if((ct->operator[](ind)).isTag())
		 {
		 	//out_file << "<" << name << ">";
			//out_file <<  whole_text;
			out_file.append(whole_text);
			vector<uint> children;
			//for debug - karane 29/03
			
			
			   
			children = ct->c_children(ind);
			
			if(!(name == "br") && !(name=="img") && !(name=="image") /*children.size() > 0*/)
				{
				 //out_file<<endl;
				 
				 if(children.size() > 0)
				 	__printNodes(children, ct, out_file);	
				 
				 //for (uint j = 0; j < ct->c_depth(nodes[i]); ++j) out_file << " ";
				 out_file.append( "</");
				 out_file.append(name);
				 out_file.append(">");
				}
		 }
		 else {
		 	//out_file << name;
			out_file.append(whole_text);
		 	}
		
}

//imprime toda a subarvore em *ct, a partir do indice ind para a string out_file. (a funcao acima funciona chamando esta.)
void __printNodes(std::vector<uint> nodes, edtl::ctree<HTML::Node> *ct, std::string &out_file)
{
	using namespace std;


	for(uint i = 0; i < nodes.size(); ++i)
	{

		//DEBUGP("Depth is %u\n", c_depth(i));
		//for (uint j = 0; j < ct->c_depth(nodes[i]); ++j) out_file << "  ";
		
		string name = (string)(ct->operator[](nodes[i]));
		string whole_text = (ct->operator[](nodes[i])).text();
		//if(name == "br")
		//	   cout << name <<" ---> textarea has "<< (ct->c_children(i)).empty()<< " children." << endl;
		
		if((ct->operator[](nodes[i])).isTag())
		 {
		 	//out_file << "<" << name << ">";
			//out_file <<  whole_text;
			out_file.append(whole_text);
			vector<uint> children;
			//for debug - karane 29/03
			   
			children = ct->c_children(nodes[i]);
			
			if(!(name == "br") && !(name=="img") && !(name=="image") /*children.size() > 0*/)
				{
				 //out_file<<endl;
				 
				 if(children.size() > 0)
				 	__printNodes(children, ct, out_file);	
				 
				 //for (uint j = 0; j < ct->c_depth(nodes[i]); ++j) out_file << " ";
				 out_file.append( "</");
				 out_file.append(name);
				 out_file.append(">");
				}
		 }
		 else {
		 	//out_file << name;
			out_file.append(whole_text);
		 	}
		 
		//out_file << endl << flush;
	}
}



void print_node( const pair<unsigned int, unsigned int>& node ){
   cout << node.second << " - " << node.first << endl;
}


void RemoveSubtrees(map<unsigned int, unsigned int>& topClasses, ctree<HTML::Node>& ctr, vector<uint>& K, uint node) {
  vector<uint> children = ctr.c_children(node);
  for (uint i = 0; i < children.size(); ++i) {
    if (topClasses.count(K[children[i]]) > 0) {
      cout << "Estou removendo o cara " << K[children[i]]<< ",  filho do " <<K[node]<<endl;
      topClasses.erase(K[children[i]]);
    }
    else {
      RemoveSubtrees(topClasses, ctr, K, children[i]);
    }
  }
}



int main(int argc, char **argv) {

         if(argc != 2)
	   {
	     printf("usage: %s <html file>\n",argv[0]);
	     exit(1);
	   }
	 char* filename = argv[1]; //nome do arquivo html limpo (em uma linha apenas.)
	 
	 	

	vector<uint> K; //vetor que conterá as classes de equivalencia para cada nodo.
	KSubtreeInspector<HTML::Node, MY_NS_HASH<HTML::Node>, node_comparator> inspec; //o objeto que calcula as classes de equiv.
	uint i;
	KSubtreeInspector<HTML::Node, MY_NS_HASH<HTML::Node>, node_comparator>::hash_type dag_hash; //hash usado para calcular as classes de equiv.
	vector<uint> G;
	std::vector<std::vector<uint> > Garcs;
	
	K.resize(30000); //define o tamanho de K, deve ser no mínimo o tamanho da árvore.
	
	int offset = 0; //offset para K
	int lim = 0;
	int tamanho = 0;
	
	FILE *htmlfile = NULL;
	int tree_num = 0;
	
	tree<HTML::Node> tr = loadHTMLFile(filename); // Esta é a árvore DOM do tipo 1, bem genérica.
	//funcoes q podem ser uteis
        /*
	tree<HTML::Node>::iterator it = tr.begin();
	tree<HTML::Node>::iterator itend = tr.end();
	for(;it!=itend;it++)
	{
		cout << it->text() << endl;	
	}
	cout << "=================" << endl;
	*/
	//tr.erase(it), apaga o nodo na posição iterator.
        //mais detalhes em htmlcxx-0.7.3/html/tree.h

        //o tipo ctree<HTML::Node> possui tree<HTML::Node> e mais alguns metadados, isso é necessário para addTree()
	ctree<HTML::Node> ctr = ctree<HTML::Node>(tr);	//carrega a árvore do arquivo filename.
	inspec.addTree(ctr, (unsigned)offset, dag_hash, G, Garcs, K);	
	
	tamanho = ctr.size();

	//iterage em todos os nodos para fazer contagem de frequencia
	map<unsigned int, unsigned int> classFreq;
	map<unsigned int, unsigned int>::iterator classFreqItr;
  map<unsigned int, unsigned int> class_index;
	for(i = 0; i < tamanho; i++) {
		cout << "Indice: " << i << "\tClasse: " << K[i] << "\t" << ctr.c_depth(i) << "\tConteúdo do Nodo: " << ctr[i].text() << endl;//<< (string)ctr[i] <<endl;
    if (ctr.c_size(i) >= 3) {
		  classFreq[ K[i] ]++;
      class_index[ K[i] ] = i;
    }
	}

	//ordena frequencia das classes
	list< pair<unsigned int, unsigned int> > sortedClassFreq;
	for(classFreqItr=classFreq.begin(); classFreqItr!=classFreq.end(); classFreqItr++) {
		sortedClassFreq.push_back( make_pair(classFreqItr->second, classFreqItr->first) );
	}
	sortedClassFreq.sort();
	for_each( sortedClassFreq.begin(), sortedClassFreq.end(), print_node );

	//seleciona as classes mais frequentes
	map<unsigned int, unsigned int> topClasses;
	list< pair<unsigned int, unsigned int> >::iterator sortedClassFreqItr = sortedClassFreq.end();
	unsigned int lastFreq = (*--sortedClassFreqItr).first;
	for(unsigned int i=0;
		sortedClassFreqItr != sortedClassFreq.begin() && i < TOP_CLASSES_LIST_SIZE;
		sortedClassFreqItr--, i++) {

		unsigned int nodeFreq = (*sortedClassFreqItr).first;
		unsigned int nodeClass = (*sortedClassFreqItr).second;

		//verifica decaimento da frequencia
		if( ((double)nodeFreq / lastFreq) < CLASS_FREQ_DECAY_LIMITE ) {
			break;
		}
		lastFreq = nodeFreq;

		//adiciona classe ao top
		topClasses[nodeClass] = 0;
	}

	//imprime top
	map<unsigned int, unsigned int>::iterator topClassesItr = topClasses.begin();
	cout << "\nTopClasses:\n";
	for(; topClassesItr!=topClasses.end(); topClassesItr++) {
		cout << topClassesItr->first << "\n";
	}

  //remove subtrees dos top10
  for(topClassesItr = topClasses.begin(); topClassesItr != topClasses.end();
      ++topClassesItr) {
    RemoveSubtrees(topClasses, ctr, K, class_index[topClassesItr->first]);    
  }

	cout << "\nTopClasses:\n";
	for(topClassesItr = topClasses.begin(); topClassesItr!=topClasses.end(); topClassesItr++) {
		cout << topClassesItr->first << "\n";
	}

	//atribui classes para cada registro
	for(i = 0; i < tamanho; i++) {
		if(topClasses.find(K[i]) != topClasses.end()) {
			//TODO verifica se elementos das classes sao subarvores de arvores de outras classes
		}

	}

}

