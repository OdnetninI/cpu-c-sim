AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
bin_PROGRAMS = cpu-c-sim
cpu_c_sim_SOURCES = \
	src/main.c \
	src/base/queue.c \
	src/cpu/cpu.c \
	src/cpu/functional/functional.c \
	src/mem/mem.c \
	src/mem/request.c \
	src/mem/simple/simple.c

cpu_c_sim_CFLAGS = $(AM_CFLAGS)
