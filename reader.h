#ifndef READER_H
#define READER_H

#include <QMainWindow>
#include "data.h"

namespace Ui {
class reader;
}

class reader : public QMainWindow
{
    Q_OBJECT

public:
    explicit reader(QWidget *parent = 0);
    ~reader();

signals:
    void closed();

private slots:
    void on_searchButton_clicked();

    void on_changeButton_clicked();

    void on_bookTable_cellClicked(int row, int column);

    void on_bookBorrow_clicked();

    void on_send_clicked();

    void on_cancel_clicked();

    void on_borInfo_cellClicked(int row, int column);

    void on_changePass_clicked();

    void ChangePass();

private:
    Ui::reader *ui;
    QSharedPointer<Password> pa;
    QSharedPointer<QGraphicsScene> ptr_scene;

    void closeEvent (QCloseEvent *event);

};

#endif // READER_H
