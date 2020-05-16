#ifndef QPRODUCTACTIVITYAPI_H
#define QPRODUCTACTIVITYAPI_H

#include <QObject>
#include "qbasehttp.h"
#include "common.h"

class QProductActivityAPI : public QBaseHttp
{
public:
    QProductActivityAPI();
    ~QProductActivityAPI();
public:
    void pushActivityInfo(ProductActivityStruct pas);
protected:
    void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);
};

#endif // QPRODUCTACTIVITYAPI_H
