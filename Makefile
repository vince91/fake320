#Fake320 mac os x

CXX=g++

INCLUDES=
CFLAGS=$(INCLUDES) -c -std=c++11 
LIBS=-lavcodec -lavutil -lavformat -lfftw3
LDFLAGS=$(LIBS)

EXEC=fake
SRCDIR=Fake320

SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(SRC:.cpp=.o)

############################

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(SRCDIR)/*.o

mrproper: clean
	rm -rf $(EXEC)