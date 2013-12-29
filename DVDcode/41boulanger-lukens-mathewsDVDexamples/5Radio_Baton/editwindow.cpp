#include "mainwindow.h"
#include "editwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QRect>
#include <QFile>
#include <stdio.h>
#include <QMessageBox>
#include <QLineEdit>


//constructor
EditDialog::EditDialog(QWidget *parent, QString *fileName) : QDialog(parent)
{
    //QByteArray to convert a QString to a char*
    QByteArray ff = fileName->toLatin1();

    //public var fileNamee
    fileNamee = ff;

    //set up the size and location of the window
    this->setGeometry(50, 50, 800, 600);

    //create a pushbutton called quit
    QPushButton *quit = new QPushButton(tr("Quit"));

    //connect it to the closeDialog slot
    connect(quit, SIGNAL(clicked()), this, SLOT(closeDialog()));
    quit->setMaximumWidth(100);

    //save pushbutton and reminder label
    QPushButton *save = new QPushButton(tr("Save"));
    label = new QLabel(tr("Don't forget to save"));
    save->setMaximumWidth(100);

    QString text = "";

    //if the filename isn't empty (the user has loaded a file)
    if (!fileName->isEmpty())
    {
        //open the file, doc open the file, and set the QString text to whatever text is read in
        f = fopen(ff, "r+");
        doc.open(f, QIODevice::ReadWrite);
        text = doc.readAll();
    }
    //if the filename is empty, make a new file
    else
    {
        //Make a new file with the getSaveFileName method
        QString saveFile = QFileDialog::getSaveFileName(this,
             "Save New Conductor File", "/");

        //convert QString to ba (treated as char*) and do the same as above
        QByteArray ba = saveFile.toLatin1();
        fileNamee = ba;
        f = fopen(ba, "w");
        fprintf(f, "empty");
        doc.open(f, QIODevice::ReadWrite);
        text = doc.readAll();
        *fileName = saveFile;
    }

    //create a PlainTextEdit widget
    textEdit = new QPlainTextEdit(text, this);
    //connect the save button
    connect(save, SIGNAL(clicked()), this, SLOT(saveText()));

    //when the user types, remind them to save.
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(docChanged()));

    //to pass to setGeometry
    QRect r(0, 0, 800, 600);

    //create a layout and grid
    QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *grid = new QGridLayout;

    //set the size and location of the box
    layout->setGeometry(r);
    //add the text widget to the layout
    layout->addWidget(textEdit);

    //add save, label, and quit to the grid
    grid->addWidget(save, 1, 1);
    grid->addWidget(label, 1, 2);
    grid->addWidget(quit, 2, 1);

    //add the grid to the layout
    layout->addLayout(grid);

    //set the layout to the window
    setLayout(layout);
}

//this slot is called when the text changes in the text edit widget
void EditDialog::docChanged()
{
    //remind the user to save
    label->setText(tr("Don't forget to save"));
}

//this slot is called with quit is pressed
void EditDialog::closeDialog()
{
    //close the file and the window
    fclose(f);
    close();
}

//called with save is clicked
void EditDialog::saveText()
{
    //convert the text edit text to a QString
    QString st;
    st = textEdit->toPlainText();

    //convert the QString to a bytearray (treated as char*)
    QByteArray ba = st.toLatin1();
    QByteArray ba2 = fileNamee.toLatin1();

    //close out the open file and reopen it in case the window was opened
    //without a filename and the user created a new one
    fclose(f);
    f = fopen(ba2, "w");

    //print all the text to the file
    fprintf(f, ba);
    textEdit->setPlainText(ba);

    //let the user know the file was saved
    label->setText(tr("File saved successfully!"));
}
