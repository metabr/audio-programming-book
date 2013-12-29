#ifndef EDITWINDOW_H
#define EDITWINDOW_H
#include <QDialog>
#include <QFile>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QLabel>


class EditDialog : public QDialog
{
    Q_OBJECT

public:
    EditDialog(QWidget *parent, QString *fileName);

    FILE *f;
    QFile doc;
    QPlainTextEdit *textEdit;
    QString fileNamee;
    QLabel *label;

private slots:
    void saveText();
    void closeDialog();
    void docChanged();
};

#endif // EDITWINDOW_H
