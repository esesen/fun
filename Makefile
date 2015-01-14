CXXFLAGS ?= -g -fdiagnostics-color=auto

CXXFLAGS += -Wall --std=c++1y

%.opp: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

variant: variant.opp
	$(CXX) -o $@ $^
