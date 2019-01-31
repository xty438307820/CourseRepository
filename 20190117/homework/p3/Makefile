Source:=$(wildcard *.c)
Objs:=$(patsubst %.c,%.o,$(Source))
Target:=main
CC:=gcc

$(Target):$(Objs)
	$(CC) $^ -o $@

clean:
	rm $(Objs) $(Target)
