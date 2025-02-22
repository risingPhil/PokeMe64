V=1
SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

# the -fno-rtti option is necessary here because libpokemegb defines it too. Otherwise you get an "undefined reference to typeinfo" link error
# when using libpokemegb classes. Source: https://stackoverflow.com/questions/11904519/c-what-are-the-causes-of-undefined-reference-to-typeinfo-for-class-name
N64_C_AND_CXX_FLAGS += -I include -I libpokemegb/include -fno-rtti -fno-exceptions
N64_LDFLAGS += -Llibpokemegb -lpokemegb
N64_ROM_REGIONFREE=1
N64_ROM_CONTROLLER_TYPE1=n64,pak=transfer
N64_ROM_TITLE="PokeMe64"

SRCS := $(shell find $(SOURCE_DIR) -type f -name '*.cpp')
OBJS := $(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

assets_ttf = $(wildcard assets/*.ttf)
assets_png = $(wildcard assets/*.png)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_ttf:%.ttf=%.font64))) \
              $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite)))

MKSPRITE_FLAGS ?=
MKFONT_FLAGS ?=

all: PokeMe64.z64
.PHONY: all

filesystem/%.font64: assets/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem "$<"

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"

# We also need Japanese characters
# I wrote a program to determine the character ranges for the japanese characters actually used by
# the gameboy games.
filesystem/Arial.font64: MKFONT_FLAGS+= -v --size 11 --outline 1.0 --char-spacing 1.0 --range 20-E9 --range 2026-2026 --range 25B6-25B7 --range 25BC-25BC --range 2640-2640 --range 2642-2642 --range 3002-3002 --range 300C-300F --range 3041-3089 --range 308B-308D --range 308F-308F --range 3092-309C --range 30A1-30D7 --range 30DB-30EF --range 30F2-30F4 --range 30FB-30FC --range 5186-5186 --range FF1A-FF1A

filesystem/Arial-small.font64: MKFONT_FLAGS+=-v --size 10 --outline 1.0 --char-spacing 1.0 --range 20-E9 --range 2026-2026 --range 25B6-25B7 --range 25BC-25BC --range 2640-2640 --range 2642-2642 --range 3002-3002 --range 300C-300F --range 3041-3089 --range 308B-308D --range 308F-308F --range 3092-309C --range 30A1-30D7 --range 30DB-30EF --range 30F2-30F4 --range 30FB-30FC --range 5186-5186 --range FF1A-FF1A

filesystem/logo-libdragon.sprite: MKSPRITE_FLAGS += -f RGBA32
filesystem/logo-bulbagarden.sprite: MKSPRITE_FLAGS += -f RGBA32
filesystem/logo-retrogamemechanicsexplained.sprite: MKSPRITE_FLAGS += -f RGBA32
filesystem/logo-datacrystal.sprite: MKSPRITE_FLAGS += -f RGBA32
filesystem/logo-pkhex.sprite: MKSPRITE_FLAGS += -f RGBA32
filesystem/logo-gbdevio.sprite: MKSPRITE_FLAGS += -f RGBA32
filesystem/logo-nesdevwiki.sprite: MKSPRITE_FLAGS += -f RGBA32
filesystem/menu-bg-9slice.sprite: MKSPRITE_FLAGS += -f RGBA16

pokemegb:
	$(MAKE) -C libpokemegb CC=$(N64_CC) CXX=$(N64_CXX) LD=$(N64_LD) PNG_SUPPORT=0 libpokemegb.a

$(BUILD_DIR)/PokeMe64.dfs: $(assets_conv)
$(BUILD_DIR)/PokeMe64.elf: $(OBJS) pokemegb

PokeMe64.z64: $(BUILD_DIR)/PokeMe64.dfs 

clean:
	$(MAKE) -C libpokemegb CC=$(N64_CC) CXX=$(N64_CXX) LD=$(N64_LD) PNG_SUPPORT=0 clean
	rm -rf $(BUILD_DIR) *.z64
	rm -rf filesystem/*
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
