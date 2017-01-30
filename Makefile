OBJS=attitude_injector.o \
	 MotionSensor/libMotionSensor.a libs/libI2Cdev.a
BIN=picam360-capture.bin
LDFLAGS+=-lilclient -ljansson

include Makefile.include


