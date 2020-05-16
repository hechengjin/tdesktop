#include "productactivityapi.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
QProductActivityAPI::QProductActivityAPI()
{

}

QProductActivityAPI::~QProductActivityAPI()
{

}

void QProductActivityAPI::pushActivityInfo(ProductActivityStruct pas)
{
    QString postUrl = "https://www.chunhuitech.cn/api/admin/api/prodactivity/report";
    QJsonObject json;
    json.insert("userId", QString::number(pas.userId));
    json.insert("clientFlag", pas.clientFlag);
    json.insert("procName", pas.procName);
    json.insert("procVersion", pas.procVersion);
    json.insert("procId", QString::number(pas.procId));
    json.insert("os", pas.os);
    json.insert("eventName", pas.eventName);
    json.insert("ip", pas.ip);
    json.insert("netIp", pas.netIp);
    json.insert("area", pas.area);
    json.insert("modifyTime", pas.modifyTime);
    json.insert("createTime", pas.createTime);
    json.insert("remarks", pas.remarks);
    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);
    post(postUrl, dataArray);
}


void QProductActivityAPI::requestFinished(QNetworkReply *reply, const QByteArray data, const int statusCode)
{
    Q_UNUSED(reply);
    int code = EC_UNKNOWN;
    QString msg = "Unknown error";
    if (statusCode == 200) {
        QJsonParseError jsonError;
        QJsonDocument resJsonDoc = QJsonDocument::fromJson(data, &jsonError);
        if (jsonError.error == QJsonParseError::NoError) {
            if(resJsonDoc.isObject()){
//                VersionInfoStruct vis;
                QVariantMap result = resJsonDoc.toVariant().toMap();
                code = result["code"].toInt();
                msg = result["msg"].toString();
                QString info = QString("start up finish! code: %1  msg: %2").arg(QString::number(code)).arg(msg);
            }
        }
    }
}
