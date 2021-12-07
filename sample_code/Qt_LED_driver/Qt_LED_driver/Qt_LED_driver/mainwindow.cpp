#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fd = open("/dev/led", O_RDWR);             // Open the device with read/write access
    if (fd < 0){
       perror("Failed to open the device...");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ioctl(fd, LED_SET_MAGIC);
}

void MainWindow::on_pushButton_2_clicked()
{
    ioctl(fd, LED_CLEAR_MAGIC);
}
