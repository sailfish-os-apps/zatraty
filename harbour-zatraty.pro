TARGET = harbour-zatraty

CONFIG += sailfishapp c++11
QT += sql concurrent

SOURCES += src/harbour-zatraty.cpp \
    src/category.cpp \
    src/database.cpp \
    src/expense.cpp \
    src/settings.cpp \
    src/models/categorylistmodel.cpp \
    src/models/datelistmodel.cpp \
    src/models/expenselistmodel.cpp \
    src/models/categorymodel.cpp \
    src/models/expensemodel.cpp

HEADERS += \
    src/category.h \
    src/database.h \
    src/expense.h \
    src/settings.h \
    src/models/categorylistmodel.h \
    src/models/datelistmodel.h \
    src/models/expenselistmodel.h \
    src/models/categorymodel.h \
    src/models/expensemodel.h

INCLUDEPATH += src \
    src/models

OTHER_FILES += qml/harbour-zatraty.qml \
    qml/cover/*.qml \
    qml/JS/*.js \
    qml/components/*.qml \
    qml/pages/*.qml \
    rpm/* \
    translations/harbour-zatraty*.ts \
    harbour-zatraty.desktop

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-zatraty.de.ts \
                translations/harbour-zatraty.it.ts \
                translations/harbour-zatraty.sv.ts \
                translations/harbour-zatraty.ru.ts

