
RC_ICONS = h2h.ico

QT += \
    widgets \
    sql \
    network \
    printsupport \
    multimedia

FORMS += \
    FormChoice.ui \
    FormCorrection.ui \
    FormErrors.ui \
    FormMain.ui \
    FormSettings.ui \
    addOneSportsmen.ui \
    addcompetition.ui \
    dlgsemifinal.ui \
    mainwin.ui

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
    dlgsemifinal.cpp \
    errors.cpp \
    fam_reg.cpp \
    firstround.cpp \
    firstround2.cpp \
    fix_result.cpp \
    formcorrection.cpp \
    main.cpp \
    mainwin.cpp \
    next_question.cpp \
    player.cpp \
    playerviewer.cpp \
    protocol.cpp \
    registration.cpp \
    settings.cpp \
    showreports.cpp \
    udp.cpp

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
    dlgsemifinal.h \
    errors.h \
    fam_reg.h \
    firstround.h \
    firstround2.h \
    formcorrection.h \
    mainwin.h \
    player.h \
    playerviewer.h \
    protocol.h \
    registration.h \
    settings.h \
    showreports.h \
    udp.h

DISTFILES += \
    images/change.png \
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

