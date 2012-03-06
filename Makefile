TARGET = a.out
OBJS = main.o parser.o lexer.o symtab.o

CXXFLAGS = -O2 -Wall -pedantic -s

all: $(TARGET)

clean:
	-rm $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

main.o: parser.h
parser.o: parser.h symtab.h lexer.h
lexer.o: lexer.h
symtab.o : symtab.h
