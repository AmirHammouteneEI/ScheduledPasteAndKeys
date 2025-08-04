#ifndef GETCURSORCOORDINATESWIDGET_H
#define GETCURSORCOORDINATESWIDGET_H

#include <QDialog>
#include <QMouseEvent>

class getCursorCoordinatesWidget : public QDialog
{
    Q_OBJECT
    int m_refIndex = 0;
public:
    explicit getCursorCoordinatesWidget(QWidget *parent = nullptr);
    ~getCursorCoordinatesWidget();
public slots:
    void showWidget();
protected:
    void mouseReleaseEvent(QMouseEvent*ev) override;
signals:
    void sendCoordinates(int index, int x, int y);
};

#endif // GETCURSORCOORDINATESWIDGET_H
