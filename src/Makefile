SHELL := /bin/bash

FUEGO_BASEPATH ?= ../../fuego

CXXFLAGS := $(shell grep CXXFLAGS ${FUEGO_BASEPATH}/Makefile | cut -d'=' -f2)
CXXFLAGS += -Wall -Wextra -Wundef -pedantic -funsafe-math-optimizations -fno-math-errno -O3 -g -std=c++11

SOURCES := $(wildcard [^_]*.cc)
HEADERS := $(wildcard [^_]*.h)
OBJECTS := $(foreach file,$(SOURCES),$(subst .cc,.o,$(file)))
TARGET := torajiro

EXT_INCL := -I${FUEGO_BASEPATH}/go -I${FUEGO_BASEPATH}/smartgame

EXT_LIBS := -L${FUEGO_BASEPATH}/go -L${FUEGO_BASEPATH}/smartgame -lfuego_go -lfuego_smartgame -lboost_system -lboost_filesystem

all: ${OBJECTS}
	${CXX} -o ${TARGET} ${OBJECTS} ${EXT_LIBS}

%.o: %.cc ${HEADERS}
	${CXX} -c -o $@ $< ${CXXFLAGS} ${EXT_INCL}

test:
	${MAKE} -C tests

clean: FORCE
	${RM} ${OBJECTS} ${TARGET}

FORCE:
