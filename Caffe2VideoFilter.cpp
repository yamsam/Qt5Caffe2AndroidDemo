#include "Caffe2VideoFilter.h"
#include "Caffe2VideoFilterRunnable.h"

Caffe2VideoFilter::Caffe2VideoFilter(QObject* parent) :
    QAbstractVideoFilter(parent),
    m_Orientation(0),
    m_Repeat(false)
{
}

QVideoFilterRunnable* Caffe2VideoFilter::createFilterRunnable()
{
    return new Caffe2VideoFilterRunnable(this);
}

QString Caffe2VideoFilter::result(){
    return m_Result;
}

void Caffe2VideoFilter::setResult(const QString &str)
{
    if (str== m_Result)
        return;

    m_Result = str;
    emit resultChanged();
}

bool Caffe2VideoFilter::repeat()
{
    return m_Repeat;
}

void Caffe2VideoFilter::setRepeat(const bool &value)
{
    m_Repeat = value;
}
