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
CFLAGS := -std=c++11 -g -O2 -Wall -DBOOST_SIGNALS_NO_DEPRECATION_WARNING

$(TARGET): $(OBJECTS)
	@mkdir -p $(shell dirname $@)
	@echo "[ LD ] " $(TARGET)
	@$(CC) $^ -o $(TARGET) -lsqlite3 -lSwiften -lboost_signals -lboost_thread -lboost_regex -lboost_program_options -lboost_filesystem -lboost_serialization -lboost_system -lboost_date_time -lz -lresolv -lpthread -ldl -lm -lc -lstdc++ -lgconf-2 -lgobject-2.0 -lglib-2.0 -lgconf-2 -lidn -lz -lssl -lcrypto -lxml2 -lavahi-client -lavahi-common -lresolv -lpthread -ldl -lm -lc -lstdc++

src/dynamic.hpp: $(DYNAMICS)
	@printf "#ifndef DYNAMIC_HPP\n#define	DYNAMIC_HPP\n\n" > $@
	@echo $(DYNAMICS) | sed -r 's~src/([[:alnum:]/.-]+)\s*~#include "\1"\n~g' >> $@
	@printf "#endif\n" >> $@
	@echo "[ .. ] Dynamic file generated."

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

tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester
	
all: $(TARGET)

.PHONY: clean all
