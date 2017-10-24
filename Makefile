CFLAGS += -ggdb -std=c11
LIBS = -lrt

all: mqmk mqrm mqsend mqrecv

mqmk: mqmk.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

mqrm: mqrm.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

mqsend: mqsend.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

mqrecv: mqrecv.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	-rm -f mqmk mqrm mqsend mqrecv

.PHONY: all clean
