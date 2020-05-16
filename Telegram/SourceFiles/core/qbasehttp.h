#ifndef QBASEHTTP_H
#define QBASEHTTP_H

#include <QObject>
//#include <QNetworkRequest>
#include <QtNetwork\QNetworkReply>
//#include <QNetworkAccessManager>


class QBaseHttp : public QObject
{
    Q_OBJECT
public:
    explicit QBaseHttp(QObject *parent = nullptr);
    ~QBaseHttp();
    void get(const QString url);
    void post(const QString url, const QByteArray &data);
protected:
    virtual void requestFinished(QNetworkReply *reply, const QByteArray data, const int statusCode) = 0;

public slots:
    void serviceRequestFinished(QNetworkReply *reply);

private:
    QNetworkRequest httpRequest;
    QNetworkAccessManager networkAccessManager;
};

#endif // QBASEHTTP_H
