CURRENT_DIR = `pwd`

EXTRATOR_DIR = projeto-extracao

BINFOLDER = bin
TESTFOLDER = test

HTMLCXX_PATH = $(CURRENT_DIR)/$(EXTRATOR_DIR)/htmlcxx-0.7.3
HTMLCXX_BIN_DIR = out

KSUBTREES_DIR = $(CURRENT_DIR)/$(EXTRATOR_DIR)/classes_equivalencia

CXX = g++-3.3
CXXFLAGS = -DHAVE___GNU_CXX__EXT_HASH_MAP

LIBSFLAGS = -lhtmlcxx -Wl,--rpath  -Wl,/usr/local/lib


		
COMPILE = $(CXX) $(CXXFLAGS) $(LIBSFLAGS)

# ksubtrees
KSUBTREES_TARGET = $(BINFOLDER)/ksubtrees 
KSUBTREES_OBJS = $(KSUBTREES_DIR)/ksubtree_inspector_test.cc \
				 $(KSUBTREES_DIR)/ksubtree_inspector.h \
				 $(KSUBTREES_DIR)/ksubtree_inspector.inl \
				 $(KSUBTREES_DIR)/ctree.h \
				 $(KSUBTREES_DIR)/ctree.inl \
				 $(KSUBTREES_DIR)/ksubtrees.c \
				 $(KSUBTREES_DIR)/my_hash_map.h 
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
ksubtrees:	$(KSUBTREES_TARGET) $(KSUBTREES_OBJS)

# clean
clean:
	rm -rf $(BINFOLDER)/*
	rm -rf $(TESTFOLDER)/out/*
	rm -rf *~

# run
run:	$(EXTRATOR_DIR)/clean_pages.pl $(KSUBTREES_TARGET)
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/music_allmusic.com/pages/006.html > $(TESTFOLDER)/out/006_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/006_cleaned.html > $(TESTFOLDER)/out/006_result.html

# run_all
run_all:	$(EXTRATOR_DIR)/clean_pages.pl $(KSUBTREES_TARGET)
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/cdnow/pages/cd1.html > $(TESTFOLDER)/out/cd1_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/cd1_cleaned.html > $(TESTFOLDER)/out/cd1_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/cdnow/pages/cd2.html > $(TESTFOLDER)/out/cd2_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/cd2_cleaned.html > $(TESTFOLDER)/out/cd2_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/cdnow/pages/cd3.html > $(TESTFOLDER)/out/cd3_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/cd3_cleaned.html > $(TESTFOLDER)/out/cd3_result.html
	
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/monster/pages/monster1.html > $(TESTFOLDER)/out/monster1_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/monster1_cleaned.html > $(TESTFOLDER)/out/monster1_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/monster/pages/monster2.html > $(TESTFOLDER)/out/monster2_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/monster2_cleaned.html > $(TESTFOLDER)/out/monster2_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/monster/pages/monster4.html > $(TESTFOLDER)/out/monster4_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/monster4_cleaned.html > $(TESTFOLDER)/out/monster4_result.html
	
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/music_allmusic.com/pages/001.html > $(TESTFOLDER)/out/001_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/001_cleaned.html > $(TESTFOLDER)/out/001_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/music_allmusic.com/pages/005.html > $(TESTFOLDER)/out/005_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/005_cleaned.html > $(TESTFOLDER)/out/005_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/music_allmusic.com/pages/006.html > $(TESTFOLDER)/out/006_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/006_cleaned.html > $(TESTFOLDER)/out/006_result.html
	
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/wines/pages/winesByProducer01.html > $(TESTFOLDER)/out/winesByProducer01_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/winesByProducer01_cleaned.html > $(TESTFOLDER)/out/winesByProducer01_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/wines/pages/winesByProducer02.html > $(TESTFOLDER)/out/winesByProducer02_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/winesByProducer02_cleaned.html > $(TESTFOLDER)/out/winesByProducer02_result.html
	perl ${EXTRATOR_DIR}/clean_pages.pl $(TESTFOLDER)/in/TP1colecoes/wines/pages/winesByProducer03.html > $(TESTFOLDER)/out/winesByProducer03_cleaned.html
	./$(KSUBTREES_TARGET) $(TESTFOLDER)/out/winesByProducer03_cleaned.html > $(TESTFOLDER)/out/winesByProducer03_result.html
	
	rm $(TESTFOLDER)/out/*_cleaned.html
