CXX=g++
CXXFLAGS=-std=c++17 -g3 -Wall -I/usr/include/R/
objdir=objs
srcdir=src

objects = $(addprefix $(objdir)/,primitives.o util.o entity_descriptor.o\
	variable_descriptor.o dictionary_descriptor.o)

redatam4r: $(objects) $(objdir)/main.o $(objdir)/libredatam.o
	$(CXX) $(CXXFLAGS) -o $@ $(objects) $(objdir)/main.o

$(objdir)/primitives.o: $(srcdir)/primitives.cpp $(srcdir)/primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

$(objdir)/util.o: $(srcdir)/util.cpp $(srcdir)/util.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/entity_descriptor.o: $(srcdir)/dic/entity_descriptor.cpp $(srcdir)/dic/entity_descriptor.h $(srcdir)/util.h $(srcdir)/primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/variable_descriptor.o: $(srcdir)/dic/variable_descriptor.cpp $(srcdir)/dic/variable_descriptor.h $(srcdir)/util.h $(srcdir)/primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/dictionary_descriptor.o: $(srcdir)/dic/dictionary_descriptor.cpp $(srcdir)/dic/dictionary_descriptor.h $(srcdir)/util.h $(srcdir)/primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/main.o: $(srcdir)/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/libredatam.o: $(srcdir)/libredatam.cpp $(srcdir)/libredatam.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(objdir)/*.o
	rm -f redatam4r
