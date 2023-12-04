# ft_IRC

# Commands TO_DO

## SENDING MESSAGES
- PRIVMSG

### Alex
- OPER (Done)
- NICK (Done)
- USER (Done)
- PASS (Done)
- PING/PONG (Done)
- QUIT

### Clément
- NAMES (Done)
- TOPIC (Done)
- MODE
- INVITE

### Jé
- JOIN 
- PART
- KICK


# IRC Commands Summary

IRC (Internet Relay Chat) commands are used to communicate with an IRC server, allowing users to join channels, send messages, change nicknames, and more. Below is a summary of some common IRC commands:

## 1. `Part`

- **Syntax**: `PART <channel>{, <channel>} [<reason>]`
- **Execution**: This command is used to leave a channel.
- **Functionality**: When a user executes this command, they will be removed from the specified channel(s). An optional reason can be provided, which is visible to others in the channel.

## 2. `Nickname`

- **Syntax**: `NICK <new_nickname>`
- **Execution**: This command changes the user's nickname on the IRC network.
- **Functionality**: Once executed, the user's current nickname is replaced with the new one provided. This is essential for identification purposes.

## 3. `Join`

- **Syntax**: `JOIN <channel>{, <channel>}`
- **Execution**: This command is used to enter a channel.
- **Functionality**: By executing this command, the user joins one or more specified IRC channels, allowing them to participate in discussions there.

## 4. `Username`

- **Syntax**: `USER <username> <hostname> <servername> <realname>`
- **Execution**: This command sets the user’s username and other identification details.
- **Functionality**: It provides essential information about the user to the server, typically executed upon connecting to the IRC network.

## 5. `Ping`

- **Syntax**: `PING <server>`
- **Execution**: This command checks the connection to the server.
- **Functionality**: It is used to ensure that the connection to the IRC server is still active. The server usually responds with a PONG message.

## 6. `Pass`

- **Syntax**: `PASS <password>`
- **Execution**: This command is used for password authentication.
- **Functionality**: It is typically used during the initial connection process to authenticate a user with a server or network.

## 7. `Privmsg`

- **Syntax**: `PRIVMSG <receiver>{, <receiver>} :<message>`
- **Execution**: This command sends a private message.
- **Functionality**: It allows users to communicate privately with other users or channels, without their messages being seen by others.

## 8. `Topic`

- **Syntax**: `TOPIC <channel> [:<new_topic>]`
- **Execution**: This command is used to change or view the topic of a channel.
- **Functionality**: It displays the current topic for a channel or sets a new topic if the optional `new_topic` parameter is provided.

## 9. `Kick`

- **Syntax**: `KICK <channel> <user> [:<reason>]`
- **Execution**: This command removes a user from a channel.
- **Functionality**: It forcibly ejects the specified user from the specified channel. An optional reason for the kick can be included.

## 10. `Mode`

- **Syntax**: `MODE <channel/user> [+/-<mode flags>] [<mode params>]`
- **Execution**: This command changes the mode of a channel or user.
- **Functionality**: It is used to set various user or channel modes, like making a channel invite-only or giving a user operator privileges.
- **FLAGS**
    - i : Définir/supprimer le canal sur invitation uniquement
    - t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
    - k : Définir/supprimer la clé du canal (mot de passe)
    - o : Donner/retirer le privilège de l’opérateur de canal
    - l : Définir/supprimer la limite d’utilisateurs pour le canal

## 11. `Oper`

- **Syntax**: `OPER <name> <password>`
- **Execution**: This command is used to obtain operator status.
- **Functionality**: On providing the correct name and password, the user is granted IRC operator privileges, allowing them to perform administrative tasks.