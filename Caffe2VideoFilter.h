#ifndef Caffe2VideoFilter_H
#define Caffe2VideoFilter_H

#include <QAbstractVideoFilter>

class Caffe2VideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY(int orientation MEMBER m_Orientation)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

public:
    Caffe2VideoFilter(QObject* parent = nullptr);

    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

    QString result();
    void setResult(const QString&);

    bool repeat();
    void setRepeat(const bool&);


signals:
    void resultChanged();
    void repeatChanged();

protected:
    int m_Orientation;
    QString m_Result;
    bool m_Repeat;
};

#endif
