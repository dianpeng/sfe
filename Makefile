PWD:=$(shell pwd)
SOURCE=$(shell find src/ -type f -name "*.cc")
INCLUDE:=$(shell find src/ -type f -name "*.h")
OBJECT=${SOURCE:.cc=.o}
TEST:=$(shell find unittest/ -type f -name "*-test.cc")
TESTOBJECT:=${TEST:.cc=.t}

# depdency include
DINJECT_INC=-Idep/dinject/include
DINJECT_LIB=-Ldep/dinject/ -ldinject

CXXFLAGS+=-I$(PWD)/include -I$(PWD) $(DINJECT_INC) -std=c++17

LDFLAGS+=-lm -lsfml-graphics -lsfml-system -lsfml-audio -lsfml-window -lGL $(DINJECT_LIB)
CXX:=g++

all: $(OBJECT)

# -----------------------------------------------------------
# Testing
# -----------------------------------------------------------

src/%.o : src/%.cc src/%.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

TEST_FLAGS= -g3 -Wall
TEST_LIBS = -lgtest -lpthread

unittest/%.t : unittest/%.cc $(OBJECT) $(INCLUDE) $(SOURCE)
	$(CXX) $(OBJECT) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

test: CXXFLAGS += $(TEST_FLAGS)
test: LDFLAGS  += $(TEST_LIBS)
test: $(TESTOBJECT)

clean:
	rm -rf $(OBJECT)

.PHONY:clean
