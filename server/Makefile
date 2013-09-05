include ../Make.defines
OFILE = server.o epollctl.o commonfunc.o hashtable.o semctl.o \
		wrapforserver.o ./threadpool/threadpool.o \
		./threadpool/queuectl.o ./threadpool/work.o
HFILE = header.h ./threadpool/threadstruct.h

DBOFILE = server_debug.o epollctl_debug.o commonfunc_debug.o hashtable_debug.o semctl_debug.o wrapforserver_debug.o ./threadpool/threadpool_debug.o \
		./threadpool/queuectl_debug.o ./threadpool/work_debug.o

release: server
	${MAKE} clean

server: ${OFILE} ${HFILE}
	${CC} -o $@ $^ ${LIBS}

${OFILE}: %.o: %.c ${HFILE}
	${CC} -c $< -o $@

debug: server_debug
	${MAKE} cleandebug

server_debug: ${DBOFILE} ${HFILE}
	${CC} -o $@ ${DEBUG} $^ ${LIBS}

${DBOFILE}: %_debug.o: %.c ${HFILE}
	${CC} -c ${DEBUG} $< -o $@

clean:
	-rm ${OFILE}

cleandebug:
	-rm ${DBOFILE}

cleanall:
	-rm ${OFILE} ${DBOFILE} server server_debug
