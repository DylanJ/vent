.SUFFIXES:
.SUFFIXES: .c .o

CC=gcc
PROGRAM=vent
CPPFLAGS=-Wall
LDLIBS=-pthread -lventrilo3
OBJDIR=obj
OUTDIR=bin

vpath %.c src
vpath %.h src

objects = $(addprefix $(OBJDIR)/, main.o channels.o users.o vent.o )
headers = $(channels.h users.h vent.h)

ku: $(objects)
	$(CC) $(CPPFLAGS) -o $(OUTDIR)/$(PROGRAM) $(objects) $(LDLIBS)

obj/%.o : %.c $(headers)
	$(CC) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean git
clean:
	rm $(objects)
	rm $(OUTDIR)/$(PROGRAM)

git:
	git push -u origin master
	
