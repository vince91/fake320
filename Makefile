#Fake320 mac os x

CXX=g++

INCLUDES=
CFLAGS=$(INCLUDES) -c -std=c++11 
LIBS=-lavformat -lavcodec -lavutil -lfftw3
LDFLAGS=$(LIBS)

EXEC=fake
SRCDIR=Fake320

SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(SRC:.cpp=.o)

############################

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) 

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(SRCDIR)/*.o

mrproper: clean
	rm -rf $(EXEC)