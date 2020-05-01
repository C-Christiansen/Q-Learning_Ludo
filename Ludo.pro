TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt



SOURCES += main.cpp \
    game.cpp \
    test_game.cpp \
    AI_player.cpp

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
    AI_player.h
