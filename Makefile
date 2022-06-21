INCLUDEDIR=include
BUILDDIR=build
SRCDIR=src
CFLAGS=-g -Wall -I$(INCLUDEDIR)

.PHONY: all clean

all: $(BUILDDIR)/pam-minigame.so $(BUILDDIR)/test-app $(BUILDDIR)/game/asteroids

clean:
	rm -r $(BUILDDIR)

$(BUILDDIR)/test-app: $(SRCDIR)/test-app.c $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $< -lpam

$(BUILDDIR)/pam-minigame.so: $(SRCDIR)/pam-minigame.c $(BUILDDIR)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

$(BUILDDIR)/game/asteroids: $(SRCDIR)/asteroids.c $(BUILDDIR)/game
	$(CC) $(CFLAGS) -o $@ $< -lncurses

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/game:
	mkdir -p $@
