all: ksubtree_inspector_test.cc ksubtree_inspector.h ksubtree_inspector.inl ctree.h ctree.inl my_hash_map.h ksubtrees.c
	g++ -o ksubtrees ksubtrees.c -DHAVE___GNU_CXX__EXT_HASH_MAP -lhtmlcxx -Wl,--rpath  -Wl,/usr/local/lib

ksub: ksubtree_inspector_test.cc ksubtree_inspector.h ksubtree_inspector.inl ctree.h ctree.inl my_hash_map.h
	g++  -c ksubtree_inspector_test.cc ksubtree_inspector_test.cc -DHAVE___GNU_CXX__EXT_HASH_MAP -lhtmlcxx -Wl,--rpath  -Wl,/usr/local/lib

clean:	
	rm -rf ksubtree_inspector_test ksubtree
