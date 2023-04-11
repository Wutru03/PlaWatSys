all: meting plawatsys-socket-client plawatsys-socket-server toggle


MYSQL_LIBS := $(shell mariadb_config --libs)


meting: meting.c
    gcc -Wall -Wconversion -Wextra -o meting meting.c $(MYSQL_LIBS)


plawatsys-socket-server: plawatsys-socket-server.c
    gcc -Wall -Wconversion -Wextra -o plawatsys-socket-server plawatsys-socket-server.c


toggle: toggle.c
    gcc -Wall -Wconversion -Wextra -o toggle toggle.c $(MYSQL_LIBS)
