#ifndef QPUSHACTIVEINFO_H
#define QPUSHACTIVEINFO_H

#include <QObject>
#include <QMutexLocker>
#include <QAtomicPointer>
#include <QDebug>

struct IpInfoStruct
{
    QString netIp;
    QString areaInfo;
};



class QPushActiveInfo : public QObject
{
    Q_OBJECT
public:
    static QPushActiveInfo &instance(void)
        {
    #ifdef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
            if(!QAtomicPointer<QPushActiveInfo>::isTestAndSetNative()) {
                qDebug() << "Error: don's support TestAndSetNative!!!!!!QNetSo";
            }

    #endif
            if(m_pInstance.testAndSetOrdered(0,0)){
                QMutexLocker locker(&m_Mutex);
                m_pInstance.testAndSetOrdered(0, new QPushActiveInfo);
            }
            return * m_pInstance;
        }
public:
    QString gethostMac();
    QString gethostIp();
    bool IPLive(QString ip, int port);
    QString getOSInfo();
    IpInfoStruct GetNetIPInfo();
private:
    QByteArray GetHtml(QString url);
    QString GetNetIPPconline(QByteArray responseData);
    QString GetAttributionPconline(QByteArray responseData);

private:
    explicit QPushActiveInfo(QObject *parent = nullptr);
    QPushActiveInfo(const QPushActiveInfo &);
    QPushActiveInfo & operator=(const QPushActiveInfo &);
    static QMutex m_Mutex;
    static QAtomicPointer<QPushActiveInfo> m_pInstance;
};

#endif // QPUSHACTIVEINFO_H
