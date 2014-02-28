CC = gcc
CFLAGS = -g -Wall -Wextra -O3
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
EXDIR = samples
TESTDIR = tests
TESTFILE = $(TESTDIR)/collections_test
INCLUDES = -I$(INCDIR)
LIBFLAGS = -cruvs
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGETS = lib test buildsamples

all: $(TARGETS)

lib: $(BINDIR)/libcollections.a

$(BINDIR)/libcollections.a: $(OBJ)
	@[ -d $(BINDIR) ] || mkdir $(BINDIR)
	@echo "Creating $@"
	@ar $(LIBFLAGS) $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

build-tests: lib
	@echo "Compiling $(TESTFILE).c"
	@$(CC) $(CFLAGS) $(INCLUDES) $(TESTFILE).c -o $(TESTFILE) -L$(BINDIR) -lcollections -lcunit

test: build-tests
	@echo "Running $(TESTFILE)"
	@$(TESTFILE)

buildsamples:
	@echo "Building samples"
	@make -C $(EXDIR)

clean: clean_tmp
	@rm -vrf $(OBJDIR)
	@make clean -C $(EXDIR)

clean_tmp:
	@rm -vf *~ $(SRCDIR)/*~ $(INCDIR)/*~ $(INCDIR)/collections/*~ $(TESTDIR)/*~
	@make clean_tmp -C $(EXDIR)

clean_tests:
	@rm -vf $(TESTFILE)

distclean mrproper veryclean: clean clean_tests
	@rm -vrf $(BINDIR)
	@make distclean -C $(EXDIR)
