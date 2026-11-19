CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinc -Iinclude

TARGET = airline_system.exe

OBJS = Admin.o \
       AirCraft.o \
       BookingAgent.o \
       Crew.o \
       FileHandler.o \
       Flight.o \
       main.o \
       Passenger.o \
       Payment.o \
       Person.o \
       Reservation.o \
       sha256.o \
       System.o \
       UI.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

Admin.o: src/Admin.cpp
	$(CXX) $(CXXFLAGS) -c src/Admin.cpp -o Admin.o

AirCraft.o: src/AirCraft.cpp
	$(CXX) $(CXXFLAGS) -c src/AirCraft.cpp -o AirCraft.o

BookingAgent.o: src/BookingAgent.cpp
	$(CXX) $(CXXFLAGS) -c src/BookingAgent.cpp -o BookingAgent.o

Crew.o: src/Crew.cpp
	$(CXX) $(CXXFLAGS) -c src/Crew.cpp -o Crew.o

FileHandler.o: src/FileHandler.cpp
	$(CXX) $(CXXFLAGS) -c src/FileHandler.cpp -o FileHandler.o

Flight.o: src/Flight.cpp
	$(CXX) $(CXXFLAGS) -c src/Flight.cpp -o Flight.o

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

Passenger.o: src/Passenger.cpp
	$(CXX) $(CXXFLAGS) -c src/Passenger.cpp -o Passenger.o

Payment.o: src/Payment.cpp
	$(CXX) $(CXXFLAGS) -c src/Payment.cpp -o Payment.o

Person.o: src/Person.cpp
	$(CXX) $(CXXFLAGS) -c src/Person.cpp -o Person.o

Reservation.o: src/Reservation.cpp
	$(CXX) $(CXXFLAGS) -c src/Reservation.cpp -o Reservation.o

sha256.o: src/sha256.cpp
	$(CXX) $(CXXFLAGS) -c src/sha256.cpp -o sha256.o

System.o: src/System.cpp
	$(CXX) $(CXXFLAGS) -c src/System.cpp -o System.o

UI.o: src/UI.cpp
	$(CXX) $(CXXFLAGS) -c src/UI.cpp -o UI.o

clean:
	del /q /f *.o $(TARGET) 2>nul || exit 0

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run