CFLAGS		+= -I./sources/app/game/game_cat_dog
CPPFLAGS	+= -I./sources/app/game/game_cat_dog

VPATH += sources/app/game/game_cat_dog

# CPP source files
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_match.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_cat.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_dog.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_border.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_wind.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_power.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_missile.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_link.cpp
SOURCES_CPP += sources/app/game/game_cat_dog/cd_game_cpu.cpp
