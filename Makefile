ODIR = ./objects
SDIR = ./src
IDIR = ./include
MAIN = monitor.cpp
EXECUTABLE = $(patsubst %.cpp,%, $(MAIN))

MAKEFLAGS := -j$(shell nproc)
ROOTFLAGS := $(shell root-config --cflags)
OPTFLAGS := -Wall -Wextra -O3 -MMD -MP
FLAGS = $(ROOTFLAGS) -I. -I$(IDIR) $(OPTFLAGS)

LIBS :=  $(shell root-config --libs)
LIBS += -lrt

OBJS := $(patsubst %.cpp,%.o,$(addprefix $(ODIR)/,$(wildcard *.cpp)))
OBJS += $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(wildcard $(SDIR)/*.cpp))

DEPS = $(patsubst %.o,%.d, $(OBJS))

.PHONY: clean

all: $(EXECUTABLE)  

debug: OPTFLAGS = -Wall -Wextra -O0 -g
debug: all

$(OBJS): | $(ODIR)
$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/$(MAIN:.cpp=.o): $(MAIN)
	$(CXX) $(FLAGS) -c -o $@ $<

$(ODIR)/%.o:$(SDIR)/%.cpp $(IDIR)/%.hpp
	$(CXX) $(FLAGS) -c -o $@ $<
	
$(EXECUTABLE):$(OBJS)
	$(CXX) -o $@  $^ $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(DEPS) $(SDIR)/*~ $(IDIR)/*~ $(EXECUTABLE) *~
	
-include $(DEPS)