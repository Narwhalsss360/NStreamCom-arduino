rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BINARY_NAME = $(notdir $(CURDIR))
BINARIES_DIRECTORY = bin
INCLUDE_DIRECTORIES = NStreamCom/include
SOURCES_DIRECTORIES = .

SOURCES = $(foreach DIR,$(SOURCES_DIRECTORIES),$(call rwildcard,$(DIR),*.cpp))
OBJECTS = $(addprefix $(BINARIES_DIRECTORY)/,$(patsubst %.cpp,%.o,$(SOURCES)))

define \n


endef

ifeq ($(OS),Windows_NT)
	BINARY = $(BINARY_NAME).exe
	CPPC = cl
	CFLAGS = /Zi /EHsc /nologo /std:c++14 /Wall $(foreach DIR,$(INCLUDE_DIRECTORIES),/I"$(DIR)") /c
	OUTPUT_FLAG = /Fo
	CLEANUP = vc140.pdb

	LINKER = link /DEBUG
	LINKER_OUTPUT_FLAG = /OUT:

	ensure_dir = if not exist $(subst /,\,$(dir $1)) mkdir $(subst /,\,$(dir $1))
	rrmdir = rmdir $1 /s /q
	copy_dir = $(call ensure_dir,$2/.) && xcopy $(subst /,\,$1) $(subst /,\,$2) /s /y
	rm = del $1
else
	BINARY = $(BINARY_NAME).out
	CPPC = g++
	CFLAGS = -g -std=c++11 -Wall $(foreach DIR,$(INCLUDE_DIRECTORIES),-I'$(DIR)') -c
	OUTPUT_FLAG = -o 

	LINKER = g++
	LINKER_OUTPUT_FLAG = -o 

	ensure_dir = mkdir -p $1
	copy_dir = $(call ensure_dir,$2/.) && cp -r $(subst \,/,$1) $(subst \,/,$(dir $2))
	rrmdir = rm -rf $1
	rm = rm $1
endif

copy_then_compile: copy-library
	make compile

copy-library:
	$(call copy_dir,../../include,NStreamCom/include)
	$(call copy_dir,../../src,NStreamCom/src)

compile: $(BINARY)

$(BINARY): $(OBJECTS)
	@echo Linking...
	$(LINKER) $^ $(LINKER_OUTPUT_FLAG)$@

$(BINARIES_DIRECTORY)/%.o: %.cpp
	@echo ---------------------------------------------------------------------
	$(call ensure_dir,$(dir $@))
	$(CPPC) $(CFLAGS) $^ $(OUTPUT_FLAG)$@
	@echo ---------------------------------------------------------------------

clean:
	-$(call rrmdir,$(BINARIES_DIRECTORY))
	-$(call rrmdir,NStreamCom)
	-$(call rm,$(BINARY))
	-$(foreach file,$(CLEANUP),$(call rm,$(file))$(\n))
