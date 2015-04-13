SSHELL = /bin/sh

CC = gcc

CFLAGS = -fpic -m32
CFLAGS64 = -fpic
CFLAGSARM = -fpic
CPPFLAGS = -DLINUX 
LDFLAGS =
LIBS =

DLL_OBJS = IRTransDLL.o

all: x86/libIRTrans.so x64/libIRTrans.so
arm: arm/libIRTrans.so


x86/libIRTrans.so: IRTransDLL.c IRTrans.h network.h remote.h
	$(CC) -shared $(CPPFLAGS) $(CFLAGS) IRTransDLL.c -o x86/libIRTrans.so  $(LDFLAGS)
	cp *.h x86

x64/libIRTrans.so: IRTransDLL.c IRTrans.h network.h remote.h
	$(CC) -shared $(CPPFLAGS) $(CFLAGS64) IRTransDLL.c -o x64/libIRTrans.so  $(LDFLAGS)
	cp *.h x64

arm/libIRTrans.so: IRTransDLL.c IRTrans.h network.h remote.h
	$(CC) -shared $(CPPFLAGS) $(CFLAGSARM) IRTransDLL.c -o arm/libIRTrans.so  $(LDFLAGS)
	cp *.h arm

dlltest: dlltest.o
	$(CC) -m32 dlltest.o -L. -lIRTrans -o dlltest

clean:
	rm -f x86/libIRTrans.so x86/*.h x64/libIRTrans.so x64/*.h arm/*.h

