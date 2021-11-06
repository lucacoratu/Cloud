#include "cloudpch.h"
#include "DiffieHellmanAPI.h"

#include "dhexchange.c"

DiffieHellmanKeyPair DiffieHellmanAPI::GenerateKeyPair()
{
    time_t seed;
    time(&seed);
    srand((unsigned int)seed);

    DiffieHellmanKeyPair keyPair;

    DH_generate_key_pair(keyPair.publicKey, keyPair.privateKey);

    return keyPair;
}

unsigned char* DiffieHellmanAPI::GenerateSecret(DH_KEY privateKey, DH_KEY otherPublicKey)
{
    DH_KEY secret;
    DH_generate_key_secret(secret, privateKey, otherPublicKey);
    return secret;
}
