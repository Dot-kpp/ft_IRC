cmake_minimum_required(VERSION 3.26)
project(ft_IRC)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)
include_directories(inc)
include_directories(inc/commands)
include_directories(inc/server)

add_executable(ft_IRC
        headers/irc.hpp
        inc/commands/Command.hpp
        inc/commands/CommandHandler.hpp
        inc/commands/Invite.hpp
        inc/commands/Join.hpp
        inc/commands/Kick.hpp
        inc/commands/Mode.hpp
        inc/commands/Names.hpp
        inc/commands/Nick.hpp
        inc/commands/Notice.hpp
        inc/commands/Oper.hpp
        inc/commands/Part.hpp
        inc/commands/Pass.hpp
        inc/commands/Ping.hpp
        inc/commands/Quit.hpp
        inc/commands/Topic.hpp
        inc/commands/User.hpp
        inc/server/Channels.hpp
        inc/server/Server.hpp
        inc/server/Socket.hpp
        inc/Client.hpp
        src/commands/Command.cpp
        src/commands/CommandHandler.cpp
        src/commands/Invite.cpp
        src/commands/Join.cpp
        src/commands/Kick.cpp
        src/commands/Mode.cpp
        src/commands/Names.cpp
        src/commands/Nick.cpp
        src/commands/Notice.cpp
        src/commands/Oper.cpp
        src/commands/Part.cpp
        src/commands/Pass.cpp
        src/commands/Ping.cpp
        src/commands/PrivMsg.cpp
        src/commands/Quit.cpp
        src/commands/Topic.cpp
        src/commands/User.cpp
        src/server/Channels.cpp
        src/server/Server.cpp
        src/server/Socket.cpp
        src/Client.cpp
        src/main.cpp)
