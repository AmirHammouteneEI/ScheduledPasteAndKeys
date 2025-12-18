#ifndef CUSTOMPRIMARYWIDGETS_H
#define CUSTOMPRIMARYWIDGETS_H

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTimeEdit>
#include <QDateTimeEdit>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include "qevent.h"

class NoWheelFocusSpinBox : public QSpinBox
{
    Q_OBJECT
public:

    NoWheelFocusSpinBox(QWidget *parent = 0):QSpinBox(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    virtual void wheelEvent(QWheelEvent *event) override
    {
        if(!hasFocus())
            event->ignore();
        else
            QSpinBox::wheelEvent(event);
    }
};

class NoWheelFocusDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    NoWheelFocusDoubleSpinBox(QWidget *parent = 0):QDoubleSpinBox(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    virtual void wheelEvent(QWheelEvent *event) override
    {
        if(!hasFocus())
            event->ignore();
        else
            QDoubleSpinBox::wheelEvent(event);
    }
};

class NoWheelFocusTimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    NoWheelFocusTimeEdit(QWidget *parent = 0):QTimeEdit(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    virtual void wheelEvent(QWheelEvent *event) override
    {
        if(!hasFocus())
            event->ignore();
        else
            QTimeEdit::wheelEvent(event);
    }
};

class NoWheelFocusDateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    NoWheelFocusDateTimeEdit(QWidget *parent = 0):QDateTimeEdit(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    virtual void wheelEvent(QWheelEvent *event) override
    {
        if(!hasFocus())
            event->ignore();
        else
            QDateTimeEdit::wheelEvent(event);
    }
};

class NoFocusCellTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    NoFocusCellTableWidget(QWidget *parent = 0):QTableWidget(parent)
    {
        setFocusPolicy(Qt::NoFocus);
        setMouseTracking(false);
        setItemDelegate(new QStyledItemDelegate(this));
    }
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override
    {
        event->ignore();
    }
};

#endif // CUSTOMPRIMARYWIDGETS_H
