CXXFLAGS ?= -g -fdiagnostics-color=auto

CXXFLAGS += -Wall --std=c++1y

default: variant

-include *.d

%.opp: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -o $@ -c $<

variant: variant.opp
	$(CXX) -o $@ $^

clean:
	@-rm variant *.opp
	
.PHONY: clean
