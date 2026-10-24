# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(Milestone03)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)



set (SRCS
    data/public/doc_ai.txt
    data/public/doc_retrieval.txt
    data/public/doc_systems.txt
    data/public/queries.txt
    include/aiws/gui/context_widget.hpp
    include/aiws/gui/main_window.hpp
    include/aiws/gui/resettable.hpp
    include/aiws/gui/search_result_model.hpp
    include/aiws/gui/search_widget.hpp
    include/aiws/gui/term_inspector_widget.hpp
    include/aiws/gui/workspace_widget.hpp
    include/aiws/chunker.hpp
    include/aiws/chunking_strategy.hpp
    include/aiws/context_builder.hpp
    include/aiws/context_strategy.hpp
    include/aiws/corpus_index.hpp
    include/aiws/document.hpp
    include/aiws/processing_core.hpp
    include/aiws/processing_types.hpp
    include/aiws/retrieval_engine.hpp
    include/aiws/retrieval_strategy.hpp
    include/aiws/text_processor.hpp
    include/aiws/workspace.hpp
    src/context_widget.cpp
    src/document.cpp
    src/main.cpp
    src/main_window.cpp
    src/search_result_model.cpp
    src/search_widget.cpp
    src/term_inspector_widget.cpp
    src/workspace.cpp
    src/workspace_widget.cpp
    chunker.cpp
    context_builder.cpp
    corpus_index.cpp
    processing_core.cpp
    retrieval_engine.cpp
    text_processor.cpp
)


add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
