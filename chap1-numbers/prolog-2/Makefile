
PROJECT=is_father
OUTPUT_PATH=./output

CPP=g++
LD=g++


CXXFLAGS= -std=c++11
CXXFLAGS+= -Wall -Wextra -Wconversion
CXXFLAGS+= -O0


LD_FLAGS= -ffunction-sections -fdata-sections
LD_LIBS= -lstdc++ -lc -lm 


CPP_SRC=$(wildcard ./*.cpp)

CPP_OBJ_TEMP=$(patsubst %.cpp, %.o, $(notdir $(CPP_SRC)))
CPP_OBJ_FILTER=
CPP_OBJ=$(filter-out $(CPP_OBJ_FILTER), $(CPP_OBJ_TEMP))

all: $(PROJECT)

$(addprefix $(OUTPUT_PATH)/,$(CPP_OBJ)): $(OUTPUT_PATH)/%.o: %.cpp
	$(CPP) -c $(CXXFLAGS) $< -o $@

$(PROJECT): $(addprefix $(OUTPUT_PATH)/, $(CPP_OBJ))
	$(LD) $(LD_FLAGS) -o $@ $^ $(LD_LIBS)


clean:
	rm -fr $(OUTPUT_PATH)/*.o *.s $(PROJECT) $(PROJECT).dSYM

astyle:
	astyle --style=stroustrup *.cpp
 
