include ../Make.defines
OFILE = client.o connectserver.o pressuretest.o wrap.o communicate.o semctl.o validate.o commonfunc.o shrmemotst.o

HFILE = header.h

DBOFILE = client_debug.o connectserver_debug.o pressuretest_debug.o wrap_debug.o communicate_debug.o semctl_debug.o validate_debug.o commonfunc_debug.o shrmemotst_debug.o

release: client
	${MAKE} clean

client: ${OFILE} ${HFILE}
	${CC} -o $@ ${OFILE} ${LIBS}

${OFILE}: %.o: %.c ${HFILE}
	${CC} -c $< -o $@

debug: client_debug
	${MAKE} cleandebug

client_debug: ${DBOFILE}
	${CC} -o $@ ${DEBUG} $^ ${LIBS}

${DBOFILE}: %_debug.o: %.c ${HFILE}
	${CC} -c ${DEBUG} $< -o $@

cleandebug:
	-rm ${DBOFILE}

clean:
	-rm ${OFILE}

cleanall:
	-rm ${DBOFILE} ${OFILE} client client_debug
