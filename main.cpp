#include <iostream>
#include <ctime>
#include <SFML/Network.hpp>
void runServer(unsigned short port);
void runClient(unsigned short port);
void printTime();
int main()
{
	char isHost;
	do {
		std::cout << "Are you the host? (y / n)\n";
		std::cin >> isHost;
		if (isHost == 'y') {
			runServer(5896);
		}
		else if (isHost == 'n') {
			runClient(5896);
		}
		else {
			std::cout << "Please enter y for yes, n for no." << std::endl;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	} while (isHost != 'y' && isHost != 'n');
}

void runServer(unsigned short port)
{
	sf::TcpListener listener;

	listener.listen(sf::Socket::AnyPort);

	std::cout << "Server is listening for connections on port " << listener.getLocalPort() << "..." << std::endl;

	while (true) {

		sf::TcpSocket client;

		//client.setBlocking(false);

		bool clientDisconnected = false;

		if (listener.accept(client) == sf::Socket::Done) {
			std::cout << client.getRemoteAddress() << " has connected to the server." << std::endl;
		}
		else {
			std::cout << "A networking error has occured." << std::endl;
		}

		char cName[17] = {0};
		char cMes[500] = {0};

		std::size_t nameSize;
		std::size_t messageSize;
		if (client.receive(cName, sizeof(cName), nameSize) != sf::Socket::Done) {
			std::cout << "Unable to read client username." << std::endl;
		}

		while (!clientDisconnected) {
			if (client.receive(cMes, sizeof(cMes), messageSize) == sf::Socket::Done) {
				std::cout << cName << "> " << cMes << std::endl;
			}
			else {
				clientDisconnected = true;
			}
		}

		if (clientDisconnected) {
			std::cout << client.getRemoteAddress() << " has disconnected." << std::endl;
			clientDisconnected = false;
		}

	}
}

void runClient(unsigned short port)
{
	sf::IpAddress serverIP;
	unsigned short portChoice;
	bool isConnected = false;

	char cMesOut[500] = "";
	char cName[17] = "";

	sf::Socket::Status status;
	do {
		std::cout << "Pick a username: ";
		std::cin >> cName;
		if (std::strlen(cName) > 16) {
			std::cout << "Name must be 16 characters or less." << std::endl;
		}
	} while (std::strlen(cName) > 16);

	do {
		std::cout << "Enter server address: ";
		std::cin >> serverIP;
		if (serverIP == "") {
			std::cout << "Server address cannot be blank" << std::endl;
		}
		std::cout << "Enter server port: ";
		std::cin >> portChoice;
		std::cin.ignore(1000, '\n');
		sf::TcpSocket socket;

		status = socket.connect(serverIP, portChoice);

		if (status == sf::Socket::Done) {
			socket.send(cName, sizeof(cName));
			std::cout << "Welcome to the server, " << cName << "! ";
			std::cout << "You are connected to " << socket.getRemoteAddress() << " on port " << socket.getRemotePort() << std::endl;
			printTime();
			isConnected = true;
			while (isConnected) {				
				std::cout << cName << "> ";
				std::cin.getline(cMesOut, 500);			
				if (socket.send(cMesOut, sizeof(cMesOut)) == sf::Socket::Done) {
					//std::cout << "Message sent successfully." << std::endl;
				}
				else {
					std::cout << "Message could not be delivered. Please check your internet connection." << std::endl;
					isConnected = false;
					std::cout << "You have been disconnected from the server." << std::endl;
					std::cout << std::endl;
				}
			}
		}
		else {
			std::cout << "Failed to connect to server." << std::endl;
		}

	} while (true);
}

void printTime()
{
	int totalSeconds = time(0);
	int currentSeconds = totalSeconds % 60;

	int totalMinutes = totalSeconds / 60;
	int currentMinutes = totalMinutes % 60;

	int totalHours = totalMinutes / 60;
	int currentHours = totalHours % 24;

	std::cout << "The current time is: " << currentHours << ":" << currentMinutes << ":" << currentSeconds << " GMT" << std::endl;
}