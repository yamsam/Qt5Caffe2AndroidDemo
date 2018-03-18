#include "Caffe2VideoFilterRunnable.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include <QDateTime>
#include <QFile>
#include <QPainter>
#include <QPen>
#include <QFont>

#include "Caffe2VideoFilter.h"
#include "caffe2/core/init.h"
//#include "classes_jp.h"
#include "classes.h"

extern QImage qt_imageFromVideoFrame(const QVideoFrame& f);

Caffe2VideoFilterRunnable::Caffe2VideoFilterRunnable(Caffe2VideoFilter* parent) :
    m_Filter(parent),
    m_Orientation(0),
    m_Flip(0),
    m_result("test")
{
    initCaffe2();
}

QString Caffe2VideoFilterRunnable::result()
{
    return m_result;
}

void Caffe2VideoFilterRunnable::setResult(const QString &test)
{
    if (test == m_result)
        return;

    m_result = test;
}


QVideoFrame Caffe2VideoFilterRunnable::run(QVideoFrame *input,
                                           const QVideoSurfaceFormat &format,
                                           RunFlags flags)
{
    if (!input->isValid())
    {
        qWarning("Invalid input format");
        return *input;
    }

    m_Orientation = m_Filter ? m_Filter->property("orientation").toInt() : 0;

#ifdef Q_OS_ANDROID
    m_Flip = true;
#else
    m_Flip = format.scanLineDirection() == QVideoformat::BottomToTop;
#endif

    if (input->handleType() == QAbstractVideoBuffer::NoHandle)
    {
        QImage img = qt_imageFromVideoFrame(*input);

        qDebug() << Q_FUNC_INFO << "NoHandle";
        if (img.format() == QImage::Format_ARGB32)
        {
            return run(input, format, flags, img);
        }

        return run(input, format, flags, img.convertToFormat(QImage::Format_ARGB32));
    }

    if (input->handleType() == QAbstractVideoBuffer::GLTextureHandle)
    {
        QImage img(input->width(), input->height(), QImage::Format_ARGB32);
        GLuint textureId = input->handle().toInt();
        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions* f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLuint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, input->width(), input->height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        m_Flip = false;
        auto pixel = img.bits();
        for (int y = 0; y < img.height(); y++)
        {
            for (int x = 0; x < img.width(); x++)
            {
                auto T = pixel[0];
                pixel[0] = pixel[2];
                pixel[2] = T;
                pixel += 4;
            }
        }

        return run(input, format, flags, img);
    }

    qDebug() << Q_FUNC_INFO << "Unsupported handle type " << input->handleType();
    return *input;
}

QVideoFrame Caffe2VideoFilterRunnable::run(QVideoFrame* input,
                                           const QVideoSurfaceFormat &format,
                                           RunFlags flags, QImage image)
{
    Q_UNUSED(format)
    Q_UNUSED(flags)

    QImage small = image.scaledToHeight(IMG_H);

    QTransform myTransform;
    myTransform.rotate(90);
    small = small.transformed(myTransform);

    QString result = predict(small);

    m_Filter->setResult(result);
    if (!m_Filter->repeat()){
        m_Filter->setActive(false);
    }

    return !image.isNull() ? QVideoFrame(image) : *input;
}

// A function to load the NetDefs from protobufs.
void Caffe2VideoFilterRunnable::loadToNetDef(caffe2::NetDef* net, const char *filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray blob = file.readAll();
    const void* data = blob.data();
    size_t len = blob.size();

    assert(data != nullptr);

    assert(len != 0);

    if (!net->ParseFromArray(data, len)) {
        qDebug() << "Couldn't parse net from data.\n";
    }
}


void Caffe2VideoFilterRunnable::initCaffe2()
{
    qDebug() << "Attempting to load protobuf netdefs...";
    loadToNetDef(&_initNet,   ":/init_net.pb");
    loadToNetDef(&_predictNet,":/predict_net.pb");
    qDebug() << "done.";
    qDebug() << "Instantiating predictor...";
    _predictor = new caffe2::Predictor(_initNet, _predictNet);
    qDebug() << "done.";
}


QString Caffe2VideoFilterRunnable::predict(QImage& img)
{
    bool infer_HWC = false;

    if (!_predictor) {
        return "Loading...";
    }

    auto w = img.width();
    auto h = img.height();

#define min(a,b) ((a) > (b)) ? (b) : (a)
#define max(a,b) ((a) > (b)) ? (a) : (b)

    auto h_offset = max(0, (h - IMG_H) / 2);
    auto w_offset = max(0, (w - IMG_W) / 2);

    auto iter_h = IMG_H;
    auto iter_w = IMG_W;

    if (h < IMG_H) {
        iter_h = h;
    }
    if (w < IMG_W) {
        iter_w = w;
    }

    QRgb rgb;
    for (auto i = 0; i < iter_h; ++i) {
        auto hi = h_offset + i;

        for (auto j = 0; j < iter_w; ++j) {
            // Tested on Pixel and S7.
            auto wi = w_offset + j;

            auto b_i = 0 * IMG_H * IMG_W + i * IMG_W + j;
            auto g_i = 1 * IMG_H * IMG_W + i * IMG_W + j;
            auto r_i = 2 * IMG_H * IMG_W + i * IMG_W + j;

            if (infer_HWC) {
                b_i = (j * IMG_W + i) * IMG_C;
                g_i = (j * IMG_W + i) * IMG_C + 1;
                r_i = (j * IMG_W + i) * IMG_C + 2;
            }

            float b_mean = 104.00698793f;
            float g_mean = 116.66876762f;
            float r_mean = 122.67891434f;

            rgb = img.pixel(wi, hi);

            input_data[r_i] = (float)qRed(rgb)   - r_mean;
            input_data[g_i] = (float)qGreen(rgb) - g_mean;
            input_data[b_i] = (float)qBlue(rgb)  - b_mean;
        }
    }

    caffe2::TensorCPU input;
    if (infer_HWC) {
        input.Resize(std::vector<int>({IMG_H, IMG_W, IMG_C}));
    } else {
        input.Resize(std::vector<int>({1, IMG_C, IMG_H, IMG_W}));
    }
    memcpy(input.mutable_data<float>(), input_data, IMG_H * IMG_W * IMG_C * sizeof(float));
    caffe2::Predictor::TensorVector input_vec{&input};
    caffe2::Predictor::TensorVector output_vec;
    caffe2::Timer t;
    t.Start();
    _predictor->run(input_vec, &output_vec);
    float fps = 1000 / t.MilliSeconds();

    constexpr int k = 3;
    float max[k] = {0};
    int max_index[k] = {0};

    // Find the top-k results manually.
    if (output_vec.capacity() > 0) {
        for (auto output : output_vec) {
            for (auto i = 0; i < output->size(); ++i) {
                for (auto j = 0; j < k; ++j) {
                    if (output->data<float>()[i] > max[j]) {
                        for (auto _j = k - 1; _j > j; --_j) {
                            max[_j] = max[_j-1];
                            max_index[_j] = max_index[_j-1];
                        }
                        max[j] = output->template data<float>()[i];
                        max_index[j] = i;
                        goto skip;
                    }
                }
                skip:;
            }
        }
    }

    QString message;
    message += QString::number(fps) + " FPS\n";
    for (auto j = 0; j < k; ++j) {
       message += QString::number(j) + ":" + imagenet_classes[max_index[j]]
               + " - " + QString::number(max[j]*100) + "%\n";
    }
    return message;
}
