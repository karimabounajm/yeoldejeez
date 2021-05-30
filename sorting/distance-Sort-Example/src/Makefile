all: mayday testFacility testRunway
CXXFLAGS=-Wall
CXX=g++
mayday: mayday.o Facility.o Runway.o gcdistance.o
	$(CXX) $(CXXFLAGS) -o $@ $^

testFacility: testFacility.o Facility.o gcdistance.o
	$(CXX) $(CXXFLAGS) -o $@ $^

testRunway: testRunway.o Runway.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o mayday testFacility testRunway
