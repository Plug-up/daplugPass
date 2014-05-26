QT += widgets\

SOURCES += \
    main.cpp \
    DaplugPassGui.cpp \
    DaplugPass.c

HEADERS += \
    DaplugPassGui.h \
    DaplugPass.h

LIBS += -L$$PWD/libs/WIN32/C-DAPLUG-API-WIN32-1.1-alpha/bin/ -ldaplug \
        -L$$PWD/libs/WIN32/OpenSSL/bin/ -leay32

INCLUDEPATH += $$PWD/libs/WIN32/C-DAPLUG-API-WIN32-1.1-alpha/include \
               $$PWD/libs/WIN32/OpenSSL/include

DEPENDPATH += $$PWD/libs/WIN32/C-DAPLUG-API-WIN32-1.1-alpha/include \
              $$PWD/libs/WIN32/OpenSSL/include

RESOURCES += \
    Ressources.qrc

RC_FILE = daplugpass.rc

