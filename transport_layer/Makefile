CC:=g++
CFLAGS := -c

MODELDIR := ../models
SRCDIR := src
LIBDIR := lib
BUILDDIR := build
TARGETDIR := bin
TARGET := bin/transport

# Normal CPP files
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
MODELSRCS := $(shell find $(MODELDIR) -type f -name *.$(SRCEXT))
MODELINC := $(shell find  $(MODELDIR) -type f -name *.hpp))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
MODELS := $(patsubst $(MODELDIR)/%,$(BUILDDIR)/%,$(MODELSRCS:.$(SRCEXT)=.o))

INC := -I include

$(TARGET): $(OBJECTS) $(MODELS)
	@echo " $(CC) $^ -o $(TARGET)"; $(CC) $^ -o $(TARGET)

# Build source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Build model files
$(BUILDDIR)/%.o: $(MODELDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<

maketransport:
	@mkdir -p $(TARGETDIR)
	@echo " $(CC) $(OBJECTS) $(MODELS) -o $(TARGET)";
	$(CC) $(OBJECTS) $(MODELS) -o $(TARGET)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR)"; $(RM) -r $(BUILDDIR)

.PHONY: clean
