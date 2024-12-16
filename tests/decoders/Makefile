rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BINARY_NAME = test
BINARIES_DIRECTORY = bin
INCLUDE_DIRECTORIES = ../../include
SOURCES_DIRECTORIES = . ../../src

SOURCES = $(foreach DIR,$(SOURCES_DIRECTORIES),$(call rwildcard,$(DIR),*.cpp))
OBJECTS = $(addprefix $(BINARIES_DIRECTORY)/,$(patsubst %.cpp,%.o,$(SOURCES)))

ifeq ($(OS),Windows_NT)
	BINARY = $(BINARY_NAME).exe
	CPPC = cl
	CFLAGS = /Zi /EHsc /nologo /std:c++14 /Wall $(foreach DIR,$(INCLUDE_DIRECTORIES),/I"$(DIR)") /c
	OUTPUT_FLAG = /Fo

	LINKER = link
	LINKER_OUTPUT_FLAG = /OUT:

	ensure_dir = @if not exist $(subst /,\,$(dir $1)) mkdir $(subst /,\,$(dir $1))
	rrmdir = rmdir $1 /s /q
else
	BINARY = $(BINARY_NAME).out
	CPPC = g++
	CFLAGS = -g -std=c++11 -Wall $(foreach DIR,$(INCLUDE_DIRECTORIES),-I'$(DIR)') -c
	OUTPUT_FLAG = -o 

	LINKER = g++
	LINKER_OUTPUT_FLAG = -o 

	ensure_dir = mkdir -p $1
	rrmdir = rm -rf $1
endif

compile: $(BINARY)

$(BINARY): $(OBJECTS)
	$(LINKER) $^ $(LINKER_OUTPUT_FLAG)$@

$(BINARIES_DIRECTORY)/%.o: %.cpp
	$(call ensure_dir,$(dir $@))
	$(CPPC) $(CFLAGS) $^ $(OUTPUT_FLAG)$@

clean:
	$(call rrmdir,$(BINARIES_DIRECTORY))
