CC = g++
LD = g++

CFLAGS = -c -g -pedantic -std=c++11 -Wall
LFLAGS = -g

OBJS = p6_main.o Model.o Views.o Controller.o
OBJS += Sim_object.o Structure.o Moving_object.o Agent.o
OBJS += Farm.o Town_Hall.o
OBJS += Peasant.o Warriors.o
OBJS += Agent_factory.o Structure_factory.o
OBJS += Geometry.o Utility.o Witch_doctor.o
PROG = proj6exe
STRUCTURE = structure_test
AGENT = agent_test

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p6_main.o: p6_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p6_main.cpp

Model.o: Model.cpp Model.h Views.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Model.cpp

Views.o: Views.cpp Views.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Views.cpp

Controller.o: Controller.cpp Controller.h Model.h Views.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Controller.cpp

Sim_object.o: Sim_object.cpp Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Sim_object.cpp

Structure.o: Structure.cpp Structure.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Structure.cpp

Farm.o: Farm.cpp Farm.h Structure.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Farm.cpp

Town_Hall.o: Town_Hall.cpp Town_Hall.h Structure.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Town_Hall.cpp

Agent.o: Agent.cpp Agent.h Moving_object.h Sim_object.h Model.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent.cpp

Peasant.o: Peasant.cpp Peasant.h Agent.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Peasant.cpp

Warriors.o: Warriors.cpp Warriors.h Agent.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Warriors.cpp

Moving_object.o: Moving_object.cpp Moving_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Moving_object.cpp

Agent_factory.o: Agent_factory.cpp Agent_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent_factory.cpp

Structure_factory.o: Structure_factory.cpp Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Structure_factory.cpp

Geometry.o: Geometry.cpp Geometry.h
	$(CC) $(CFLAGS) Geometry.cpp

Utility.o: Utility.cpp Utility.h
	$(CC) $(CFLAGS) Utility.cpp

Witch_doctor.o: Witch_doctor.cpp Witch_doctor.h
	$(CC) $(CFLAGS) Witch_doctor.cpp

Structure_test.o: Structure_test.cpp Geometry.h
	$(CC) $(CFLAGS) Structure_test.cpp

Agent_test.o: Agent_test.cpp Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent_test.cpp

clean:
	rm -f *.o
real_clean:
	rm -f $(PROG)
	rm -f *.o

