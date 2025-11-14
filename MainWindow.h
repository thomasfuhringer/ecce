#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void loadFile(const QString file_name);
    void loadURL(const QString address);

private:
    void closeEvent(QCloseEvent* event);
    void createActionsMenusToolbars();
    void showAboutBox();
    QMenu* file_menu;
    QMenu* help_menu;
    QAction* open_action;
    QAction* exit_action;
    QAction* execute_action;
    QAction* about_action;
    QTextEdit* text_edit;
    QToolBar* main_toolbar;
    QLineEdit* address_lineedit;

private slots:
    void open();
    void execute();
};

class AboutBox : public QDialog {

public:
    AboutBox(QMainWindow* parent = 0)
        : QDialog(parent) {
        setWindowTitle("About");
        QVBoxLayout* layout = new QVBoxLayout(this);
        QLabel* label = new QLabel(this);
        label->setText(QString("<h2>Ecce</h2>"
                               "<p><em>Version %1, 2025</em></p><p>&nbsp;</p>"
                               "<p>Markdown viewer by Thomas F&uuml;hringer<br></p>")
                .arg(QApplication::instance()->applicationVersion()));
        label->setMargin(20);
        QDialogButtonBox* button_box = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::accept);
        layout->addWidget(label);
        layout->addWidget(button_box);
        setLayout(layout);
        setMinimumSize(QSize(310, 250));
    }
};

#endif // MAINWINDOW_H
