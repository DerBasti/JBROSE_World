#include "WorldServer.h"
#include "FileTypes/AIP.h"

int main() {
	WorldServer server(29200);
	server.loadEncryption();
	server.startAcceptingClients();
	return 0;
}