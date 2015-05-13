BASE_PATH := ..
GTEST_PATH ?= ${BASE_PATH}/../../gtest
FUEGO_BASEPATH ?= ${BASE_PATH}/../../fuego

TARGET := test
vpath gtest_main.a ${GTEST_PATH}/make/


EXT_INCL = -I${FUEGO_BASEPATH}/go -I${FUEGO_BASEPATH}/smartgame -I${GTEST_PATH}/include
CXXFLAGS := $(shell grep CXXFLAGS ${FUEGO_BASEPATH}/Makefile | cut -d'=' -f2)
CXXFLAGS += -Wall -Wextra -pedantic -funsafe-math-optimizations -fno-math-errno -O3 -g -std=c++11 -I${BASE_PATH} ${EXT_INCL}

SOURCES = $(wildcard ./gtest_*.cc)
LDLIBS += -L${FUEGO_BASEPATH}/go -L${FUEGO_BASEPATH}/smartgame -lfuego_go -lfuego_smartgame -lboost_system -lboost_filesystem

test: ${SOURCES:.cc=.o} gtest_main.a
	${LINK.cc} -o $@ $^ ${LDLIBS}

clean:
	${RM} ${TARGET} *.o

.PHONY: test
