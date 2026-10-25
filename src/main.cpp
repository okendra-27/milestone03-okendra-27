#include "aiws/gui/main_window.hpp"

#include <QApplication>
#include <QMessageBox>

// TODO: Define the ExitCallback function used to confirm an Exit request.

void ExitCallback(QWidget* parent) {
    QMessageBox::StandardButton response =
        QMessageBox::question(
            parent,
            "Confirm Exit",
            "Are you sure you want to exit?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No);

    if (response == QMessageBox::Yes) {
        QApplication::quit();
    }
}

int main(int argc, char* argv[]) {
    // TODO: Create the QApplication, construct/configure the MainWindow,
    // register the ExitCallback, show the window, and run the event loop.
       QApplication app(argc, argv);

    aiws::gui::MainWindow window;

    // Register required callback
    window.set_exit_callback(ExitCallback);

    // M3 startup size requirement
    window.resize(900, 900);

    window.show();

    return app.exec();
    (void)argc;  // To suppress unused-parameter warning.
    (void)argv;  // To suppress unused-parameter warning.
    return 0;
}
