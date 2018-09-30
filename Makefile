CXX=g++
CXXFLAGS=-g3 -std=c++17
objdir=objs

objects = $(addprefix $(objdir)/,primitives.o entity_descriptor.o\
	variable_descriptor.o dictionary_descriptor.o util.o main.o)

redatam4r: $(objects)
	$(CXX) $(CXXFLAGS) -o $@ $(objects)

$(objdir)/primitives.o: primitives.cpp primitives.h $(objdir)
	$(CXX) $(CXXFLAGS) $< -o $@ -c

$(objdir)/entity_descriptor.o: dic/entity_descriptor.cpp dic/entity_descriptor.h $(objdir)
	$(CXX) $(CXXFLAGS) $< -o $@ -c
	
$(objdir)/variable_descriptor.o: dic/variable_descriptor.cpp dic/variable_descriptor.h $(objdir)
	$(CXX) $(CXXFLAGS) $< -o $@ -c

$(objdir)/dictionary_descriptor.o: dic/dictionary_descriptor.cpp dic/dictionary_descriptor.h $(objdir)
	$(CXX) $(CXXFLAGS) $< -o $@ -c

$(objdir)/util.o: util.cpp util.h $(objdir)
	$(CXX) $(CXXFLAGS) $< -o $@ -c

$(objdir)/main.o: main.cpp $(objdir)
	$(CXX) $(CXXFLAGS) $< -o $@ -c

$(objdir):
	mkdir -p $@

.PHONY: clean
clean:
	rm -f $(objdir)/*.o
	rm -fr $(objdir)
	rm -f redatam4r
