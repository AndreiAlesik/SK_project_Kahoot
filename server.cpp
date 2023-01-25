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
class User{
    private:
        int score;

    public:
        User(std::string, Socket*);

        std::string name;
        Socket* socket;
        bool isClosed = false;
        
        int getScore();
        void setScore(int);
        void incrementScore();

};
User::User(std::string name, Socket* socket){
    this->score = 0;
    this->name = name;
    this->socket = socket;
}



int User::getScore(){
    return this->score;
}

void User::setScore(int n){
    this->score = n;
}

void User::incrementScore(){
    this->score++;
}
class Game{
    private:
        int qNumber;
        int gameId;
        int time;
        bool** answers;
        bool isGameReady;
        bool isGameStarted;
        int nSetAnswers;
        int nQuizQuestion;

    public:
        Game(std::string, int, int, int, Socket*);

        std::string gameName;
        std::vector<User*> users;
        Socket* socket;

        int getQNumber();
        void setQNumber(int);

        int getTime();
        
        int getGameId();
        void setGameId(int);

        bool* getAnswer(int);
        void setAnswer(bool[4]);

        bool getGameReady();
        void setGameReady();

        bool getGameStarted();

        void disconnectAllUsers();

        bool isValidUser(std::string);
        bool addUser(std::string, Socket*);
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

        //a - [0,3];
        void checkAnswer(int, int q, int a);
};

class Socket{
    public:
        int sock;
        char buffer[512];
        std::string nowReaded;
        int messageSize = -1;
        std::list<std::string> message;
        Game* game = nullptr;


        void readData();
        void writeData(std::string);
        void socketClose();
};

Game::Game(std::string name, int gameId, int qNumber, int time, Socket* socket){
    this->gameName = name;
    this->gameId = gameId;
    this->qNumber = qNumber;
    this->time = time;
    this->isGameReady = false;
    this->isGameStarted = false;
    this->answers = new bool*[qNumber];
    this->socket = socket;
    this->nQuizQuestion = 0;
    for(int i = 0; i < qNumber; i++){
        this->answers[i] = new bool[4];
    }
    this->users = {};
    this->nSetAnswers = 0;
}

int Game::getQNumber(){
    return this->qNumber;
}

void Game::setQNumber(int n){
    this->qNumber = n;
}

int Game::getTime(){
    return this->time;
}

int Game::getGameId(){
    return this->gameId;
}

void Game::setGameId(int id){
    this->gameId = id;
}

bool* Game::getAnswer(int n){
    return this->answers[n];
}

void Game::setAnswer(bool arr[4]){
    if(this->nSetAnswers < this->qNumber){
        this->answers[this->nSetAnswers] = arr;
        this->nSetAnswers++;
    }   
}

bool Game::getGameReady(){
    return this->isGameReady;
}

void Game::setGameReady(){
    this->isGameReady = true;
}

bool Game::isValidUser(std::string name){
    if(this->users.size() == 0)
        return true;
    else{
        for(User* el : this->users){
            if(name == el->name)
                return false;
        }
        return true;
    }
}

bool Game::addUser(std::string name, Socket* sock){
    if(isValidUser(name)){
        this->users.push_back(new User(name, sock));
        return true;
    }
    return false;
}

int Game::userPosition(std::string name){
    int i = 0;
    for(User* el : this->users){
            if(name == el->name)
                return i;
            i++;
        }
    return -1;
}

bool Game::deleteUser(std::string name){
    if(userPosition(name) != -1){
        this->users.erase(this->users.begin() + userPosition(name));
        return true;
    }
    return false;
}

std::string Game::getUsersMessage(){
    std::string mess = "\\users";
    if(this->users.size() == 0)
        mess += "\\";
    else{
        for(auto &user : this->users){
            mess += "\\" + user->name;
        }
    }
    return mess;
}

void Game::broadcastToUsers(std::string mess, bool withAdmin){
    if(withAdmin)
        socket->writeData(mess);
    for(auto &user : this->users){
        if(!user->isClosed)
            user->socket->writeData(mess);
    }
}

void Game::checkAnswer(int sock, int q, int a){
    if(q == this->nQuizQuestion){
        if(this->answers[q][a] == true){
            for(User* el : this->users){
                    if(sock == el->socket->sock){
                        el->incrementScore();
                        break;
                    }
            }
        }
    }
}

bool Game::isHost(int sock){
    if(sock == socket->sock)
        return true;
    else 
        return false;
}

void Game::onHostDisconnected(){
    broadcastToUsers("\\unreachable_host\\", false);
    disconnectAllUsers();
}

void Game::onPlayerDisconnected(int sock){
    std::string user;
    bool before = false;
    for(User* el : this->users){
        if(sock == el->socket->sock){ 
            if(!isGameStarted){
                user = el->name;
                before = true;
            }
            else{
                el->isClosed = true;
            }
            break;
        }
    }
    if(before){
    deleteUser(user);
    broadcastToUsers("\\delete_user\\"+user, true);
    }
}

bool Game::startGame(){
    if(users.empty())
        return false;
    else
    {
        setGameReady();
        this->isGameStarted = true;
        return true;
    }
}

bool Game::getGameStarted(){
    return this->isGameStarted;
}

void Game::nextQuestion(){
    this->nQuizQuestion++;
}

std::string Game::getUserPoints(){
    std::string command = "\\game_results";
    for(User* el : this->users){
        command += "\\name\\" + el->name;
        command += "\\score\\" + std::to_string(el->getScore());
    }
    return command;
}

void Game::disconnectAllUsers(){
    for(User* el : this->users){
        el->socket->socketClose();
    }
}



void Socket::readData(){
    std::memset(buffer, 0, 512);
    int read_size = read(sock, buffer, sizeof(buffer));
    
    if(read_size < 1){
        fprintf(stderr, "%d disconnected\n", sock);
        throw "client disconnected";
    }
    nowReaded += std::string(buffer, read_size);
    while(true){
        if(messageSize < 0){
            if(nowReaded.size() > sizeof(int)){
                messageSize = ntohl(*(int*)nowReaded.c_str());
                nowReaded = nowReaded.substr(sizeof(int));
            }
            else
                break;
        }  
        int len = nowReaded.length();
        if(messageSize <= len){
            message.push_back(nowReaded.substr(0,messageSize));
            nowReaded = nowReaded.substr(messageSize);
            messageSize = -1;
        }
        else{
            break;
        }
    }
}

void Socket::writeData(std::string message){
    int size = htonl(message.size());
    if(write(sock, (char*)&size, sizeof(int)) < 0){
        printf("%s\n", "write data");
    }
    if(write(sock, message.c_str(), message.size()) < 0){
        printf("%s\n", "write data");
    }
}

void Socket::socketClose(){
    close(sock);
    printf("close\n");
}


int epoll_fd;
std::map <int, Game*> games = {};

void handleMessage(std::string mess, Socket* sock);

void readData(){
	epoll_event event;
	Socket* sock;
	
	while(true){
		epoll_wait(epoll_fd, &event, 1, -1);
		if(event.events & EPOLLIN){
			
			sock = (Socket*)event.data.ptr;
			try{
				sock->readData();

				for(const auto &i : sock->message){
					handleMessage(i, sock);
				}
				sock->message.clear();
			}
			catch(const char* e){
				printf("exception: %s\n", e);
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock->sock, NULL);
				sock->socketClose();
				if(!std::strcmp(e, "Bad id") && !std::strcmp(e, "Bad user") && !std::strcmp(e, "Game already started")){
					if(sock->game->isHost(sock->sock)){
						sock->game->onHostDisconnected();
						games.erase(sock->game->getGameId());
						delete sock->game;
					}
					else{
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
uint16_t readPort(char * txt);

// ustawienie SO_REUSEADDR
void setReuseAddr(int sock);

void readData();

int generateGameId();



bool isIdExists(int id);




int main(){
	epoll_fd = epoll_create1(0);

	if(epoll_fd == -1){
		fprintf(stderr, "Błąd podczas tworzenia deskryptora epoll\n");
	}

	// ustawienie i sprawdzenie dostępności potru
	char portNumber[] = "5050";
	auto port = readPort(portNumber);
	
	// tworzenie soketa
	servFd = socket(AF_INET, SOCK_STREAM, 0);
	if(servFd == -1) error(1, errno, "socket failed");
	
	// przechwytywanie przerywania ctrl+c
	signal(SIGINT, ctrl_c);
	// Zapobieganie wpisaniu martwych gniazd w zapis
	signal(SIGPIPE, SIG_IGN);
	
	setReuseAddr(servFd);

	epoll_event event;
	event.events = EPOLLIN;
	
	// powiązanie z dowolnym adresem i portem podanym w argumentach
	sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
	int res = bind(servFd, (sockaddr*) &serverAddr, sizeof(serverAddr));
	if(res) error(1, errno, "powiązanie się nie powiodło");
	
	// enter listening mode
	res = listen(servFd, 1);
	if(res) error(1, errno, "błąd podczas nasłuchiwania");
	


	std::thread t1(readData);
	
	while(true){
		sockaddr_in clientAddr{0};
		socklen_t clientAddrSize = sizeof(clientAddr);
		
		// akceptacja połączenia
		auto clientFd = accept(servFd, (sockaddr*) &clientAddr, &clientAddrSize);
		if(clientFd == -1) error(1, errno, "błąd akceptacji połączenia");

		// fcntl(clientFd, F_SETFL, fcntl(clientFd, F_GETFL) | O_NONBLOCK); 
		fcntl(clientFd, F_SETFL, O_NONBLOCK, 1);

		Socket* socket = new Socket();
		socket->sock = clientFd;
		event.data.ptr = socket;
		
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientFd, &event);

		printf("nowe połączenie: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);

	}

	t1.join();
	
}

uint16_t readPort(char * txt){
	char * ptr;
	auto port = strtol(txt, &ptr, 10);
	if(*ptr!=0 || port<1 || (port>((1<<16)-1))) error(1,0,"niedozwolony argument %s", txt);
	return port;
}

void setReuseAddr(int sock){
	const int one = 1;
	int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	if(res) error(1,errno, "setsockopt failed");
}

void ctrl_c(int){
	for(int clientFd : clientFds)
		close(clientFd);
	close(servFd);
	printf("Wyłączenie serwera\n");
	exit(0);
}


int generateGameId(){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(100000, 999999);
	return distr(gen);
}

void handleMessage(std::string strMess, Socket* sock){
	size_t foundGameName = strMess.find("\\create_game\\");
	size_t foundSendAnswers = strMess.find("\\send_answers\\");
	size_t foundNewJoinPlayers = strMess.find("\\join_game\\");
	size_t foundStartGame = strMess.find("\\start_game\\");
	size_t foundNextQuastion = strMess.find("\\next_question\\");
	size_t foundStudentAnswer = strMess.find("\\answer_student\\");
	size_t foundEndGame = strMess.find("\\end_game\\");
	size_t foundTimer = strMess.find("\\countdown_end\\");
	if(foundGameName == 0){
		size_t foundQuantity = strMess.find("\\quantity\\");
		size_t foundTime = strMess.find("\\time\\");
		std::string gameName = std::string(&strMess[13], &strMess[foundQuantity]);
		std::string gameQuantity = std::string(&strMess[foundQuantity+10], &strMess[foundTime]);
		std::string gameTime = strMess.substr(foundTime+6);
		int gameId = generateGameId();
		while(isIdExists(gameId)){
			gameId = generateGameId();
		}
		Game* myGame = new Game(gameName, gameId, std::stoi(gameQuantity), std::stoi(gameTime), sock);
		sock->game = myGame;
		sock->writeData(std::to_string(gameId));
		games.insert({gameId, myGame});
	}
	else if(foundSendAnswers == 0){
		size_t foundAnswers = strMess.find("\\answers\\");
		bool* answers = new bool[4];
		std::string gameId = std::string(&strMess[17], &strMess[foundAnswers]);
		for(int i = 0; i < 4; i++){
			if(strMess.at(foundAnswers+9+2*i) == '1')
				answers[i] = true;
			else	
				answers[i] = false;
		}
		games[std::stoi(gameId)]->setAnswer(answers);		
	}
	else if(foundNewJoinPlayers == 0){
		size_t foundUser = strMess.find("\\user\\");
		std::string gameId = std::string(&strMess[14], &strMess[foundUser]);
		std::string gameUser = strMess.substr(foundUser+6);
		try{
			std::stoi(gameId);
		}
		catch(std::invalid_argument& e){
			sock->writeData("\\error\\id");
			return;
		}
		if(!isIdExists(std::stoi(gameId))){
			sock->writeData("\\error\\id");
			throw "Złe id";
		}
		else{
			Game* game = games[std::stoi(gameId)];
			if(!game->isValidUser(gameUser)){
				sock->writeData("\\error\\user");
				throw "Zły użytkownik";
			}
			else if(game->getGameStarted()){
				sock->writeData("\\error\\started");
				throw "Gra już się zaczeła";
			}
			else{
				std::string response = "\\ok\\game_name\\" + game->gameName + "\\quantity\\" + 
										std::to_string(game->getQNumber()) + "\\time\\" + 
										std::to_string(game->getTime());
				sock->game = game;
				sock->writeData(response);
				sock->writeData(game->getUsersMessage());
				game->addUser(gameUser, sock);
				game->broadcastToUsers("\\add_user\\"+gameUser, true);
			}
		} 
	}
	else if(foundStartGame == 0){
		size_t foundId = strMess.find("\\id\\");
		std::string gameId = strMess.substr(foundId+4);
		Game* game = games[std::stoi(gameId)];
		if(!game->startGame()){
			sock->writeData("\\error\\no_users");
		}
		else{
			sock->writeData("\\ok\\start_game\\");
			game->broadcastToUsers("\\next_question\\", false);
		}
	}
	else if(foundNextQuastion == 0){
		size_t foundId = strMess.find("\\id\\");
		std::string gameId = strMess.substr(foundId+4);
		Game* game = games[std::stoi(gameId)];
		game->nextQuestion();
		game->broadcastToUsers("\\next_question\\", false);
	}
	else if(foundStudentAnswer == 0){
		size_t foundId = strMess.find("\\id\\");
		size_t foundNumber = strMess.find("\\number\\");
		size_t foundAnswer = strMess.find("\\answer\\");
		std::string gameId = std::string(&strMess[foundId + 4], &strMess[foundNumber]);
		std::string number = std::string(&strMess[foundNumber + 8], &strMess[foundAnswer]);
		std::string answer = strMess.substr(foundAnswer + 8);
		Game* game = games[std::stoi(gameId)];
		game->checkAnswer(sock->sock, std::stoi(number) - 1, std::stoi(answer));
	}
	else if(foundEndGame == 0){
		size_t foundId = strMess.find("\\id\\");
		std::string gameId = strMess.substr(foundId + 4);
		Game* game = games[std::stoi(gameId)];
		game->broadcastToUsers("\\end_game\\", false);
		game->disconnectAllUsers();
		sock->socketClose();
		games.erase(sock->game->getGameId());
		delete sock->game;
	}
	else if(foundTimer == 0){
		size_t foundId = strMess.find("\\id\\");
		std::string gameId = strMess.substr(foundId + 4);
		Game* game = games[std::stoi(gameId)];
		game->socket->writeData("\\question_end\\admin");
		game->broadcastToUsers("\\question_end\\user", false);
		game->broadcastToUsers(game->getUserPoints(), true);
	}
}

bool isIdExists(int id){
	if(games.empty())
		return false;
	std::map<int,Game*>::iterator it;
	it = games.find(id);
	if(it != games.end()){
		return true;
	}
	return false;
}