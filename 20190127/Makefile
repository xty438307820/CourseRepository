Sources:=$(wildcard *.c)
Targets:=$(patsubst %.c,%,$(Sources))
RMSources:=$(patsubst %.c,%.out,$(Sources))
CC:=gcc

$(Targets):$(Sources)
	@for i in $(Targets);do $(CC) $${i}.c -o $${i}.out;done

clean:
	rm $(RMSources)
