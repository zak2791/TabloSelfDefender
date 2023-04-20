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
    FormSettings.ui

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)

SOURCES += \
    Ball.cpp \
    FormMain.cpp \
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
    next_question.cpp \
    registration.cpp \
    settings.cpp \
    showreports.cpp \
    udp.cpp \
    xlxs.cpp

HEADERS += \
    Ball.h \
    FormMain.h \
    choice.h \
    choice_one_athlete.h \
    choice_two_athletes.h \
    errors.h \
    fam_reg.h \
    firstround.h \
    firstround2.h \
    formcorrection.h \
    registration.h \
    settings.h \
    showreports.h \
    udp.h \
    xlxs.h

