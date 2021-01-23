#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "common/common.h"
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
protected:
    //绘图事件函数
    void paintEvent(QPaintEvent *event);
private slots:

    void on_reg_btn_clicked();

    void on_toolButton_triggered(QAction *arg1);

    void on_pushButton_2_clicked();
    //保存配置文件信息
    void saveWebInfo(QString ip,QString port,QString path);
    QByteArray getRegJson(QString user,QString nick, QString pwd, QString phone,QString mail);

private:
    Ui::Login *ui;
    Common m_cm;
};


#endif // LOGIN_H
