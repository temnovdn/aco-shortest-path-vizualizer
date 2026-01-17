cmake_minimum_required(VERSION 3.16)
project(ACO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
	main.cpp
	MainWindow.cpp
	GraphScene.cpp
	graph.cpp
	NodeItem.cpp
	EdgeItem.cpp
)

set(HEADERS
	MainWindow.h
	GraphScene.h
	graph.h
	NodeItem.h
	EdgeItem.h
)

qt_add_executable(ACO
	${SOURCES}
	${HEADERS}
)

target_link_libraries(ACO
	PRIVATE Qt6::Widgets
)
