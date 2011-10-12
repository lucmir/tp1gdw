CURRENT_DIR = `pwd`

EXTRATOR_DIR = projeto-extracao

BINFOLDER = bin
TESTFOLDER = test

HTMLCXX_PATH = $(CURRENT_DIR)/$(EXTRATOR_DIR)/htmlcxx-0.7.3
HTMLCXX_BIN_DIR = out

KSUBTREES_DIR = $(CURRENT_DIR)/$(EXTRATOR_DIR)/classes_equivalencia

CXX = g++
CXXFLAGS = -DHAVE___GNU_CXX__EXT_HASH_MAP

LIBSFLAGS = -L $(HTMLCXX_PATH)/${HTMLCXX_BIN_DIR}/bin -lhtmlcxx
#-Wl,--rpath 
# -Wl,/usr/local/lib
		
COMPILE = $(CXX) $(CXXFLAGS) $(LIBSFLAGS)

# ksubtrees
KSUBTREES_TARGET = $(BINFOLDER)/ksubtrees
KSUBTREES_OBJS = $(KSUBTREES_DIR)/ksubtree_inspector_test.cc \
				 $(KSUBTREES_DIR)/ksubtree_inspector.h \
				 $(KSUBTREES_DIR)/ksubtree_inspector.inl \
				 $(KSUBTREES_DIR)/ctree.h \
				 $(KSUBTREES_DIR)/ctree.inl \
				 $(KSUBTREES_DIR)/my_hash_map.h ksubtrees.c
$(KSUBTREES_TARGET):
	$(COMPILE) -o $(KSUBTREES_TARGET) $(KSUBTREES_DIR)/ksubtrees.c

######## Commands ########

# htmlcxx
htmlcxx:
	cd $(HTMLCXX_PATH) && \
	make clean && \
	./configure --prefix=${CURRENT_DIR}/${HTMLCXX_BIN_DIR} && \
	make && \
	make install
	cd ..
htmlcxx_clean:
	rm -rfv $(HTMLCXX_PATH)/${HTMLCXX_BIN_DIR}*

# ksubtrees
ksubtrees:	$(KSUBTREES_TARGET)

# clean
clean:
	rm -rf $(BINFOLDER)/*
	rm -rf $(TESTFOLDER)/out/*
	rm -rf *~

# run
run:	$(EXTRATOR_DIR)/clean_pages.pl $(KSUBTREES_TARGET)
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/cdnow/pages/cd1.html > $(TESTFOLDER)/out/cd1_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/cd1_cleaned.html > $(TESTFOLDER)/out/cd1_result.html

