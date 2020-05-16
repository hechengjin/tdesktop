#include "qbasehttp.h"
#include <QThread>
#include <QUrl>
QBaseHttp::QBaseHttp(QObject *parent) : QObject(parent)
{
    httpRequest.setRawHeader("Content-Type", "application/json");
}

QBaseHttp::~QBaseHttp()
{
    networkAccessManager.disconnect();
}

void QBaseHttp::get(const QString url)
{
    httpRequest.setUrl(QUrl(url));
    networkAccessManager.get(httpRequest);
}

void QBaseHttp::post(const QString url, const QByteArray &data)
{
//    qDebug() << "BaseHttp::post thread Id:" << QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));
//    QLoggingLib::instance().info("BaseHttp::post thread Id:" + QString::number((uint64_t)QThread::currentThreadId()), LMV_NET);
//    QLoggingLib::instance().info("BaseHttp::post thread: + " + QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0')));
    httpRequest.setUrl(QUrl(url));
    networkAccessManager.post(httpRequest, data);
}

void QBaseHttp::serviceRequestFinished(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "BaseHttp::serviceRequestFinished thread Id:" << QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));
//    QLoggingLib::instance().info("BaseHttp::serviceRequestFinished thread Id:" + QString::number((uint64_t)QThread::currentThreadId()), LMV_NET);
//    QLoggingLib::instance().info("BaseHttp::serviceRequestFinished thread: + " + QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0')));
//    qDebug() << "BaseAPI...serviceRequestFinished...statusCode:" << statusCode;

    if(reply->error() == QNetworkReply::NoError) {
        requestFinished(reply, reply->readAll(), statusCode);
    } else {
        requestFinished(reply, "", statusCode);
    }

    // At the end of that slot, we won't need it anymore
    reply->deleteLater();
}
