#include "aiws/gui/main_window.hpp"
#include "aiws/gui/context_widget.hpp"
#include "aiws/gui/search_widget.hpp"
#include "aiws/gui/term_inspector_widget.hpp"
#include "aiws/gui/workspace_widget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>

namespace aiws::gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {

    workspace_widget_ = new WorkspaceWidget(this);
    search_widget_ = new SearchWidget(this);
    term_inspector_widget_ = new TermInspectorWidget(this);
    context_widget_ = new ContextWidget(this);

    status_label_ = new QLabel("Current status: Ready", this);

    reset_button_ = new QPushButton("Reset", this);
    exit_button_ = new QPushButton("Exit", this);

    // Central widget
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // Main layout
    QVBoxLayout* main_layout = new QVBoxLayout;

    main_layout->addWidget(workspace_widget_);
    main_layout->addWidget(search_widget_);

    // Bottom row containing term inspector and context
    QHBoxLayout* lower_layout = new QHBoxLayout;
    lower_layout->addWidget(term_inspector_widget_);
    lower_layout->addWidget(context_widget_);

    main_layout->addLayout(lower_layout);

    // Status row
    QHBoxLayout* status_layout = new QHBoxLayout;

    status_layout->addWidget(status_label_);
    status_layout->addStretch();
    status_layout->addWidget(reset_button_);
    status_layout->addWidget(exit_button_);

    main_layout->addLayout(status_layout);

    central->setLayout(main_layout);

    connect(reset_button_,
            &QPushButton::clicked,
            this,
            &MainWindow::reset_application);

    connect(exit_button_,
            &QPushButton::clicked,
            this,
            &MainWindow::exit_application);

    connect(workspace_widget_,
            &WorkspaceWidget::add_document_requested,
            this,
            &MainWindow::add_document);

    connect(workspace_widget_,
            &WorkspaceWidget::build_corpus_requested,
            this,
            &MainWindow::build_corpus);

    connect(workspace_widget_,
            &WorkspaceWidget::document_selected,
            this,
            &MainWindow::document_selected);

    connect(search_widget_,
            &SearchWidget::search_requested,
            this,
            &MainWindow::search_requested);

    connect(search_widget_,
            &SearchWidget::query_changed,
            this,
            &MainWindow::query_changed);

    connect(search_widget_,
            &SearchWidget::source_document_requested,
            this,
            &MainWindow::source_document_requested);

    connect(search_widget_,
            &SearchWidget::selected_chunk_changed,
            term_inspector_widget_,
            &TermInspectorWidget::set_selected_chunk);

    connect(search_widget_,
            &SearchWidget::selected_chunk_changed,
            term_inspector_widget_,
            &TermInspectorWidget::set_selected_chunk);

    connect(term_inspector_widget_,
            &TermInspectorWidget::term_inspection_requested,
            this,
            &MainWindow::term_inspection_requested);

    connect(context_widget_,
            &ContextWidget::build_context_requested,
            this,
            &MainWindow::build_context_requested);


    update_action_states();
}

void MainWindow::set_exit_callback(ExitCallback callback) noexcept {
    exit_callback_ = callback;
}



void MainWindow::add_document() {
    QString filename =
        QFileDialog::getOpenFileName(
            this,
            "Open Document",
            QString(),
            "Text Files (*.txt)");

    if (filename.isEmpty()) {
        return;
    }

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString text = QString::fromUtf8(file.readAll());

    Document doc(
        filename.toStdString(),
        QFileInfo(filename).fileName().toStdString(),
        text.toStdString()
        );

    workspace_.add_document(doc);

    workspace_widget_->add_document(
        QString::fromStdString(doc.title()));

    int row = static_cast<int>(
        workspace_.documents().size() - 1);

    workspace_widget_->select_document(row);

    workspace_widget_->show_selected_document(
        QString::fromStdString(doc.title()),
        QString::fromStdString(doc.text()));

    invalidate_corpus();
    clear_processing_outputs();
    update_action_states();
}

void MainWindow::build_corpus() {
    processing_core_.rebuild(workspace_);

    corpus_current_ = true;

    workspace_widget_->set_corpus_status(
        QString("Corpus: %1 chunks")
            .arg(processing_core_.chunk_count()));

    clear_processing_outputs();

    status_label_->setText("Corpus built.");

    update_action_states();
}

void MainWindow::document_selected(int row) {
    if (row < 0) {
        workspace_widget_->clear_selected_document();
        return;
    }

    const auto& docs = workspace_.documents();

    if (static_cast<size_t>(row) >= docs.size()) {
        return;
    }

    const auto& doc = docs[row];

    workspace_widget_->show_selected_document(
        QString::fromStdString(doc.title()),
        QString::fromStdString(doc.text()));
}

void MainWindow::search_requested(const QString& query, int k) {
    auto results =
        processing_core_.search(query.toStdString(), k);

    search_widget_->set_results(std::move(results));
    (void)query;  // To suppress unused-parameter warning.
    (void)k;  // To suppress unused-parameter warning.
}

void MainWindow::query_changed(const QString&) {
    update_action_states();
}

void MainWindow::build_context_requested(int token_budget) {
    auto results =
        processing_core_.build_context(
            search_widget_->query().toStdString(),
            search_widget_->result_count(),
            static_cast<std::size_t>(token_budget));

    context_widget_->set_context(std::move(results));
}

void MainWindow::source_document_requested(const QString& document_id) {

    const auto& docs = workspace_.documents();

    for (std::size_t i = 0; i < docs.size(); ++i) {

        if (QString::fromStdString(docs[i].id())
            == document_id) {

            workspace_widget_->select_document(
                static_cast<int>(i));

            workspace_widget_->show_selected_document(
                QString::fromStdString(docs[i].title()),
                QString::fromStdString(docs[i].text()));

            break;
        }
    }
}


void MainWindow::term_inspection_requested(
    const QString& term,
    const QString& chunk_id) {

    std::size_t df =
        processing_core_.document_frequency(
            term.toStdString());

    if (chunk_id.isEmpty()) {

        term_inspector_widget_->set_statistics(
            df,
            false,
            0);

        return;
    }

    std::size_t tf =
        processing_core_.term_frequency(
            term.toStdString(),
            chunk_id.toStdString());

    term_inspector_widget_->set_statistics(
        df,
        true,
        tf);
}

void MainWindow::reset_application() {

    workspace_.clear();

    corpus_current_ = false;

    workspace_widget_->reset();
    search_widget_->reset();
    term_inspector_widget_->reset();
    context_widget_->reset();

    status_label_->setText("Ready");

    update_action_states();
}

void MainWindow::exit_application() {

    if (exit_callback_ != nullptr) {
        exit_callback_(this);
    }
}

void MainWindow::update_action_states() {

    bool has_documents =
        !workspace_.documents().empty();

    workspace_widget_->set_build_corpus_enabled(
        has_documents);

    bool can_search =
        corpus_current_ &&
        search_widget_->has_query();

    search_widget_->set_search_enabled(
        can_search);

    term_inspector_widget_->set_inspection_enabled(
        corpus_current_);

    context_widget_->set_build_context_enabled(
        can_search);
}

void MainWindow::invalidate_corpus() {

    corpus_current_ = false;

    workspace_widget_->set_corpus_status(
        "Corpus not built");

    clear_processing_outputs();

    update_action_states();
}

void MainWindow::clear_processing_outputs() {

    search_widget_->clear_results();

    term_inspector_widget_->clear_statistics();

    context_widget_->clear_context();
}

}  // namespace aiws::gui
