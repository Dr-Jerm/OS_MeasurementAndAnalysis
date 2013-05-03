EXECUTABLES = os_bench ipc_mp_bench ipc_shmem_bench io_bench

CC = gcc
CFLAGS = -O0 -o
LFLAGS = -lrt
PFLAGS = -pthread
GFLAGS = -g
all: $(EXECUTABLES)

os_bench : stats.h os_bench.c util.c
	$(CC) $(CFLAGS) os_bench os_bench.c $(LFLAGS)

ipc_mp_bench : stats.h ipc_mp_bench.c util.c
	$(CC) $(CFLAGS) ipc_mp_bench ipc_mp_bench.c $(LFLAGS)

ipc_shmem_bench : stats.h ipc_shmem_bench.c util.c
	$(CC) $(CFLAGS) ipc_shmem_bench ipc_shmem_bench.c $(LFLAGS) $(PFLAGS) 

io_bench : stats.h io_bench.c util.c
	$(CC) $(CFLAGS) io_bench io_bench.c $(LFLAGS) $(GFLAGS)

clean:
	rm -f *.o $(EXECUTABLES)
