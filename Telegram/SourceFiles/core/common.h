#ifndef COMMON_H
#define COMMON_H
#include <QObject>

#define VERSION "2.0.1"
#define VERSION_NUM "20001"
#define APP_NAME "Telegram"
#define APP_ID "7"
#define APP_PLAT_FORM_WIN_32 "Qt_Windows_32bit"
#define APP_PLAT_FORM_WIN_64 "Qt_Windows_64bit"
#define EVENT_START_UP    "start-up"

typedef enum{
    UI_EN,
    UI_ZH
}LANGUAGE;

enum ErrorCode
{
    EC_SUCCESS = 0,
    EC_ERROR = 400000,
    EC_UNKNOWN = 900000,
    EC_DB = 900002
};

struct ProductActivityStruct
{
    int userId;
    QString clientFlag;
    QString procName;
    QString procVersion;
    int procId; //产品id
    QString os;
    QString eventName;
    QString ip;
    QString netIp;
    QString area;
    QString modifyTime;
    QString createTime;
    QString remarks;
};
Q_DECLARE_METATYPE(ProductActivityStruct)
#endif // COMMON_H
