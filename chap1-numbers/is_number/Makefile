
PROJECT=is_number

CPP=g++
LD=g++


CXXFLAGS= -std=c++11
CXXFLAGS+= -Wall -Wextra -Wconversion
CXXFLAGS+= -O0


LD_FLAGS=
LD_LIBS= -lstdc++


CPP_SRC=$(wildcard ./*.cpp)

CPP_OBJ_TEMP=$(patsubst %.c, %.o, $(notdir $(CPP_SRC)))
CPP_OBJ_FILTER=
CPP_OBJ=$(filter-out $(CPP_OBJ_FILTER), $(CPP_OBJ_TEMP))

all: $(PROJECT)

$(CPP_OBJ): %.o : %cpp
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(PROJECT): $(CPP_OBJ)
	$(LD) $(LD_FLAGS) -o $@ $^ $(LD_LIBS)


clean:
	rm -fr *.o *.s $(PROJECT) $(PROJECT).dSYM


astyle:
	astyle --style=stroustrup *.cpp
 
