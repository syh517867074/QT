#include "title.h"
#include "ui_title.h"
#include <QMouseEvent>

Title::Title(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Title)
{
    ui->setupUi(this);
    //logo图片
    ui->logo->setPixmap(QPixmap(":/upload/images/logo.ico").scaled(40,40));
    //ui->logo->setPixmap(":/upload/images/logo.ico");
    ui->wgtitle->setStyleSheet("color:rgb(255,255,255)");
    m_parent = parent;

    //按钮功能实现
    connect(ui->set,&QToolButton::clicked,[=]()
    {
          emit showSetWg();
    });
    connect(ui->min,&QToolButton::clicked,[=]()
    {
        m_parent->showMinimized();
    });
    connect(ui->close,&QToolButton::clicked,[=]()
    {
        emit closeWindow();
    });
}

Title::~Title()
{
    delete ui;
}

void Title::mouseMoveEvent(QMouseEvent *event)
{
    //只允许左键拖动
    if(event->buttons() & Qt::LeftButton)
    {
        //窗口跟着鼠标移动
        //窗口左上角的点 - 差值
        m_parent->move(event->globalPos()-m_pt);

    }
}

void Title::mousePressEvent(QMouseEvent *ev)
{
    //如果鼠标左键按下
    if(ev->button() == Qt::LeftButton)
    {
        //求差值=鼠标当前位置-窗口左上角
        m_pt  =ev->globalPos() - m_parent->geometry().topLeft();
    }
}
