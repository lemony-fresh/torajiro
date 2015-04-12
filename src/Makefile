SHELL := /bin/bash

FUEGO_BASEPATH ?= ../../fuego

CXXFLAGS := $(shell grep CXXFLAGS ${FUEGO_BASEPATH}/Makefile | cut -d'=' -f2)
CXXFLAGS += -std=c++11

SOURCES := $(wildcard [^_]*.cc)
OBJECTS := $(foreach file,$(SOURCES),$(subst .cc,.o,$(file)))
BINARY := main

EXT_INCL := -I${FUEGO_BASEPATH}/go -I${FUEGO_BASEPATH}/smartgame

EXT_LIBS := -L${FUEGO_BASEPATH}/go -L${FUEGO_BASEPATH}/smartgame -lfuego_go -lfuego_smartgame -lboost_system -lboost_filesystem-mt

all: ${OBJECTS}
	${CXX} -o ${BINARY} ${OBJECTS} ${EXT_LIBS}

%.o: %.cc
	${CXX} -c -o $@ $< ${CXXFLAGS} ${EXT_INCL}

clean: FORCE
	${RM} ${OBJECTS} ${BINARY}

FORCE:
