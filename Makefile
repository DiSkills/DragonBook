SRCDIR = src
SRCMODULES = $(SRCDIR)/buffer.cpp $(SRCDIR)/lexer.cpp
OBJMODULES = $(SRCMODULES:$(SRCDIR)/%.cpp=%.o)

CXX = g++
CXXFLAGS = -ggdb -Wall -Ilibs/DiTL

%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

prog: src/main.cpp $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	@$(CXX) -MM $^ > $@

clean:
	@rm -f prog *.o deps.mk
