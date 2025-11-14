#include "MainWindow.h"
#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Ecce");
    setUnifiedTitleAndToolBarOnMac(true);
    createActionsMenusToolbars();
    text_edit = new QTextEdit();
    text_edit->setReadOnly(true);
    setCentralWidget(text_edit);

    QSettings settings(this);
    restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());
    restoreState(settings.value("MainWindow/WindowState").toByteArray());
}

MainWindow::~MainWindow() { }

void
MainWindow::closeEvent(QCloseEvent* event) {
    QSettings settings(this);
    settings.setValue("MainWindow/Geometry", saveGeometry());
    settings.setValue("MainWindow/WindowState", saveState());
    QMainWindow::closeEvent(event);
}

void
MainWindow::createActionsMenusToolbars() {
    setWindowIcon(QIcon(":/Images/App.png"));
    open_action = new QAction(QIcon::fromTheme("document-open", QIcon(":/Images/Open.png")), "&Open...", this);
    open_action->setShortcuts(QKeySequence::Open);
    open_action->setStatusTip(tr("Open an existing file"));
    connect(open_action, &QAction::triggered, this, &MainWindow::open);

    exit_action = new QAction(QIcon::fromTheme("application-exit", QIcon(":/Images/Exit.png")), "E&xit", this);
    exit_action->setMenuRole(QAction::QuitRole);
    exit_action->setStatusTip("Quit Ecce");
    exit_action->setShortcut(QKeySequence::Quit);
    connect(exit_action, &QAction::triggered, this, &MainWindow::close);

    about_action = new QAction(QIcon::fromTheme("help-about", QIcon(":/Images/About.ico")), "&About", this);
    about_action->setMenuRole(QAction::AboutRole);
    about_action->setStatusTip(tr("About Ecce"));
    connect(about_action, &QAction::triggered, this, &MainWindow::showAboutBox);

    file_menu = menuBar()->addMenu("&File");
    file_menu->addAction(open_action);
    file_menu->addSeparator();
    file_menu->addAction(exit_action);
    help_menu = menuBar()->addMenu("&Help");
    help_menu->addAction(about_action);

    execute_action = new QAction(QIcon::fromTheme("view-refresh", QIcon(":/Images/Execute.png")), "&Retrieve...", this);
    execute_action->setShortcut(QKeySequence(Qt::Key_Return));
    connect(execute_action, &QAction::triggered, this, &MainWindow::execute);

    main_toolbar = addToolBar("Main");
    main_toolbar->setIconSize(QSize(16, 16));
    main_toolbar->setFloatable(false);
    main_toolbar->setMovable(false);
    main_toolbar->setObjectName("ToolBar");

    main_toolbar->addAction(open_action);
    address_lineedit = new QLineEdit;
    main_toolbar->insertWidget(nullptr, address_lineedit);
    main_toolbar->addAction(execute_action);

    statusBar()->setVisible(true);
}

void
MainWindow::open() {
    QString file_name = QFileDialog::getOpenFileName(this, "Open File", nullptr, "Markdown Files (*.md)");
    if (file_name != nullptr)
        loadFile(file_name);
}

void
MainWindow::loadFile(const QString file_name) {
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        // qDebug() << "File read failed";
        statusBar()->showMessage("Could not read file.");
        return;
    }
    QTextStream in(&file);
    text_edit->setMarkdown(in.readAll());
    address_lineedit->setText(file_name);
    statusBar()->clearMessage();
    text_edit->setFocus();
}

void
MainWindow::loadURL(const QString address) {
    QNetworkAccessManager manager;
    QNetworkReply* network_reply = manager.get(QNetworkRequest(QUrl(address)));
    QEventLoop event_loop;
    connect(network_reply, SIGNAL(finished()), &event_loop, SLOT(quit()));
    event_loop.exec();

    text_edit->setMarkdown(network_reply->readAll());
    address_lineedit->setText(address);
    text_edit->setFocus();
}

void
MainWindow::execute() {
    QString address = address_lineedit->text();

    if (address.first(4) == "http")
        loadURL(address);
    else
        loadFile(address);
}

void
MainWindow::showAboutBox() {
    AboutBox aboutBox(this);
    aboutBox.exec();
}
