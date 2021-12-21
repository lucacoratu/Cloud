#include "cloudpch.h"
#include "ClientData.h"

#define BUFFER_SIZE 32762

ClientData::ClientData(const std::string publicKey, const std::string privateKey)
	: publicKey(publicKey), privateKey(privateKey)
{
	this->secret = "";
	this->accountUsername = "";
	this->supportsEncyption = false;
}


ClientData::~ClientData()
{
	this->accountUsername.clear();
}

const std::string ClientData::GetPrivateKey() const
{
	return this->privateKey;
}

const std::string ClientData::GetSecret() const
{
	return this->secret;
}

const std::string ClientData::GetAccountUsername() const
{
	return this->accountUsername;
}

const std::string ClientData::GetActiveDirectory() const
{
	return this->currentDirectory;
}

const FileCursor ClientData::GetFileCursor(std::string filename) const
{
	FileCursor cursor = { "", 0 };

	for (size_t i = 0; i < this->filesToDownload.size(); i++)
	{
		if (filesToDownload[i].filename == filename)
			return filesToDownload[i];
	}

	return cursor;
}

const bool ClientData::IsFileInUploadList(const std::string& filename)
{
	for (auto& file : this->filesToUpload)	{
		if (file == filename)
			return true;
	}
	return false;
}

bool ClientData::SupportsEncryption()
{
	/*
	* If the secret has been generated it means that the client supports encrypted connection
	* The secret is generated when the server receives the public key from the client
	*/
	return (this->secret != "") ? true : false;
}

void ClientData::SetSupportsEncryption(const bool supportsEncryption)
{
	this->supportsEncyption = supportsEncryption;
}

void ClientData::SetSecret(const std::string& secret)
{
	this->secret = secret;
}

void ClientData::SetAccountUsername(const std::string& username)
{
	this->accountUsername = username;
}

void ClientData::ChangeCurrentDirectory(const std::string& currentDirectory)
{
	this->currentDirectory = currentDirectory;
	//std::cout << "Current directory: " << currentDirectory;
}

void ClientData::AddDownloadFile(std::string filename)
{
	std::ifstream in(filename);
	// get length of file:
	in.seekg(0, in.end);
	int length = in.tellg();
	in.seekg(0, in.beg);

	//Calculate the number of chunks it has
	uint32_t totalNumberChunks = length / BUFFER_SIZE;

	this->filesToDownload.push_back({ filename, 0, totalNumberChunks });
}

void ClientData::EraseDownloadFile(std::string filename)
{
	for (size_t i = 0; i < this->filesToDownload.size(); i++) {
		if (this->filesToDownload[i].filename == filename)
			this->filesToDownload.erase(this->filesToDownload.begin() + i);
	}
}

void ClientData::IncrementChunkNumber(std::string filename)
{
	for (size_t i = 0; i < this->filesToDownload.size(); i++) {
		if (this->filesToDownload[i].filename == filename)
			this->filesToDownload[i].fileChunkNumber++;
	}
}

void ClientData::AddUploadFile(const std::string& filename)
{
	this->filesToUpload.push_back(filename);
}

void ClientData::EraseUploadFile(const std::string& filename)
{
	for (size_t i = 0; i < this->filesToUpload.size(); i++) {
		if (this->filesToUpload[i] == filename) {
			this->filesToUpload.erase(this->filesToUpload.begin() + i);
			return;
		}
	}
}
