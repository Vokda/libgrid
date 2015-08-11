#
# $@ Contains the target file name.
# $< Contains the first dependency file name. 
#
# Declaration of variables
CC = g++
CC_FLAGS = -Wall -g -std=c++11 

# File names
EXEC = libgrid_test
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS_DIR)$(OBJECTS) -o $(EXEC)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $(OBJECTS_DIR)$@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS_DIR)$(OBJECTS)
