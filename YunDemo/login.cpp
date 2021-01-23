#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include "common/common.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //去掉创建的边框
    this->setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    //设置当前窗口所有的字体
    this->setFont(QFont("宋体",10,QFont::Bold,false));

    //titlewidget信号处理
    connect(ui->title_wg,&Title::closeWindow,[=]()
    {
        //判断当前stackedWidget显示的页面
        if(ui->stackedWidget->currentWidget()==ui->set_page)
        {
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            ui->lineEdit_9->clear();
            ui->lineEdit_10->clear();
        }
        else if(ui->stackedWidget->currentWidget()==ui->reg_page)
        {
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            ui->lineEdit_3->clear();
            ui->lineEdit_4->clear();
            ui->lineEdit_5->clear();
            ui->lineEdit_6->clear();
            ui->lineEdit_7->clear();
            ui->lineEdit_8->clear();
        }
        else
        {
            this->close();
        }
    });
    connect(ui->title_wg,&Title::showSetWg,[=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->set_page);
    });

    connect(ui->toolButton,&QToolButton::clicked,[=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->reg_page);
    });
}

Login::~Login()
{
    delete ui;
}

void Login::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //给窗口画背景图
    QPainter p(this);
    QPixmap pixmap(":/upload/images/20190923102909_hfvny.jpg");
    p.drawPixmap(0,0,this->width(),this->height(),pixmap);
}
//注册按钮
void Login::on_reg_btn_clicked()
{
    //取数据
    QString ip = ui->lineEdit_9->text();
    QString port = ui->lineEdit_10->text();
    //注册数据
    QString name = ui->lineEdit_3->text();
    QString nick = ui->lineEdit_4->text();
    QString passwd = ui->lineEdit_5->text();
    QString confirm_pwd = ui->lineEdit_6->text();
    QString phone = ui->lineEdit_7->text();
    QString email = ui->lineEdit_8->text();

    //校验
    //参考server设置位置的代码

    QByteArray postData = getRegJson(name,nick,passwd,phone,email);
    //发送http的请求协议
    QNetworkAccessManager* manager=Common::getNetManager();
    //http请求头
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
    //url

    QString url = QString("http://%1:%2/reg").arg(ip).arg(port);
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->post(request,postData);

    //接收server返回的
    connect(reply,&QNetworkReply::readyRead,[=]()
    {
           //都返回数据
        QByteArray jsonData = reply->readAll();
        //解析json字符串
       QString status = m_cm.getCode(jsonData);
        //得到一个数=QString status
        if("002"==status)
        {
            //success
            //将当前注册的用户信息填入到登陆的输入中
            ui->lineEdit->setText(name);
            ui->lineEdit_2->setText(passwd);
            //当前注册信息清除

            //跳转到登录界面
            ui->stackedWidget->setCurrentWidget(ui->login_page);
        }
        else if("003"==status)
        {
            //该用户已存在
            QMessageBox::warning(this,"警告","当前注册的用户已存在");
            return;
        }
        else if("004"==status)
        {
            QMessageBox::critical(this,"警告","注册失败");
            return;
        }
     });
}
//服务器设置
void Login::on_pushButton_2_clicked()
{
    //获取控件数据
    QString ip= ui->lineEdit_9->text();
    QString port =ui->lineEdit_10->text();
    //数据的校验
    QRegExp exp(IP_REG);
    if(!exp.exactMatch(ip))
    {
        QMessageBox::warning(this,"警告","ip格式不正确");
        ui->lineEdit_9->clear();
        //设置焦点
        ui->lineEdit_9->setFocus();
        return;
    }
    exp.setPattern(PORT_REG);
    if(!exp.exactMatch(port))
    {
        QMessageBox::warning(this,"警告","port格式不正确");
        ui->lineEdit_10->clear();
        //设置焦点
        ui->lineEdit_10->setFocus();
        return;
   }
    //跳转到登录界面
    ui->stackedWidget->setCurrentWidget(ui->login_page);

}

QByteArray Login::getRegJson(QString user, QString nick, QString pwd, QString phone, QString mail)
{
    QJsonObject obj;
    obj.insert("username",user);
    obj.insert("nickname",nick);
    obj.insert("firstPwd",pwd);
    obj.insert("phone",phone);
    obj.insert("email",mail);

    QJsonDocument doc=QJsonDocument(obj);
    return doc.toJson();
}

//保存配置文件信息
void Login::saveWebInfo(QString ip,QString port,QString path)
{
    //先读文件
    QFile file(path);
    //判断文件是否存在
    bool bl=file.open(QFile::ReadOnly);
    if(bl==false)
    {
        //文件不存在
        //写一个新文件

        return;
    }
    //文件存在
    QByteArray data = file.readAll();
    //先读原来的配置文件
    QJsonDocument doc=QJsonDocument::fromJson(data);
    if(!doc.isObject())
    {
        return;
    }
    //取出login信息
    QJsonObject obj = doc.object();
    //取出login信息
    QJsonValue  loginobj = obj.value("login");
//    if(loginobj.isObject())
//    {
//        QJsonObject subobj = value.toObject();
//        QString pwd = subobj.value("pwd").toString();
//        QString remember = subobj.value("remember").toString();
//        QString remember = subobj.value("user").toString();
//        qDebug()<<ip<<" "<<port;
//    }
    QMap<QString,QVariant>logininfo;
    logininfo.insert("pwd",QVariant(ip));
    logininfo.insert("user",QVariant(port));
    logininfo.insert("remember",QVariant(port));
    //取出图片路径
    QJsonValue  pathobj = obj.value("login");
    QMap<QString,QVariant>pathinfo;
    pathinfo.insert("path","XXX");
    //存储web信息
    QMap<QString,QVariant>webinfo;
    webinfo.insert("ip",QVariant(ip));
    webinfo.insert("port",QVariant(port));

    QMap<QString,QVariant>info;
    info.insert("type_path",pathinfo);
    info.insert("login",logininfo);
    info.insert("web_server",webinfo);

    doc =QJsonDocument::fromVariant(info);
    //转成字符串写到文件中去
    data = doc.toJson();
    //写文件

}

void Login::on_toolButton_triggered(QAction *arg1)
{
    return ;
}
