#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <poll.h>
#include <thread>
#include <unordered_set>
#include <signal.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string>
#include <cstring>
#include <random>
#include <map>
#include <stdexcept>
#include <list>

class Socket;
/**

Class: User
Description: A class that represents a user and their associated socket connection.
*/
class User
{
private:
    int score;

public:
    User(std::string, Socket *);

    std::string name;

    Socket *socket;

    bool isClosed = false;

    /**
     * Function: getScore
     *
     * Return value: The user's score.
     *
     * Description: Returns the user's score.
     *
     */
    int getScore();
    /**
     * Function: setScore
     *
     * Parameters:
     *  int score - The new score for the user.
     *
     * Description: Sets the user's score to the given value.
     *
     */
    void setScore(int);
    /**
     * Function: incrementScore
     *
     * Description: Increments the user's score by 1.
     *
     */
    void incrementScore();
};

/**

Constructor: User
Parameters:
std::string name - The user's name.
Socket* socket - The user's socket connection.
Description: Creates a new User object with the given name and socket connection.
Initializes the user's score to 0 and assigns the given name and socket to the corresponding member variables.
*/
User::User(std::string name, Socket *socket)
{
    this->score = 0;
    this->name = name;
    this->socket = socket;
}

/**

Function: getScore
Return value: The user's score.
Description: Returns the user's score.
*/
int User::getScore()
{
    return this->score;
}

/**
 *
Function: setScore
Parameters:
int n - The new score for the user.
Description: Sets the user's score to the given value.
*/
void User::setScore(int n)
{
    this->score = n;
}

/**

Function: incrementScore
Description: Increments the user's score by 1.
*/
void User::incrementScore()
{
    this->score++;
}

/**

Class: Game
Description: A class that represents a game and the associated users, questions and answers.
*/
class Game
{
private:
    /*
     * qNumber: The current question number.
     */
    int qNumber;
    /**
     * gameId: The id of the game.
     */
    int gameId;
    /**
     * time: The time given for a question.
     */
    int time;
    /**
     * answers: A 2D array representing the answers for all questions.
     */
    bool **answers;
    /**
     * isGameReady: A flag indicating whether the game is ready to start.
     */
    bool isGameReady;
    /**
     * isGameStarted: A flag indicating whether the game has started.
     */
    bool isGameStarted;
    /**
     * nSetAnswers: The number of answers that have been set.
     */
    int nSetAnswers;
    /**
     * nQuizQuestion: The total number of questions in the game.
     */
    int nQuizQuestion;

public:
    /**
     * Constructor: Game
     *
     * Parameters:
     * std::string gameName - The name of the game.
     * int gameId - The id of the game.
     * int time - The time given for a question.
     * int nQuizQuestion - The total number of questions in the game.
     * Socket* socket - The game's socket connection.
     *
     * Description: Creates a new Game object with the given name, gameId, time, nQuizQuestion and socket connection.
     *
     */
    Game(std::string, int, int, int, Socket *);

    /**
     * gameName: The name of the game.
     */
    std::string gameName;

    /**
     * users: A vector of User pointers representing the users in the game.
     */
    std::vector<User *> users;

    /**
     * socket: The game's socket connection.
     */
    Socket *socket;

    /**
     * Function: getQNumber
     *
     * Return value: The current question number.
     *
     * Description: Returns the current question number.
     *
     */
    int getQNumber();
    /**
     * Function: setQNumber
     *
     * Parameters:
     *  int qNumber - The new question number.
     *
     * Description: Sets the current question number to the given value.
     *
     */
    void setQNumber(int);
    /**
     * Function: getTime
     *
     * Return value: The time given for a question.
     *
     * Description: Returns the time given for a question.
     *
     */
    int getTime();

    /**

    Function: setGameId
    Parameters:
    int gameId - The new game id.
    Description: Sets the game id to the given value.
    */
    int getGameId();

    /**

    Function: getAnswer
    Parameters:
    int qNumber - The question number.
    Return value: A pointer to the array of answers for the given question.
    Description: Returns a pointer to the array of answers for the given question.
    */
    void setGameId(int);

    bool *getAnswer(int);
    /**

    Function: setAnswer
    Parameters:
    bool answers[4] - The array of answers for a question.
    Description: Sets the answers for a question to the given value.
    */
    void setAnswer(bool[4]);

    bool getGameReady();
    void setGameReady();

    bool getGameStarted();

    void disconnectAllUsers();

    bool isValidUser(std::string);
    bool addUser(std::string, Socket *);
    int userPosition(std::string);
    bool deleteUser(std::string);

    bool isHost(int);
    void onHostDisconnected();
    void onPlayerDisconnected(int);

    std::string getUsersMessage();
    void broadcastToUsers(std::string, bool);

    bool startGame();
    std::string getUserPoints();

    void nextQuestion();

    // a - [0,3];
    void checkAnswer(int, int q, int a);
};

/**

Class: Socket
Description: A class representing a socket connection.
*/
class Socket
{
public:
    int sock;
    char buffer[512];
    /*

    Variable: nowReaded
    Description: A string storing the data that has been read from the socket but not yet processed.
    */
    std::string nowReaded;
    int messageSize = -1;
    std::list<std::string> message;
    Game *game = nullptr;

    void readData();
    void writeData(std::string);
    void socketClose();
};

Game::Game(std::string name, int gameId, int qNumber, int time, Socket *socket)
{
    this->gameName = name;
    this->gameId = gameId;
    this->qNumber = qNumber;
    this->time = time;
    this->isGameReady = false;
    this->isGameStarted = false;
    this->answers = new bool *[qNumber];
    this->socket = socket;
    this->nQuizQuestion = 0;
    for (int i = 0; i < qNumber; i++)
    {
        this->answers[i] = new bool[4];
    }
    this->users = {};
    this->nSetAnswers = 0;
}

int Game::getQNumber()
{
    return this->qNumber;
}

void Game::setQNumber(int n)
{
    this->qNumber = n;
}

int Game::getTime()
{
    return this->time;
}

int Game::getGameId()
{
    return this->gameId;
}

void Game::setGameId(int id)
{
    this->gameId = id;
}

bool *Game::getAnswer(int n)
{
    return this->answers[n];
}

void Game::setAnswer(bool arr[4])
{
    if (this->nSetAnswers < this->qNumber)
    {
        this->answers[this->nSetAnswers] = arr;
        this->nSetAnswers++;
    }
}

bool Game::getGameReady()
{
    return this->isGameReady;
}

void Game::setGameReady()
{
    this->isGameReady = true;
}

bool Game::isValidUser(std::string name)
{
    if (this->users.size() == 0)
        return true;
    else
    {
        for (User *el : this->users)
        {
            if (name == el->name)
                return false;
        }
        return true;
    }
}

bool Game::addUser(std::string name, Socket *sock)
{
    if (isValidUser(name))
    {
        this->users.push_back(new User(name, sock));
        return true;
    }
    return false;
}

int Game::userPosition(std::string name)
{
    int i = 0;
    for (User *el : this->users)
    {
        if (name == el->name)
            return i;
        i++;
    }
    return -1;
}

bool Game::deleteUser(std::string name)
{
    if (userPosition(name) != -1)
    {
        this->users.erase(this->users.begin() + userPosition(name));
        return true;
    }
    return false;
}

std::string Game::getUsersMessage()
{
    std::string mess = "\\users";
    if (this->users.size() == 0)
        mess += "\\";
    else
    {
        for (auto &user : this->users)
        {
            mess += "\\" + user->name;
        }
    }
    return mess;
}

/**

Function: broadcastToUsers
Parameters:
    std::string mess - The message to be broadcasted.
    bool withAdmin - A flag indicating whether the message should also be sent to the admin.
Description: Broadcasts the given message to all users in the game. If the withAdmin flag is set to true,
the message will also be sent to the admin. If a user's connection is closed, the message will not be sent to them.
*/
void Game::broadcastToUsers(std::string mess, bool withAdmin)
{
    if (withAdmin)
        socket->writeData(mess);
    for (auto &user : this->users)
    {
        if (!user->isClosed)
            user->socket->writeData(mess);
    }
}

void Game::checkAnswer(int sock, int q, int a)
{
    if (q == this->nQuizQuestion)
    {
        if (this->answers[q][a] == true)
        {
            for (User *el : this->users)
            {
                if (sock == el->socket->sock)
                {
                    el->incrementScore();
                    break;
                }
            }
        }
    }
}

bool Game::isHost(int sock)
{
    if (sock == socket->sock)
        return true;
    else
        return false;
}

void Game::onHostDisconnected()
{
    broadcastToUsers("\\unreachable_host\\", false);
    disconnectAllUsers();
}


/**

Function: onPlayerDisconnected
Parameters:
    int sock - The file descriptor of the socket that has disconnected.
Description: This function is called when a player has disconnected from the game. It marks the user's connection as closed
and if the game has not started yet it will delete the user from the game.
It will also send a message to all the other users in the game and the admin that a player has disconnected.
*/
void Game::onPlayerDisconnected(int sock)
{
    std::string user;
    bool before = false;
    for (User *el : this->users)
    {
        if (sock == el->socket->sock)
        {
            if (!isGameStarted)
            {
                user = el->name;
                before = true;
            }
            else
            {
                el->isClosed = true;
            }
            break;
        }
    }
    if (before)
    {
        deleteUser(user);
        broadcastToUsers("\\delete_user\\" + user, true);
    }
}

bool Game::startGame()
{
    if (users.empty())
        return false;
    else
    {
        setGameReady();
        this->isGameStarted = true;
        return true;
    }
}

bool Game::getGameStarted()
{
    return this->isGameStarted;
}

void Game::nextQuestion()
{
    this->nQuizQuestion++;
}

std::string Game::getUserPoints()
{
    std::string command = "\\game_results";
    for (User *el : this->users)
    {
        command += "\\name\\" + el->name;
        command += "\\score\\" + std::to_string(el->getScore());
    }
    return command;
}

void Game::disconnectAllUsers()
{
    for (User *el : this->users)
    {
        el->socket->socketClose();
    }
}


/**

Function: readData

Description: This function is used to read data from the socket. It reads up to 512 bytes of data

from the socket and stores it in the buffer. If the read size is less than 1, it throws an exception

indicating that the client has disconnected.

It also checks the buffer for messages, and if it finds any complete messages, it adds them to the message list.

If the message size is not yet determined, it looks for the message size in the buffer, and if found,

it saves it and removes it from the buffer.
*/
void Socket::readData()
{
    std::memset(buffer, 0, 512);
    int read_size = read(sock, buffer, sizeof(buffer));

    if (read_size < 1)
    {
        fprintf(stderr, "%d disconnected\n", sock);
        throw "client disconnected";
    }
    nowReaded += std::string(buffer, read_size);
    while (true)
    {
        if (messageSize < 0)
        {
            if (nowReaded.size() > sizeof(int))
            {
                messageSize = ntohl(*(int *)nowReaded.c_str());
                nowReaded = nowReaded.substr(sizeof(int));
            }
            else
                break;
        }
        int len = nowReaded.length();
        if (messageSize <= len)
        {
            message.push_back(nowReaded.substr(0, messageSize));
            nowReaded = nowReaded.substr(messageSize);
            messageSize = -1;
        }
        else
        {
            break;
        }
    }
}
/*
The writeData function takes a string argument message, which it sends over the socket connection represented by the sock member of the Socket class.
First, the size of the message is converted to network byte order using the htonl function and then written to the socket using the write function.
Then the actual message is written to the socket using the write function and the message.c_str() function, which returns a pointer to the underlying character array used by the string object.
If the write function returns a value less than 0, it prints an error message "write data".
*/
void Socket::writeData(std::string message)
{
    int size = htonl(message.size());
    if (write(sock, (char *)&size, sizeof(int)) < 0)
    {
        printf("%s\n", "write data");
    }
    if (write(sock, message.c_str(), message.size()) < 0)
    {
        printf("%s\n", "write data");
    }
}

void Socket::socketClose()
{
    close(sock);
    printf("close\n");
}

int epoll_fd;
std::map<int, Game *> games = {};

void handleMessage(std::string mess, Socket *sock);
/*
The function readData() is a loop that waits for incoming data on a socket using the epoll_wait() function. The function is passed an epoll_event struct, a pointer to a Socket object, and waits for an event of type EPOLLIN, indicating that the socket is ready for reading.

When data is available, the function calls the readData() method of the Socket object, passing in the buffer, size and socket file descriptor. The method reads the data from the socket and stores it in the nowReaded variable.

Then, the function iterates through the message list of the Socket object and calls the handleMessage() function passing in the message and the socket object. The handleMessage() function is responsible for processing the received message.

If an exception is caught, the function prints the exception message, 
removes the socket from the epoll instance, closes the socket, 
and deletes the Socket object. If the exception message is "Bad id", 
"Bad user" or "Game already started" the function calls the onHostDisconnected() 
or onPlayerDisconnected() function of the game and removes the game from the list of games.
*/
void readData()
{
    epoll_event event;
    Socket *sock;

    while (true)
    {
        epoll_wait(epoll_fd, &event, 1, -1);
        if (event.events & EPOLLIN)
        {

            sock = (Socket *)event.data.ptr;
            try
            {
                sock->readData();

                for (const auto &i : sock->message)
                {
                    handleMessage(i, sock);
                }
                sock->message.clear();
            }
            catch (const char *e)
            {
                printf("exception: %s\n", e);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock->sock, NULL);
                sock->socketClose();
                if (!std::strcmp(e, "Bad id") && !std::strcmp(e, "Bad user") && !std::strcmp(e, "Game already started"))
                {
                    if (sock->game->isHost(sock->sock))
                    {
                        sock->game->onHostDisconnected();
                        games.erase(sock->game->getGameId());
                        delete sock->game;
                    }
                    else
                    {
                        sock->game->onPlayerDisconnected(sock->sock);
                    }
                }
                delete sock;
            }
        }
    }
}
class Game;
class Socket;

// soket servera
int servFd;

// soket klienta
std::unordered_set<int> clientFds;

// przechwytywanie SIGINT
void ctrl_c(int);

// konwertacja cstring to port
uint16_t readPort(char *txt);

// ustawienie SO_REUSEADDR
void setReuseAddr(int sock);

void readData();

int generateGameId();

bool isIdExists(int id);
/*
 It creates an epoll file descriptor, sets up a socket and binds it to a specified port, 
 and enters a listening mode. 
 The function then creates a separate thread to handle incoming data on the socket using the readData function. 
 In the main thread, it continuously accepts new connections on the socket and 
 adds them to the epoll file descriptor using epoll_ctl with the EPOLLIN event flag.
 When a new connection is accepted, it creates a new instance of the Socket class, 
 sets the client file descriptor on it, and sets the event.data.ptr to the Socket instance. 
 The program also sets up signal handlers for the SIGINT and SIGPIPE signals. 
 When the program receives a SIGINT signal, the ctrl_c function is called and the program exits gracefully. 
 When the program receives a SIGPIPE signal, it is ignored and the program continues running.
*/
int main()
{
    epoll_fd = epoll_create1(0);

    if (epoll_fd == -1)
    {
        fprintf(stderr, "Błąd podczas tworzenia deskryptora epoll\n");
    }

    // ustawienie i sprawdzenie dostępności potru
    char portNumber[] = "5050";
    auto port = readPort(portNumber);

    // tworzenie soketa
    servFd = socket(AF_INET, SOCK_STREAM, 0);
    if (servFd == -1)
        error(1, errno, "socket failed");

    // przechwytywanie przerywania ctrl+c
    signal(SIGINT, ctrl_c);
    // Zapobieganie wpisaniu martwych gniazd w zapis
    signal(SIGPIPE, SIG_IGN);

    setReuseAddr(servFd);

    epoll_event event;
    event.events = EPOLLIN;

    // powiązanie z dowolnym adresem i portem podanym w argumentach
    sockaddr_in serverAddr{.sin_family = AF_INET, .sin_port = htons((short)port), .sin_addr = {INADDR_ANY}};
    int res = bind(servFd, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (res)
        error(1, errno, "powiązanie się nie powiodło");

    // enter listening mode
    res = listen(servFd, 1);
    if (res)
        error(1, errno, "błąd podczas nasłuchiwania");

    std::thread t1(readData);

    while (true)
    {
        sockaddr_in clientAddr{0};
        socklen_t clientAddrSize = sizeof(clientAddr);

        // akceptacja połączenia
        auto clientFd = accept(servFd, (sockaddr *)&clientAddr, &clientAddrSize);
        if (clientFd == -1)
            error(1, errno, "błąd akceptacji połączenia");

        // fcntl(clientFd, F_SETFL, fcntl(clientFd, F_GETFL) | O_NONBLOCK);
        fcntl(clientFd, F_SETFL, O_NONBLOCK, 1);

        Socket *socket = new Socket();
        socket->sock = clientFd;
        event.data.ptr = socket;

        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientFd, &event);

        printf("nowe połączenie: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
    }

    t1.join();
}
/*
The readPort function converts a string representing a port number to an unsigned 16-bit integer. 
It first uses the strtol function to convert the string to a long integer, 
and assigns the result to the variable port. 
Then, it checks if the string contains any non-numeric characters, 
by checking if the value of the pointer ptr is not equal to zero. 
If this is the case, it means that the string contains non-numeric characters 
and the function calls the error function with the message "niedozwolony argument %s", txt, 
which means "invalid argument %s". 
Next, it checks if the port number is less than 1 or greater than the maximum value that can be stored in an unsigned 16-bit integer, 
it also calls the error function with the same message. 
If the port number is valid, the function returns it.
*/
uint16_t readPort(char *txt)
{
    char *ptr;
    auto port = strtol(txt, &ptr, 10);
    if (*ptr != 0 || port < 1 || (port > ((1 << 16) - 1)))
        error(1, 0, "niedozwolony argument %s", txt);
    return port;
}

/*
The function setReuseAddr(int sock) sets the SO_REUSEADDR option on the given socket sock. 
This option allows the socket to bind to a port that is still waiting for all connection drops to complete. 
This is useful for server applications that want to quickly reuse a recently released port without waiting for the operating system to release it. 
The function uses the setsockopt() function to set the option on the socket. 
If the call to setsockopt() fails, the function throws an error with the message "setsockopt failed" along with the corresponding error code.
*/
void setReuseAddr(int sock)
{
    const int one = 1;
    int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if (res)
        error(1, errno, "setsockopt failed");
}
/*
catching ctrl+c, closing clients fd
*/
void ctrl_c(int)
{
    for (int clientFd : clientFds)
        close(clientFd);
    close(servFd);
    printf("Wyłączenie serwera\n");
    exit(0);
}

/*
generate random id
*/
int generateGameId()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(100000, 999999);
    return distr(gen);
}

/*
This is a function that handles different types of messages passed as a string argument. 
It starts by looking for specific keywords in the message using the find() function
of the std::string class. Depending on the keyword found, it performs different actions, 
such as creating a new game, sending answers, joining a game, starting a game, 
moving to the next question, checking student answers, ending a game, 
and handling a countdown timer. The function uses the std::stoi() 
function to convert strings to integers and the insert() method of the std::map container to add new elements to the games map. 
It also uses the writeData() function to send responses to clients and the broadcastToUsers() function to send messages to all connected users in a game.
*/
void handleMessage(std::string strMess, Socket *sock)
{
    size_t foundGameName = strMess.find("\\create_game\\");
    size_t foundSendAnswers = strMess.find("\\send_answers\\");
    size_t foundNewJoinPlayers = strMess.find("\\join_game\\");
    size_t foundStartGame = strMess.find("\\start_game\\");
    size_t foundNextQuastion = strMess.find("\\next_question\\");
    size_t foundStudentAnswer = strMess.find("\\answer_student\\");
    size_t foundEndGame = strMess.find("\\end_game\\");
    size_t foundTimer = strMess.find("\\countdown_end\\");
    if (foundGameName == 0)
    {
        size_t foundQuantity = strMess.find("\\quantity\\");
        size_t foundTime = strMess.find("\\time\\");
        std::string gameName = std::string(&strMess[13], &strMess[foundQuantity]);
        std::string gameQuantity = std::string(&strMess[foundQuantity + 10], &strMess[foundTime]);
        std::string gameTime = strMess.substr(foundTime + 6);
        int gameId = generateGameId();
        while (isIdExists(gameId))
        {
            gameId = generateGameId();
        }
        Game *myGame = new Game(gameName, gameId, std::stoi(gameQuantity), std::stoi(gameTime), sock);
        sock->game = myGame;
        sock->writeData(std::to_string(gameId));
        games.insert({gameId, myGame});
    }
    else if (foundSendAnswers == 0)
    {
        size_t foundAnswers = strMess.find("\\answers\\");
        bool *answers = new bool[4];
        std::string gameId = std::string(&strMess[17], &strMess[foundAnswers]);
        for (int i = 0; i < 4; i++)
        {
            if (strMess.at(foundAnswers + 9 + 2 * i) == '1')
                answers[i] = true;
            else
                answers[i] = false;
        }
        games[std::stoi(gameId)]->setAnswer(answers);
    }
    else if (foundNewJoinPlayers == 0)
    {
        size_t foundUser = strMess.find("\\user\\");
        std::string gameId = std::string(&strMess[14], &strMess[foundUser]);
        std::string gameUser = strMess.substr(foundUser + 6);
        try
        {
            std::stoi(gameId);
        }
        catch (std::invalid_argument &e)
        {
            sock->writeData("\\error\\id");
            return;
        }
        if (!isIdExists(std::stoi(gameId)))
        {
            sock->writeData("\\error\\id");
            throw "Złe id";
        }
        else
        {
            Game *game = games[std::stoi(gameId)];
            if (!game->isValidUser(gameUser))
            {
                sock->writeData("\\error\\user");
                throw "Zły użytkownik";
            }
            else if (game->getGameStarted())
            {
                sock->writeData("\\error\\started");
                throw "Gra już się zaczeła";
            }
            else
            {
                std::string response = "\\ok\\game_name\\" + game->gameName + "\\quantity\\" +
                                       std::to_string(game->getQNumber()) + "\\time\\" +
                                       std::to_string(game->getTime());
                sock->game = game;
                sock->writeData(response);
                sock->writeData(game->getUsersMessage());
                game->addUser(gameUser, sock);
                game->broadcastToUsers("\\add_user\\" + gameUser, true);
            }
        }
    }
    else if (foundStartGame == 0)
    {
        size_t foundId = strMess.find("\\id\\");
        std::string gameId = strMess.substr(foundId + 4);
        Game *game = games[std::stoi(gameId)];
        if (!game->startGame())
        {
            sock->writeData("\\error\\no_users");
        }
        else
        {
            sock->writeData("\\ok\\start_game\\");
            game->broadcastToUsers("\\next_question\\", false);
        }
    }
    else if (foundNextQuastion == 0)
    {
        size_t foundId = strMess.find("\\id\\");
        std::string gameId = strMess.substr(foundId + 4);
        Game *game = games[std::stoi(gameId)];
        game->nextQuestion();
        game->broadcastToUsers("\\next_question\\", false);
    }
    else if (foundStudentAnswer == 0)
    {
        size_t foundId = strMess.find("\\id\\");
        size_t foundNumber = strMess.find("\\number\\");
        size_t foundAnswer = strMess.find("\\answer\\");
        std::string gameId = std::string(&strMess[foundId + 4], &strMess[foundNumber]);
        std::string number = std::string(&strMess[foundNumber + 8], &strMess[foundAnswer]);
        std::string answer = strMess.substr(foundAnswer + 8);
        Game *game = games[std::stoi(gameId)];
        game->checkAnswer(sock->sock, std::stoi(number) - 1, std::stoi(answer));
    }
    else if (foundEndGame == 0)
    {
        size_t foundId = strMess.find("\\id\\");
        std::string gameId = strMess.substr(foundId + 4);
        Game *game = games[std::stoi(gameId)];
        game->broadcastToUsers("\\end_game\\", false);
        game->disconnectAllUsers();
        sock->socketClose();
        games.erase(sock->game->getGameId());
        delete sock->game;
    }
    else if (foundTimer == 0)
    {
        size_t foundId = strMess.find("\\id\\");
        std::string gameId = strMess.substr(foundId + 4);
        Game *game = games[std::stoi(gameId)];
        game->socket->writeData("\\question_end\\admin");
        game->broadcastToUsers("\\question_end\\user", false);
        game->broadcastToUsers(game->getUserPoints(), true);
    }
}
/*
checking id, if exist,
return true
*/
bool isIdExists(int id)
{
    if (games.empty())
        return false;
    std::map<int, Game *>::iterator it;
    it = games.find(id);
    if (it != games.end())
    {
        return true;
    }
    return false;
}