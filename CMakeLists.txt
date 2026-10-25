# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.16)
project(Milestone3 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.2/mingw_64")
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)



set (SRCS
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
    src/chunker.cpp
    src/context_builder.cpp
    src/context_widget.cpp
    src/corpus_index.cpp
    src/document.cpp
    src/main.cpp
    src/main_window.cpp
    src/processing_core.cpp
    src/retrieval_engine.cpp
    src/search_result_model.cpp
    src/search_widget.cpp
    src/term_inspector_widget.cpp
    src/text_processor.cpp
    src/workspace.cpp
    src/workspace_widget.cpp
)


add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        include
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
