CXX = g++
CPPFLAGS = -g -Wall
BIN_DIR = bin
CLASS_DIR = classes
STD_11 = -std=c++11

all: $(BIN_DIR)/.dirstamp search 

search: search.cpp $(BIN_DIR)/database.o $(BIN_DIR)/webpage.o 

	$(CXX) $(CPPFLAGS) $(STD_11) $^ -o $@

crawler: crawler.cpp 

	$(CXX) $(CPPFLAGS) $(STD_11) $^ -o $@

$(BIN_DIR)/database.o: $(CLASS_DIR)/database.cpp $(CLASS_DIR)/database.h $(BIN_DIR)/webpage.o setutility.h
	
	$(CXX) $(CPPFLAGS) $(STD_11) -c $< -o $@

$(BIN_DIR)/webpage.o: $(CLASS_DIR)/webpage.cpp $(CLASS_DIR)/webpage.h $(BIN_DIR)/.dirstamp

	$(CXX) $(CPPFLAGS) $(STD_11) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)

$(BIN_DIR)/.dirstamp:
	mkdir -p $(BIN_DIR)
	touch $(BIN_DIR)/.dirstamp