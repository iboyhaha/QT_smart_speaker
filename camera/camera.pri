
TARGET_ARCH = $${QT_ARCH}
contains(TARGET_ARCH, arm){
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    INCLUDEPATH += /opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/usr/include
} else {
    LIBS += -L/usr/local/lib -L/opt/opencv_4/lib \
            -lopencv_world
#            -lopencv_core \
#            -lopencv_highgui \
#            -lopencv_imgproc \
#            -lopencv_videoio \
#            -lopencv_imgcodecs

    #INCLUDEPATH可写可不写，系统会到找到此路径
    INCLUDEPATH += /usr/local/include
}
HEADERS += \
    $$PWD/camera.h

SOURCES += \
    $$PWD/camera.cpp
