CXX=g++
CXXFLAGS=-std=c++17 -g3 -Wall
objdir=objs

objects = $(addprefix $(objdir)/,primitives.o util.o entity_descriptor.o\
	variable_descriptor.o dictionary_descriptor.o main.o)

redatam4r: $(objects)
	$(CXX) $(CXXFLAGS) -o $@ $(objects)

$(objdir)/primitives.o: primitives.cpp primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

$(objdir)/util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/entity_descriptor.o: dic/entity_descriptor.cpp dic/entity_descriptor.h util.h primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/variable_descriptor.o: dic/variable_descriptor.cpp dic/variable_descriptor.h util.h primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/dictionary_descriptor.o: dic/dictionary_descriptor.cpp dic/dictionary_descriptor.h util.h primitives.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(objdir)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<


.PHONY: clean
clean:
	rm -f $(objdir)/*.o
	rm -f redatam4r
