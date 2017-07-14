CC:=g++
CFLAGS := -g -c

MODELDIR := ../models
SRCDIR := src
UTILDIR := ../utils
LIBDIR := lib
BUILDDIR := build
TARGETDIR := bin
TARGET := bin/transport

# Normal CPP files
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
UTILSRCS := $(shell find $(UTILDIR) -type f -name *.$(SRCEXT))
MODELSRCS := $(shell find $(MODELDIR) -type f -name *.$(SRCEXT))
MODELINC := $(shell find  $(MODELDIR) -type f -name *.hpp))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
MODELS := $(patsubst $(MODELDIR)/%,$(BUILDDIR)/%,$(MODELSRCS:.$(SRCEXT)=.o))
UTILS := $(patsubst $(UTILDIR)/%,$(BUILDDIR)/%,$(UTILSRCS:.$(SRCEXT)=.o))

INC := -I include

# Build source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Build model files
$(BUILDDIR)/%.o: $(MODELDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<

# Build util files
$(BUILDDIR)/%.o: $(UTILDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<

maketransport: $(OBJECTS) $(MODELS) $(UTILS)
	@mkdir -p $(TARGETDIR)
	@echo " $(CC) $^ -o $(TARGET)";
	$(CC) $^ -o $(TARGET)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR)"; $(RM) -r $(BUILDDIR)

.PHONY: clean
