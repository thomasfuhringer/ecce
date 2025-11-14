// Markdown viewer
// Thomas Führinger, 2025-10-26

#include "MainWindow.h"
#include <QTextEdit>

int
main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Ecce");
    QCoreApplication::setApplicationVersion("1.0");

    MainWindow main_window;

    if (argc > 1)
        main_window.loadFile(argv[1]);
    else
        main_window.loadURL("https://raw.githubusercontent.com/thomasfuhringer/ecce/refs/heads/main/README.md");

    main_window.show();
    return app.exec();
}
