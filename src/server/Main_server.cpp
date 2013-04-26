#include <server/Server.h>

using namespace std;


int main() {
	int port = DEFAULT_PORT;
	ConfigSettings::config->getValue("network_serverPort", port);

	Server server((unsigned short) port);	
	server.start();

	string input;
	for(;;) {
		cin >> input;
		if(input == "rs"){
			server.restart();
		} else if (input == "rl") {
			server.reload();
		} else if (input == "q") {
			break;
		}
	}

	return 0;
}
