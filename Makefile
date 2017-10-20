FNAME = hw1


EXAMPLELIST = $(wildcard *.cpp)
TARGETS = $(basename $(EXAMPLELIST))
DEPS = $(TARGETS:=.o)

all: $(FNAME:=.o) $(TARGETS)
	g++ $(DEPS) -o $(FNAME)

$(FNAME:=.o): $(FNAME:=.cpp)
	g++ -c $(FNAME:=.cpp) -o $(FNAME:=.o) -O2 
	
$(TARGETS): 
	g++ -c $@.cpp -o $@.o -O2 

clean:
	rm *.o
	
cleanall:
	rm *.o