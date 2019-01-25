CXXFLAGS=-O2 -fPIC -std=c++17 -Wall
LDFLAGS=-lstdc++fs

SRC_HEADERS=src/dic/entity_descriptor.h \
            src/dic/variable_descriptor.h \
            src/dic/dictionary_descriptor.h \
            src/util.h \
            src/read_dic.h \
            src/read_ptr.h \
			src/read_rbf.h \
            src/fs.h \
            src/redatam_exception.h \
            src/primitives.h

INCLUDE_HEADERS=$(SRC_HEADERS:src/%.h=include/redatam/%.h)

INCLUDE_DIRS=$(sort $(dir $(INCLUDE_HEADERS)))

SRC_SOURCES=src/dic/dictionary_descriptor.cpp \
            src/dic/entity_descriptor.cpp \
            src/dic/variable_descriptor.cpp \
            src/redatam_exception.cpp \
            src/util.cpp \
            src/primitives.cpp

OBJ_OBJECTS=$(SRC_SOURCES:src/%.cpp=obj/%.o)

OBJ_DIRS=$(sort $(dir $(OBJ_OBJECTS)))

obj/%.o: src/%.cpp $(SRC_HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

include/redatam/%.h: src/%.h
	@mkdir -p $(dir $@)
	cp $< $@

all: lib/shared/libredatam.so lib/static/libredatam.a $(INCLUDE_HEADERS)


lib/shared/libredatam.so: $(OBJ_OBJECTS)
	@mkdir -p $(dir $@)
	g++ -shared $(CPPFLAGS) $(CXXFLAGS) -o $@ $(OBJ_OBJECTS)

lib/static/libredatam.a: $(OBJ_OBJECTS)
	@mkdir -p $(dir $@)
	ar rcs $@ $(OBJ_OBJECTS)

.PHONY: clean
clean:
	rm -fr obj
	rm -fr lib
	rm -fr include
