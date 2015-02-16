ODIR = ./objects
SDIR = ./src
IDIR = ./include
MAIN = monitor.cpp
EXECUTABLE = $(patsubst %.cpp,%, $(MAIN))
DDIR = ./dictionary
DICTIONARY = Dictionary.cpp
MYLIBRARY = libMonitor.so

MAKEFLAGS := -j$(shell nproc)
SOFLAGS = -shared
FLAGS := $(shell root-config --cflags)
FLAGS += -I. -I$(IDIR)
OPTFLAG = $(FLAGS) -Wall -Wextra -O3

LIBS :=  $(shell root-config --libs)
LIBS += -lrt

OBJS := $(patsubst %.cpp,%.o,$(addprefix $(ODIR)/,$(wildcard *.cpp)))
OBJS += $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(wildcard $(SDIR)/*.cpp))
OBJS += $(ODIR)/$(patsubst %.cpp,%.o, $(DICTIONARY))

.PHONY: clean

all: lib $(EXECUTABLE)  

debug:OPTFLAG = $(FLAGS) -Wall -Wextra -O0 -g
debug:lib $(EXECUTABLE)

lib: FLAGS += -fPIC
lib: $(MYLIBRARY)

$(OBJS): | $(ODIR)
$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/$(MAIN:.cpp=.o): $(MAIN)
	$(CXX) $(OPTFLAG) -c -o $@ $<
	
$(ODIR)/$(patsubst %.cpp,%.o, $(DICTIONARY)): $(DDIR)/$(DICTIONARY)
	$(CXX) $(OPTFLAG) -c -o $@ $<

$(ODIR)/%.o:$(SDIR)/%.cpp $(IDIR)/%.hpp
	$(CXX) $(OPTFLAG) -c -o $@ $<
	
$(DDIR)/$(DICTIONARY):$(wildcard $(IDIR)/*.hpp) $(wildcard $(IDIR)/*LinkDef.h)
	rootcling -f $@ -rml $(MYLIBRARY) -rmf $(patsubst %.so,%.rootmap, $(MYLIBRARY)) -c -p $^ 
	mv $(DDIR)/*.pcm .
	
$(MYLIBRARY):$(OBJS)
	$(CXX) $(SOFLAGS) -o $@ $^
	
$(EXECUTABLE):$(OBJS)
	$(CXX) -o $@  $^ $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(SDIR)/*~ $(IDIR)/*~  $(IDIR)/*.d $(DDIR)/* $(EXECUTABLE) *.txt *.root  *.so *.pcm *.rootmap *~ *.d
	

