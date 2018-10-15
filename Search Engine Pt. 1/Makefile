CXX = g++
CPPFLAGS = -g -Wall
BIN_DIR = bin
CLASS_DIR = classes

all: $(BIN_DIR)/.dirstamp search

search: search.cpp $(BIN_DIR)/database.o $(BIN_DIR)/webpage.o 

	$(CXX) $(CPPFLAGS) $^ -o $@

$(BIN_DIR)/database.o: $(CLASS_DIR)/database.cpp $(CLASS_DIR)/database.h $(BIN_DIR)/webpage.o $(CLASS_DIR)/setutility.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/webpage.o: $(CLASS_DIR)/webpage.cpp $(CLASS_DIR)/webpage.h $(BIN_DIR)/.dirstamp

	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)

$(BIN_DIR)/.dirstamp:
	mkdir -p $(BIN_DIR)
	touch $(BIN_DIR)/.dirstamp