#CXX := g++
#CXX := clang
SRCDIR = src
BUILDDIR = build

SRCEXT = cpp
HEADEREXT = hpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
DYNAMICS = $(shell find $(SRCDIR)/Objects/ -type f -iname "*.$(HEADEREXT)")
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CPPFLAGS = -std=gnu++11 -g -Wall -I/usr/local/include -MD
LDFLAGS = -lstdc++ -lpthread -lsqlite3 -L/usr/local/lib -lgloox -lm
LOGFLAGS =

bin/dungeon: $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	@echo "[ LD ] " $@
	@$(CC) $^ -o $@ $(LDFLAGS)

src/dynamic.hpp: $(DYNAMICS)
	@printf "#ifndef DYNAMIC_HPP\n#define	DYNAMIC_HPP\n\n" > $@
	@echo $(DYNAMICS) | sed -r 's~src/([[:alnum:]/.-]+)\s*~#include "\1"\n~g' | sort >> $@
	@printf "#endif\n" >> $@
	@echo "[ .. ] Dynamic header file generated."

build/main.o: src/main.cpp
	@mkdir -p $(shell dirname $@)
	@echo "[ CC ] " $@
	@$(CXX) $(CPPFLAGS) $(LOGFLAGS) -D_LOGUNIT=\"$(notdir $<)\" -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(SRCDIR)/%.$(HEADEREXT)
	@mkdir -p $(shell dirname $@)
	@echo "[ CC ] " $@
	@$(CXX) $(CPPFLAGS) $(LOGFLAGS) -D_LOGUNIT=\"$(notdir $<)\" -c -o $@ $<

doc/Documentation.pdf: $(shell find doc/ -type f -not -iname "Documentation.pdf")
	@echo [ TeX ] Generating documentation...
	@cd doc && pdflatex main.tex
	@mv doc/main.pdf doc/Documentation.pdf

clean:
	$(RM) -r $(BUILDDIR) $(TARGET)
	$(RM) *.log dumps/*.dot

tester: test/tester.cpp $(OBJECTS)
	@echo "[ CC ]  test/tester.cpp --> bin/tester"
	@$(CXX) $(CPPFLAGS) $(LDFLAGS) $(OBJECTS) -o bin/tester $<

all: bin/dungeon bin/create  doc

debug: LOGFLAGS := -D_LOGMORE
debug: clean all

doc: doc/Documentation.pdf

worldgraph: dumps/world-last.dot
	@echo "[ DOT ]  dumps/world-last.dot --> dumps/world.pdf"
	@dot -Tpdf <dumps/world-last.dot >dumps/world.pdf
	@echo "[ DOT ]  dumps/world-last.dot --> dumps/world.png"
	@dot -Tpng <dumps/world-last.dot >dumps/world.png


-include $(OBJECTS:.o=.d)

.PHONY: clean all doc debug worldgraph
