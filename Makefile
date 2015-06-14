ODIR = ./objects
SDIR = ./src
IDIR = ./include
MAIN = monitor.cpp
EXECUTABLE = $(patsubst %.cpp,%, $(MAIN))

MAKEFLAGS := -j$(shell nproc)
FLAGS := $(shell root-config --cflags)
FLAGS += -I. -I$(IDIR)
OPTFLAG = $(FLAGS) -Wall -Wextra -O3 -MMD -MP

LIBS :=  $(shell root-config --libs)
LIBS += -lrt

OBJS := $(patsubst %.cpp,%.o,$(addprefix $(ODIR)/,$(wildcard *.cpp)))
OBJS += $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(wildcard $(SDIR)/*.cpp))

DEPS = $(patsubst %.o,%.d, $(OBJS))

.PHONY: clean

all: $(EXECUTABLE)  

debug:OPTFLAG = $(FLAGS) -Wall -Wextra -O0 -g
debug:lib $(EXECUTABLE)


$(OBJS): | $(ODIR)
$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/$(MAIN:.cpp=.o): $(MAIN)
	$(CXX) $(OPTFLAG) -c -o $@ $<

$(ODIR)/%.o:$(SDIR)/%.cpp $(IDIR)/%.hpp
	$(CXX) $(OPTFLAG) -c -o $@ $<
	
$(EXECUTABLE):$(OBJS)
	$(CXX) -o $@  $^ $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(DEPS) $(SDIR)/*~ $(IDIR)/*~ $(EXECUTABLE) *~
	
-include $(DEPS)