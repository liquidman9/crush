#include <server/Server.h>
#include <shared/util/SharedUtils.h>

using namespace std;


int main() {
	int port = DEFAULT_PORT;
	ConfigSettings::config->getValue("network_serverPort", port);

	Server server((unsigned short) port);	
	server.start();

	string input;
	for(;;) {
		std::getline(cin, input);
		vector<string> input_tokens;
		shared::utils::tokenize(input, input_tokens, " ");
		if(input_tokens[0] == "rs"){
			server.restart();
		} else if (input_tokens[0] == "rl") {
			server.reload();
		} else if (input_tokens[0] == "p") {
			server.pause();
		} else if (input_tokens[0] == "q") {
			break;
		} else if (input_tokens[0] == "s") {
			server.startGame();
		} else  {
			if (input_tokens[0] == "quit") {
				break;
			}
			if (input_tokens[0] == "server") {
				if (input_tokens[1] == "start") { 
					server.startGame();
				} else if (input_tokens[1] == "reload") {
					server.reload();
				} else if (input_tokens[1] == "pause") {
					server.pause();
				} else if (input_tokens[1] == "restart") {
					server.restart();
				}
			}
			if (input_tokens[0] == "game") {
				if (input_tokens[1] == "num_entities") {
					cout << "Number of entities: " << server.getGameState().size() << endl;
				} else {
					if (input_tokens[1] == "entity" && input_tokens.size() > 2) {
						try {
							ServerEntity * entity = dynamic_cast<ServerEntity*>(server.getGameState().getEntityById(std::stoi(input_tokens[2])).get());
							if (entity != NULL) {
								cout << "Entity " + input_tokens[2] << endl;
								entity->print();
							} else {
								cout << "Entity with given ID does not exist!" << endl;
							}
						} catch (std::exception) {
							cout << "ID was not specified/in proper format." << endl;
						}
					}
				}
			}
		}
	}

	return 0;
}
