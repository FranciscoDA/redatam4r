CXXFLAGS=-O2 -fPIC -std=c++17 -Wall
CPPFLAGS=-Iinclude
LDLIBS=-lstdc++fs

INCLUDE_HEADERS=include/redatam/dic/entity_descriptor.h \
            include/redatam/dic/variable_descriptor.h \
            include/redatam/dic/dictionary_descriptor.h \
            include/redatam/util.h \
            include/redatam/read_dic.h \
            include/redatam/read_ptr.h \
			include/redatam/read_rbf.h \
            include/redatam/fs.h \
            include/redatam/redatam_exception.h \
            include/redatam/primitives.h

SRC_SOURCES=src/dic/dictionary_descriptor.cpp \
            src/dic/entity_descriptor.cpp \
            src/dic/variable_descriptor.cpp \
            src/redatam_exception.cpp \
            src/util.cpp \
            src/primitives.cpp

OBJ_OBJECTS=$(SRC_SOURCES:src/%.cpp=obj/%.o)

obj/%.o: src/%.cpp $(SRC_HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

all: lib/shared/libredatam.so lib/static/libredatam.a

lib/shared/libredatam.so: $(OBJ_OBJECTS) $(INCLUDE_HEADERS)
	@mkdir -p $(dir $@)
	g++ -shared $(CPPFLAGS) $(CXXFLAGS) -o $@ $(OBJ_OBJECTS)

lib/static/libredatam.a: $(OBJ_OBJECTS) $(INCLUDE_HEADERS)
	@mkdir -p $(dir $@)
	ar rcs $@ $(OBJ_OBJECTS)

.PHONY: clean
clean:
	rm -fr obj
	rm -fr lib
