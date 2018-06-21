#include "WorldServer.h"

int main() {
	WorldServer server(29200);
	server.loadEncryption();
	server.startAcceptingClients();
	return 0;
}