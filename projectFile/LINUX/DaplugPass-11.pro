QT += widgets\

SOURCES += \
    main.cpp \
    DaplugPassGui.cpp \
    DaplugPass.c

HEADERS += \
    DaplugPassGui.h \
    DaplugPass.h

unix:!macx: LIBS += -L$$PWD/libs/LINUX/C-DAPLUG-API-LINUX64-1.1-alpha/bin/ -ldaplug

INCLUDEPATH += $$PWD/libs/LINUX/C-DAPLUG-API-LINUX64-1.1-alpha/include
DEPENDPATH += $$PWD/libs/LINUX/C-DAPLUG-API-LINUX64-1.1-alpha/include

unix:!macx: LIBS += -lcrypto

RESOURCES += \
    Ressources.qrc

RC_FILE = daplugpass.rc
