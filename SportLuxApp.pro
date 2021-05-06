QT       += core gui sql uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DatabaseClass/DatabaseWorks.cpp \
    ValidatorClass/LogPassValidator.cpp \
    WindowsClass/OptionsWindow.cpp \
    WindowsClass/SignInWindow.cpp \
    WindowsClass/SignUpWindow.cpp \
    WindowsClass/TableActionsClass/ManifestForm.cpp \
    WindowsClass/TableActionsClass/ProductDialog.cpp \
    WindowsClass/TableActionsClass/RecordDialog.cpp \
    WindowsClass/TableWindow.cpp \
    main.cpp

HEADERS += \
    DatabaseClass/DatabaseWorks.h \
    Enums.h \
    ValidatorClass/LogPassValidator.h \
    WindowsClass/OptionsWindow.h \
    WindowsClass/SignInWindow.h \
    WindowsClass/SignUpWindow.h \
    WindowsClass/TableActionsClass/ManifestForm.h \
    WindowsClass/TableActionsClass/ProductDialog.h \
    WindowsClass/TableActionsClass/RecordDialog.h \
    WindowsClass/TableWindow.h

FORMS += \
    WindowsClass/OptionsWindow.ui \
    WindowsClass/SignInWindow.ui \
    WindowsClass/SignUpWindow.ui \
    WindowsClass/TableActionsClass/ManifestForm.ui \
    WindowsClass/TableActionsClass/ProductDialog.ui \
    WindowsClass/TableActionsClass/RecordDialog.ui \
    WindowsClass/TableWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
