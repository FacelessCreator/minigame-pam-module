INCLUDEDIR=include
BUILDDIR=build
SRCDIR=src
CFLAGS=-c -g -Wall -I$(INCLUDEDIR) -Wextra

.PHONY: all clean

all: $(BUILDDIR)/pam-minigame.so $(BUILDDIR)/test-app

clean:
	rm -r $(BUILDDIR)

$(BUILDDIR)/test-app: $(BUILDDIR)/test-app.o
	$(CC) -o $@ $(BUILDDIR)/test-app.o -lpam

$(BUILDDIR)/test-app.o: $(SRCDIR)/test-app.c $(BUILDDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/pam-minigame.so: $(BUILDDIR)/pam-minigame.o
	$(CC) -shared -o $@ $(BUILDDIR)/pam-minigame.o

$(BUILDDIR)/pam-minigame.o: $(SRCDIR)/pam-minigame.c $(BUILDDIR)
	$(CC) $(CFLAGS) -fPIC $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)
