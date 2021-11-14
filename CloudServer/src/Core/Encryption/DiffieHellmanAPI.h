#ifndef DIFFIEHELLMANAPI_H
#define DIFFIEHELLMANAPI_H

/*
*	Wrapper class over a implementation of the Diffie-Hellman algorithm
*	This Wrapper should be able to receive a public key and generate the secret which will be saved after in the client data
*	As well as generate 128 bit key pairs
*/

#include "dhexchange.h"

typedef struct {
	DH_KEY publicKey;
	DH_KEY privateKey;
} DiffieHellmanKeyPair;

class DiffieHellmanAPI {
private:

public:
	static DiffieHellmanKeyPair GenerateKeyPair();
	static void GenerateSecret(DH_KEY secret, DH_KEY privateKey, DH_KEY otherPublicKey);
};

static void _print_key(const char* name, const DH_KEY key) {
	int i;

	printf("%s=\t", name);
	/*for (i = DH_KEY_LENGTH - 1; i >= 0; i--) {
		printf("%02x", key[i]);
	}
	*/
	for (i = 0; i < DH_KEY_LENGTH - 1; i++) {
		printf("%02x", key[i]);
	}
	printf("\n");
}

#ifdef SV_DEBUG
#define SV_PRINT_KEY(name, key) _print_key((name), (key))
#else
#define SV_PRINT_KEY(x) 
#endif // SV_DEBUG


#endif
