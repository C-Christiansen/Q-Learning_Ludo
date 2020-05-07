TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt



SOURCES += main.cpp \
    game.cpp \
    test_game.cpp \
    AI_player.cpp \
    move_logic.cpp

SUBDIRS += \
    Ludo.pro

DISTFILES += \
    Ludo.pro.user

HEADERS += \
    dice.h \
    game.h \
    iplayer.h \
    player_random.h \
    positions_and_dice.h \
    test_game.h \
    AI_player.h \
    player_fast.h \
    move_logic.h \
    player_random_safe.h \
    player_aggro_fast.h
