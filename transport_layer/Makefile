CC:=g++
CFLAGS := -c

SRCDIR := src
LIBDIR := lib
BUILDDIR := build
TARGETDIR := bin
TARGET := bin/transport

# Normal CPP files
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

INC := -I include

$(TARGET): $(OBJECTS)
	@echo " $(CC) $^ -o $(TARGET)"; $(CC) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

maketransport:
	@mkdir -p $(TARGETDIR)
	@echo " $(CC) $(OBJECTS) -o $(TARGET)";
	$(CC) $(OBJECTS) -o $(TARGET)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR)"; $(RM) -r $(BUILDDIR)

.PHONY: clean
