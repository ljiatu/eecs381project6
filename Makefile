CC = g++
LD = g++

CFLAGS = -c -g -pedantic -std=c++11 -Wall
LFLAGS = -g

OBJS = p6_main.o Model.o View.o Visual_view.o List_view.o Local_view.o Map_view.o Health_view.o Amounts_view.o Combat_view.o Controller.o
OBJS += Sim_object.o Structure.o Moving_object.o Agent.o
OBJS += Farm.o Town_Hall.o
OBJS += Peasant.o Warriors.o Witch_doctor.o
OBJS += Agent_factory.o Structure_factory.o
OBJS += Geometry.o Utility.o
PROG = proj6exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p6_main.o: p6_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p6_main.cpp

Model.o: Model.cpp Model.h View.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Model.cpp

View.o: View.cpp View.h
	$(CC) $(CFLAGS) View.cpp

Visual_view.o: Visual_view.cpp Visual_view.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Visual_view.cpp

List_view.o: List_view.cpp List_view.h
	$(CC) $(CFLAGS) List_view.cpp

Local_view.o: Local_view.cpp Local_view.h
	$(CC) $(CFLAGS) Local_view.cpp

Map_view.o: Map_view.cpp Map_view.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Map_view.cpp

Health_view.o: Health_view.cpp Health_view.h
	$(CC) $(CFLAGS) Health_view.cpp

Amounts_view.o: Amounts_view.cpp Amounts_view.h
	$(CC) $(CFLAGS) Amounts_view.cpp

Combat_view.o: Combat_view.cpp Combat_view.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Combat_view.cpp

Controller.o: Controller.cpp Controller.h Model.h View.h Local_view.h Map_view.h Health_view.h Amounts_view.h Combat_view.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
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

Witch_doctor.o: Witch_doctor.h Witch_doctor.cpp Warriors.h Agent.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Witch_doctor.cpp

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

clean:
	rm -f *.o
real_clean:
	rm -f $(PROG)
	rm -f *.o

