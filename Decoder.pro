#TEMPLATE = app
TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

TARGET = l16_decoder

SOURCES += \
    basedataelement.cpp \
    basemessage.cpp \
    basecodeword.cpp \
    #main.cpp \
    l16_decoder.cpp


INCLUDEPATH += /dlip_ref/s_dlip/Tools/boost_library/boost-1.65/include/

HEADERS += \
    basedataelement.h \
    dataelements.h \
    j3_3.h \
    j12_6.h \
    l16_ed7_messages.h \
    basemessage.h \
    basecodeword.h \
    json.hpp \
    l16_decoder.h \
    messages.h

DISTFILES += \
    make_data_elements.py
