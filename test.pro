TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fun.cpp

HEADERS += \
    fun.h \
    def.h \
    src/jthreadconfig.h \
    src/jmutex.h \
    src/jmutexautolock.h \
    src/jthread.h

OTHER_FILES +=


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/src/release/ -ljthread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/src/debug/ -ljthread
else:unix: LIBS += -L$$PWD/src/ -ljthread

INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/src/release/libjthread.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/src/debug/libjthread.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/src/release/jthread.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/src/debug/jthread.lib
else:unix: PRE_TARGETDEPS += $$PWD/src/libjthread.a

unix|win32: LIBS += -laep -L/usr/lib/openssl/engines -lrt
