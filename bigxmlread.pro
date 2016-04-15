HEADERS       = bigxmlmainwindow.h \
                bigxmlreader.h \
    xqueryform.h
SOURCES       = bigxmlmain.cpp \
                bigxmlmainwindow.cpp \
                bigxmlreader.cpp \
    xqueryform.cpp
QT           += xml

win32 {
    QMAKE_LFLAGS += -static-libgcc
}

FORMS += \
    xqueryform.ui
