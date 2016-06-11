CC := gcc
CFLAGS := -Wall -Wextra -std=gnu11 -O3
MAJOR_VERSION := 1
MINOR_VERSION := 0
MICRO_VERSION := 0
INCLUDE := $(foreach i,include, -I$i)
LIBRARY := c_nxt
SONAME := $(LIBRARY).so.$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
OBJECTS := $(wildcard src/*.c)
OBJECTS := $(OBJECTS:.c=.o)
LIBS := $(foreach l,bluetooth, -l$l)


all: $(SONAME)


$(SONAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -shared -Wl,-soname,$(SONAME) -o $(SONAME)
	@touch $(LIBRARY).so
	@rm $(LIBRARY).so
	ln -s $(SONAME) $(LIBRARY).so


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) -MD -fPIC -c $< -o $@


clean:
	@rm $(OBJECTS) $(SONAME) $(LIBRARY).so example


example: example.c $(SONAME)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJECTS) -L. $(LIBS) -o $@ $<


-include $(OBJECTS:.o=.d) example.d
