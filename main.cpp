#include <iostream>
#include <ctime>
#include <SFML/Network.hpp>
void runServer(unsigned short port);
void runClient(unsigned short port);
void printTime();
std::string clientName;
//sf::Packet packet;
sf::TcpSocket client;

int main()
{
	bool isHost;
	std::cout << "Are you the host?\n";
	std::cin >> isHost;
	if (isHost) {
		runServer(5896);
	}
	else {
		runClient(5896);
	}
}

void runServer(unsigned short port)
{
	sf::TcpListener listener;

	listener.listen(sf::Socket::AnyPort);

	sf::Packet packet;

	

	client.setBlocking(false);

	

	std::cout << "Server is listening for connections on port " << listener.getLocalPort() << "..." << std::endl;

	while (true) {
		
		if (listener.accept(client) == sf::Socket::Done) {
			std::cout << client.getRemoteAddress() << " has connected to the server." << std::endl;			
		}
		else {
			std::cout << "A networking error has occured." << std::endl;
		}

		std::string clientMessage;
		if (client.receive(packet) == sf::Socket::Done) {
			std::cout << "Message received." << std::endl;
			std::cout << clientName << "> " << clientMessage << std::endl;
		}
		else {
			std::cout << "Message was not received." << std::endl;
		}
		
		if (packet >> clientName) {
			std::cout << "Success!";
		}
		else {
			std::cout << "Error: failed to read client name!";
		}

		if (packet >> clientMessage) {
			std::cout << "Success!";
		}
		else {
			std::cout << "Error: failed to read message data!";
		}

		
	}	
}

void runClient(unsigned short port)
{
	sf::IpAddress serverIP;
	unsigned short portChoice;

	std::cout << "Pick a username: ";
	std::cin >> clientName;
	std::cout << "Enter server address: ";
	std::cin >> serverIP;
	std::cout << "Enter server port: ";
	std::cin >> portChoice;
	
	sf::TcpSocket socket;

	sf::Packet packet;

	

	std::string clientMessage;

	sf::Socket::Status status = socket.connect(serverIP, portChoice);

	if (status == sf::Socket::Done) {
		packet << clientName;
		client.send(packet);
		std::cout << "Welcome to the server, " << clientName << "! ";
		std::cout << "You are connected to " << socket.getRemoteAddress() << " on port " << socket.getRemotePort() << std::endl;
		printTime();
	}
	else {
		std::cout << "Failed connection." << std::endl;
	}

	while (true) {
		std::cout << "Type message: ";
		std::cin >> clientMessage;
		packet << clientMessage;
		if (client.send(packet) == sf::Socket::Done) {
			std::cout << "Message sent successfully." << std::endl;
			packet.clear();
		}
		else {
			std::cout << "Message could not be delivered. Please check your internet connection." << std::endl;
		}
	}
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