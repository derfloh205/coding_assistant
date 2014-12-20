SUBMISSION = ca
EXECUTABLE = $(SUBMISSION)
SOURCES    = $(wildcard cppFiles/*.cpp)
OBJECTS    = $(patsubst %,%,${SOURCES:.cpp=.o})
CXX        = g++
CXXFLAGS   = -Wall -g -c -std=c++11 -o
LDFLAGS    = 
LDLIBS     =
#-------------------------------------------------------------------------------

#make executable
all: $(EXECUTABLE) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $@ $< -MMD -MF ./$@.d

#link Objects
$(EXECUTABLE) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

#make clean
clean:
	rm -f ./cppFiles/*.o
	rm -f ./cppFiles/*.o.d
	rm -f $(EXECUTABLE)

#make valgrind
valgrind:
	valgrind --tool=memcheck --leak-check=full ./$(EXECUTABLE)

.PHONY: clean cleanw submission

#The dependencies:
-include $(wildcard *.d)

run:
	./$(SUBMISSION)
