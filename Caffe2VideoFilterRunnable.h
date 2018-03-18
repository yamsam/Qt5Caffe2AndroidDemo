#ifndef Caffe2VideoFilterRunnable_H
#define Caffe2VideoFilterRunnable_H

#include <QVideoFilterRunnable>
#include <QVideoFrame>

#define PROTOBUF_USE_DLLS 1
#define CAFFE2_USE_LITE_PROTO 1
#include <caffe2/core/predictor.h>
#include <caffe2/core/operator.h>
#include <caffe2/core/timer.h>

#define IMG_H 227
#define IMG_W 227
#define IMG_C 3
#define MAX_DATA_SIZE IMG_H * IMG_W * IMG_C

class Caffe2VideoFilter;

class Caffe2VideoFilterRunnable : public QVideoFilterRunnable
{

public:
    Caffe2VideoFilterRunnable(Caffe2VideoFilter* parent = nullptr);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &format, RunFlags flags) Q_DECL_OVERRIDE;
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &format, RunFlags flags, QImage image);

    void loadToNetDef(caffe2::NetDef* net, const char *filename);
    void initCaffe2();

    QString predict(QImage& img);

    QString result();
    void    setResult(const QString &test);

protected:
    Caffe2VideoFilter* m_Filter;
    int m_Orientation;
    int m_Flip;
    QString m_result;

    caffe2::NetDef _initNet, _predictNet;
    caffe2::Predictor *_predictor;
    caffe2::Workspace _caffe_ws;

    char raw_data[MAX_DATA_SIZE];
    float input_data[MAX_DATA_SIZE];
};

#endif
