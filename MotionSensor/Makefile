CC=gcc
CXX=g++
AR=ar
CXXFLAGS=-Wall -g -O2 -std=c++11 
CXX_OPTS=-c -DMPU6500 -DMPU_DEBUGOFF -I../libs/

SOURCES=sensor.cc inv_mpu_lib/inv_mpu.c inv_mpu_lib/inv_mpu_dmp_motion_driver.c
OBJECTS=$(SOURCES:.c=.o)

LIB=libMotionSensor.a

%.o: %.c
	$(CC) $(CXXFLAGS) $(CXX_OPTS) $< -o $@ 
	
%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CXX_OPTS) $< -o $@ 
	

$(LIB): $(OBJECTS) 
	$(AR) rcs $(LIB) $(OBJECTS) 

all: $(LIB)

clean:
	rm -rf *.o
	rm -rf inv_mpu_lib/*.o
	rm -rf $(LIB)
