#ifndef TITLE_H
#define TITLE_H

#include <QWidget>

namespace Ui {
class Title;
}

class Title : public QWidget
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = nullptr);
    ~Title();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *ev);
signals:
    void showSetWg();
    void closeWindow();
private:
    Ui::Title *ui;
    QPoint m_pt; //差值: 鼠标当前位置 - 减去窗口左上角的店

    QWidget *m_parent;
};

#endif // TITLE_H
