cmake_minimum_required(VERSION 3.20)
project(345_warzone)

set(CMAKE_CXX_STANDARD 14)

include_directories(Cards)
include_directories(GameEngine)
include_directories(Map)
include_directories(Orders)
include_directories(Player)
include_directories(CommandProcessing)
include_directories(GameLogObserver)

add_executable(345_warzone
        Cards/Cards.cpp
        Cards/Cards.h
        GameEngine/GameEngine.cpp
        GameEngine/GameEngine.h
        GameLogObserver/LoggingObserver.cpp
        GameLogObserver/LoggingObserver.h
        Map/Map.cpp
        Map/Map.h
        Orders/Orders.cpp
        Orders/Orders.h
        Player/Player.cpp
        Player/Player.h
        mainDriver.cpp
        CommandProcessing/CommandProcessing.cpp
        CommandProcessing/CommandProcessing.h
)
