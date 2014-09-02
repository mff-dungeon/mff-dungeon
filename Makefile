CC := g++
CC := clang
SRCDIR := src
DYNDIR := src/Objects/*.hpp
BUILDDIR := build
TARGET := bin/dungeon

SRCEXT := cpp
HEADEREXT := hpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT) -not -name "main.cpp")
DYNAMICS := $(shell echo $(DYNDIR))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11 -g -Wall -I/usr/local/include
LDFLAGS := -lstdc++ -lsqlite3 -L/usr/local/lib -lgloox

$(TARGET): $(OBJECTS) build/main.o
	@mkdir -p $(shell dirname $@)
	@echo "[ LD ] " $(TARGET)
	@$(CC) $^ -o $(TARGET) $(LDFLAGS)

src/dynamic.hpp: $(DYNAMICS)
	@printf "#ifndef DYNAMIC_HPP\n#define	DYNAMIC_HPP\n\n" > $@
	@echo $(DYNAMICS) | sed -r 's~src/([[:alnum:]/.-]+)\s*~#include "\1"\n~g' >> $@
	@printf "#endif\n" >> $@
	@echo "[ .. ] Dynamic header file generated."

build/main.o: src/main.cpp src/dynamic.hpp
	@mkdir -p $(shell dirname $@)
	@echo "[ CC ] " $< " --> " $@
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(SRCDIR)/%.$(HEADEREXT)
	@mkdir -p $(shell dirname $@)
	@echo "[ CC ] " $< " --> " $@
	@$(CC) $(CFLAGS) -c -o $@ $<

doc/Documentation.pdf: $(shell find doc/ -type f -not -iname "Documentation.pdf")
	@echo [ TeX ] Generating documentation...
	@cd doc && pdflatex main.tex
	@mv doc/main.pdf doc/Documentation.pdf

clean:
	$(RM) -r $(BUILDDIR) $(TARGET)
	$(RM) *.log

sedfix:
	@git checkout src/dynamic.hpp

tester: test/tester.cpp $(OBJECTS)
	@echo "[ CC ]  test/tester.cpp --> bin/tester"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o bin/tester $<
	
all: $(TARGET) doc

doc: doc/Documentation.pdf


.PHONY: clean all doc
