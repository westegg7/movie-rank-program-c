CC=cc
CVERSION=c11
CFLAGS=-g3 -Wall -Wextra -pedantic-errors
LINKFLAGS=-lm -pthread
SRCS=*.c
EXENAME=main

all     :       printinfo
	${CC} -o ${EXENAME} ${SRCS} ${CFLAGS} ${LINKFLAGS} 

printinfo       :
	@echo "CC : ${CC}"
	@echo "CVERSION : ${CVERSION}"
	@echo "CFLAGS : ${CFLAGS}"
	@echo "LINKFLAGS : ${LINKFLAGS}"
	@echo "SRCS : ${SRCS}"
	@echo "EXENAME : ${EXENAME}"
	@echo "----------------------------"
