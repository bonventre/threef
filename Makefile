CFLAGS = -Iinclude $(shell root-config --cflags)
LFLAGS = $(shell root-config --libs) -lMinuit2
CCFLAGS = -Wno-deprecated-declarations
CC = g++
OBJ_DIR = build
SOURCES := $(shell find ./src -name '*.cpp')
OBJECTS = $(SOURCES:src/%.cpp=$(OBJ_DIR)/%.o)
INCLUDES := $(shell find ./src -name '*.h')
INCLUDES_DST = $(addprefix include/, $(subst ./src/,, $(INCLUDES)))# $(notdir $(INCLUDES))))
LIB = lib/libFFF.so

ifndef ROOTSYS
$(error ROOTSYS is not set)
endif

all: dirs includes $(OBJECTS) $(LIB)

clean:
	-$(RM) -r include/fff/* build/* lib/* $(EXE)

dirs:
	@test -d include/fff || mkdir -p include/fff
	@test -d build || mkdir build
	@test -d lib || mkdir lib

.PHONY: includes doc

includes: dirs $(INCLUDES_DST)

include/%: %
	@test -d $(dir $(subst src,include/fff,$^)) || mkdir -p $(dir $(subst src,include/fff,$^))
	cp -p $^ $(addprefix include/fff/, $(subst src/,, $^))

vpath %.h src
vpath samplers/%.h src/samplers

$(OBJ_DIR)/%.o: src/%.cpp
	$(CC) $(CCFLAGS) -c -o $@ $< $(CFLAGS)

$(LIB): $(OBJECTS)
	echo $(OBJECTS)
	$(CC) $(CCFLAGS) -shared -fPIC -o $@ $^ $(CFLAGS) $(LFLAGS)

#$(EXE): $(OBJECTS)
#	$(CC) $(CCFLAGS) -o $@ $^ $(CFLAGS) $(LFLAGS)

doc:
	doxygen doc/Doxyfile

