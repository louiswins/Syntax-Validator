TARGET = a.out
OBJS = main.o parser.o lexer.o

CXXFLAGS = -O2 -Wall -pedantic -s

all: $(TARGET)

clean:
	-rm $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

main.o: parser.h lexer.h
parser.o: parser.h
lexer.o: lexer.h
