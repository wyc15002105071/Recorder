QT       += core gui multimedia multimediawidgets serialport
 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += common media

message($(QMAKE_SYSROOT))

SOURCES += \
    allocators/allocator.cpp \
    allocators/allocator_service.cpp \
    allocators/drm_allocator.cpp \
    common/log.cpp \
    common/observer.cpp \
    listeners/key_listener.cpp \
    main.cpp \
    main_task.cpp \
    main_widget.cpp \
    common/rthread.cpp \
    media/live555/h264livevideoservermediasubsession.cpp \
    media/live555/h264livevideosource.cpp \
    media/live555/rtspserver.cpp \
    media/mediamuxer.cpp \
    media/mediarecorder.cpp \
    media/mediautils.cpp \
    media/rkmpp_dec_api.cpp \
    media/rkmpp_enc_api.cpp \
    media/rkrgadef.cpp \
    media/streampusher.cpp \
    media/videoframereader.cpp \
    media/videoinputdevice.cpp \
    utils/fileutils.cpp \
    utils/networkutils.cpp \
    utils/serialportutils.cpp \
    utils/storageutils.cpp \
    utils/thumbnailutils.cpp \
    widgets/basewidget.cpp \
    widgets/menuwidget.cpp \
    widgets/pushstreamwidget.cpp \
    widgets/recordwidget.cpp \
    widgets/setting_widget.cpp \
    widgets/viewer/baseviewer.cpp \
    widgets/viewer/diskselectionitem.cpp \
    widgets/viewer/diskselectionwidget.cpp \
    widgets/viewer/imagebrowser.cpp \
    widgets/viewer/imageviewer.cpp \
    widgets/record_timer_widget.cpp \
    widgets/videowidget.cpp \
    widgets/viewer/listwidgetitem.cpp \
    widgets/viewer/progressviewer.cpp \
    widgets/viewer/selectionicon.cpp \
    widgets/viewer/videoplayer.cpp \
    widgets/viewer/videoviewer.cpp

HEADERS += \
    allocators/allocator.h \
    allocators/allocator_service.h \
    allocators/drm_allocator.h \
    common/Mutex.h \
    common/common.h \
    common/log.h \
    common/observer.h \
    listeners/key_listener.h \
    main_task.h \
    main_widget.h \
    common/rthread.h \
    media/live555/h264livevideoservermediasubsession.h \
    media/live555/h264livevideosource.h \
    media/live555/rtspserver.h \
    media/mediamuxer.h \
    media/mediarecorder.h \
    media/mediautils.h \
    media/rkmpp_dec_api.h \
    media/rkmpp_enc_api.h \
    media/rkrgadef.h \
    media/streampusher.h \
    media/videoframereader.h \
    media/videoinputdevice.h \
    utils/fileutils.h \
    utils/networkutils.h \
    utils/serialportutils.h \
    utils/storageutils.h \
    utils/thumbnailutils.h \
    widgets/basewidget.h \
    widgets/menuwidget.h \
    widgets/pushstreamwidget.h \
    widgets/recordwidget.h \
    widgets/setting_widget.h \
    widgets/viewer/baseviewer.h \
    widgets/viewer/diskselectionitem.h \
    widgets/viewer/diskselectionwidget.h \
    widgets/viewer/imagebrowser.h \
    widgets/viewer/imageviewer.h \
    widgets/record_timer_widget.h \
    widgets/videowidget.h \
    widgets/viewer/listwidgetitem.h \
    widgets/viewer/progressviewer.h \
    widgets/viewer/selectionicon.h \
    widgets/viewer/videoplayer.h \
    widgets/viewer/videoviewer.h

FORMS += \
    mainwidget.ui \
    widgets/basewidget.ui \
    widgets/menuwidget.ui \
    widgets/pushstreamwidget.ui \
    widgets/recordwidget.ui \
    widgets/settingwidget.ui \
    widgets/viewer/diskselectionitem.ui \
    widgets/viewer/diskselectionwidget.ui \
    widgets/viewer/imagebrowser.ui \
    widgets/viewer/imageviewer.ui \
    widgets/recordtimerwidget.ui \
    widgets/viewer/listwidgetitem.ui \
    widgets/viewer/progressviewer.ui \
    widgets/viewer/selectionicon.ui \
    widgets/viewer/videoplayer.ui \
    widgets/viewer/videoviewer.ui

INCLUDEPATH += $$PWD/third-party/headers/mpp \
               $$PWD/third-party/headers/drm \
               $$PWD/third-party/headers/ffmpeg \
               $$PWD/third-party/headers/rga

INCLUDEPATH += $$PWD/third-party/headers/live555 \
               $$PWD/third-party/headers/live555/liveMedia \
               $$PWD/third-party/headers/live555/BasicUsageEnvironment \
               $$PWD/third-party/headers/live555/groupsock \
               $$PWD/third-party/headers/live555/UsageEnvironment

LIBS += -ludev \
        -L$$PWD/third-party/prebuilt/linux_arm32 \
        -ldrm -lrockchip_mpp -lrockchip_vpu -lrga \
        -lliveMedia -lgroupsock -lBasicUsageEnvironment -lUsageEnvironment \
        -lavformat -lavcodec -lavutil -lswscale -lswresample


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
