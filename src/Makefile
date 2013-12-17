SHELL := /bin/bash

FUEGO_BASEPATH ?= ../../fuego/trunk

CXXFLAGS := $(shell grep CXXFLAGS ${FUEGO_BASEPATH}/Makefile | cut -d'=' -f2)
CXXFLAGS += -std=c++0x

SOURCES := $(wildcard [^_]*.cc)
OBJECTS := $(foreach file,$(SOURCES),$(subst .cc,.o,$(file)))
BINARY := main

EXT_INCL := -I${FUEGO_BASEPATH}/go -I${FUEGO_BASEPATH}/smartgame

EXT_LIBS := -L${FUEGO_BASEPATH}/go -L${FUEGO_BASEPATH}/smartgame -lfuego_go -lfuego_smartgame

all: ${OBJECTS}
	${CXX} -o ${BINARY} ${OBJECTS} ${EXT_LIBS}

%.o: %.cc
	${CXX} -c -o $@ $< ${CXXFLAGS} ${EXT_INCL}

clean: FORCE
	${RM} ${OBJECTS} ${BINARY}

FORCE:
