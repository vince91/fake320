#Fake320 mac os x

CXX=g++

INCLUDES=-I./include
CFLAGS=$(INCLUDES) -c -std=c++11 
LIBS=-lavcodec -lavutil -lavformat -lfftw3
LDFLAGS=-L./lib $(LIBS)

EXEC=fake320
OBJDIR=obj
SRCDIR=src

SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

############################

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o

mrproper: clean
	rm -rf $(EXEC)