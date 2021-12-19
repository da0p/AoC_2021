CXX = g++
CFLAGS = -g -Wall -Wextra -Werror -pedantic-errors -std=c++20
DEPS = 
TARGET = packet_decoder
OBJ = $(TARGET).o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
