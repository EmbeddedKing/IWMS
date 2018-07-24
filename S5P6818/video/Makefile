CC = arm-linux-gcc
CFLAGS = 
LFLAGS = -lpthread
OBJ = video_device.o video_list.o main.o
ELF = video

$(ELF):$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)
	cp $(ELF) /home/tarena/workdir/rootfs/rootfs

%.o:%.c
	$(CC) $(CFLAGS) -c $^ $(LFALGS)