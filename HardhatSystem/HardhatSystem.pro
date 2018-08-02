QT += quick qml widgets concurrent multimedia sql
CONFIG += c++11


include( $$PWD/library/JQToolsLibrary/JQToolsLibrary.pri )
include( $$PWD/library/MaterialUI/MaterialUI.pri )
include( $$PWD/components/components.pri )

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/cpp/
unix:INCLUDEPATH += /usr/local/include/
win32:INCLUDEPATH += C:/opencv_3.0/opencv/build/include/

HEADERS += \
    $$PWD/cpp/jqtools_manage.hpp \
    $$PWD/cpp/yolo_v2_class.hpp \
    $$PWD/cpp/utils.h \
    $$PWD/cpp/bettervideocapture.hpp \
    cpp/dataface.h

SOURCES += \
    $$PWD/cpp/main.cpp \
    cpp/utils.cpp \
    cpp/dataface.cpp

RESOURCES += \
    $$PWD/qml/qml.qrc

unix:LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio
unix:LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui
unix:LIBS += -L$$PWD/cpp -ldarknet
win32:LIBS += C:/opencv_3.0/opencv/build/x64/vc14/lib/opencv_world340.lib
win32:LIBS += $$PWD/cpp/yolo_cpp_dll.lib

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
