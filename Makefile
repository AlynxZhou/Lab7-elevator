# Filename: Makefile
# Created by 请叫我喵 Alynx.
# alynx.zhou@gmail.com, http://alynx.xyz/.

CC ?= gcc
CFLAGS ?= -std=c11
LIBS ?= -lSDL2 -lSDL2_ttf
OBJECTS := DataExport.o DataImport.o ElevatorSimulation.o main.o ScreenSimulation.o

all: CFLAGS += -O2
all: ${OBJECTS}
	${CC} ${CFLAGS} ${LIBS} -o lab7-elevator \
	   ${OBJECTS}

.PHONY: install
install: uninstall
	-mkdir Release
	cp lab7-elevator Release/
	cp -r SysConf/ Release/
	cp -r UserRequest/ Release/
	cp -r SimulationFiles/ Release/
	cp NotoSansCJKsc-Regular.otf Release/
	cp README.md Release/

.PHONY: uninstall
uninstall:
	-rm -rf Release/

.PHONY: debug
debug: CFLAGS += -g
debug: clean ${OBJECTS}
	${CC} ${CFLAGS} ${LIBS} -o lab7-elevator_debug ${OBJECTS}

.PHONY: clean
clean:
	-rm -f lab7-elevator lab7-elevator_debug ${OBJECTS} \
	     DataExport.h.gch DataModel.h.gch ScreenSimulation.h.gch \
	     DataImport.h.gch ElevatorSimulation.h.gch a.out

.PHONY: rebuild
rebuild: clean all

main.o: DataModel.h ElevatorSimulation.h

DataExport.o: DataModel.h DataImport.h

DataImport.o: DataModel.h

ScreenSimulation.o: DataModel.h DataExport.h DataImport.h ElevatorSimulation.h

ElevatorSimulation.o: DataModel.h DataExport.h DataImport.h ScreenSimulation.h
