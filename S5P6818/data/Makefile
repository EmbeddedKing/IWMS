CC = arm-linux-gcc
CFLAGS = -L/home/tarena/workdir/rootfs/rootfs/lib -I/home/tarena/workdir/rootfs/rootfs/usr/include/include
LFLAGS = -lpthread -lsqlite3
OBJS = data_device.o data_thread.o main.o
ELF = data

$(ELF):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)
	cp $(ELF) /home/tarena/workdir/rootfs/rootfs

%.o:%.c
	$(CC) $(CFLAGS) -c $^