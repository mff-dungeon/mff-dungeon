CC := g++
CC := clang
SRCDIR := src
DYNDIR := src/Objects/*.hpp
BUILDDIR := build
TARGET := bin/dungeon

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
DYNAMICS := $(shell echo $(DYNDIR))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11 -g -O2 -Wall -I/usr/local/include

$(TARGET): $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	@echo "[ LD ] " $(TARGET)
	@$(CC) $^ -o $(TARGET) -lsqlite3 -lstdc++ -L/usr/local/lib -lgloox

src/dynamic.hpp: $(DYNAMICS)
	@printf "#ifndef DYNAMIC_HPP\n#define	DYNAMIC_HPP\n\n" > $@
	@echo $(DYNAMICS) | sed -r 's~src/([[:alnum:]/.-]+)\s*~#include "\1"\n~g' >> $@
	@printf "#endif\n" >> $@
	@echo "[ .. ] Dynamic header file generated."

build/main.o: src/main.cpp src/dynamic.hpp
	@mkdir -p $(shell dirname $@)
	@echo "[ CC ] " $< " --> " $@
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(shell dirname $@)
	@echo "[ CC ] " $< " --> " $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) -r $(BUILDDIR) $(TARGET)
	$(RM) *.log

sedfix:
	@git checkout src/dynamic.hpp

tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester
	
all: $(TARGET)

.PHONY: clean all
