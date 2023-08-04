
RC_ICONS = h2h.ico

QT += \
    widgets \
    sql \
    network \
    printsupport

FORMS += \
    FormChoice.ui \
    FormCorrection.ui \
    FormErrors.ui \
    FormMain.ui \
    FormSettings.ui \
    addOneSportsmen.ui \
    addcompetition.ui \
    mainwin.ui

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)

SOURCES += \
    Ball.cpp \
    FormMain.cpp \
    addcompetition.cpp \
    camera.cpp \
    cameraconnection.cpp \
    cameraviewer.cpp \
    choice.cpp \
    choice_one_athlete.cpp \
    choice_two_athletes.cpp \
    errors.cpp \
    fam_reg.cpp \
    firstround.cpp \
    firstround2.cpp \
    fix_result.cpp \
    formcorrection.cpp \
    main.cpp \
    mainwin.cpp \
    next_question.cpp \
    registration.cpp \
    settings.cpp \
    showreports.cpp \
    udp.cpp \
    xlxs.cpp

HEADERS += \
    Ball.h \
    FormMain.h \
    addcompetition.h \
    camera.h \
    cameraconnection.h \
    cameraviewer.h \
    choice.h \
    choice_one_athlete.h \
    choice_two_athletes.h \
    errors.h \
    fam_reg.h \
    firstround.h \
    firstround2.h \
    formcorrection.h \
    mainwin.h \
    registration.h \
    settings.h \
    showreports.h \
    udp.h \
    xlxs.h

DISTFILES += \
    images/circle.png \
    images/circle_green.png \
    images/close.png \
    images/open.png

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavformat

INCLUDEPATH += $$PWD/ffmpeg/include
DEPENDPATH += $$PWD/ffmpeg/include

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavutil

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavdevice

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavfilter

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lswscale

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lswresample


RESOURCES += \
    images.qrc

