#include "../../inc/server/Server.hpp"

// Numeric Replies:

// ERR_NONICKNAMEGIVEN (431)
// ERR_ERRONEUSNICKNAME (432)
// ERR_NICKNAMEINUSE (433)
// ERR_NICKCOLLISION (436)
// f the server does not accept the new nickname supplied by the client as valid (for instance, due to containing invalid characters), it should issue an ERR_ERRONEUSNICKNAME numeric and ignore the NICK command. Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character. Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, including:

// no leading # character or other character advertized in CHANTYPES
// no leading colon (:)
// no ASCII space
// If the server does not receive the <nickname> parameter with the NICK command, it should issue an ERR_NONICKNAMEGIVEN numeric and ignore the NICK command.

// The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.

//   CHANTYPES=#

//   CHANTYPES=&#

//   CHANTYPES=#&
bool parseNickname(std::string nickname, int clientFd)
{
    Server *server = Server::instance;

    std::map<int, Client>::iterator it = server->clients.begin();
    while (it != server->clients.end())
    {
        if (it->second.getNickName() == nickname)
        {
            std::string doubleNickError = "433 " + nickname + " :Nickname is already in use";
            send(clientFd, doubleNickError.c_str(), doubleNickError.size(), 0);
            return false;
        }
        it++;
    }
    if (isdigit(nickname[0])) {
        std::string doubleNickError = "432 " + nickname + " :Erroneus nickname";
        send(clientFd, doubleNickError.c_str(), doubleNickError.size(), 0);
        return false;
    }

    // Check if the nickname contains only valid characters
    for (std::string::iterator it = nickname.begin(); it != nickname.end(); ++it)
    {
        char c = *it;
        if (!(isalnum(c) || c == '[' || c == ']' || c == '{' || c == '}' || c == '\\' || c == '|'))
        {
            std::string doubleNickError = "432 " + nickname + " :Erroneus nickname";
            send(clientFd, doubleNickError.c_str(), doubleNickError.size(), 0);
            return false;
        }
    }

    return true;
}