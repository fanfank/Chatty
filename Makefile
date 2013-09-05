#include ./Make.defines
release: release_client release_server

release_client:
	cd ./client && ${MAKE}

release_server:
	cd ./client && ${MAKE}

debug: debug_client debug_server

debug_client:
	cd ./client && ${MAKE} debug

debug_server:
	cd ./server && ${MAKE} debug

cleanall: cleanclient cleanserver

cleanclient:
	cd ./client && ${MAKE} cleanall

cleanserver:
	cd ./server && ${MAKE} cleanall


