#ifndef CLIENTDATA_H
#define CLIENTDATA_H


//Details about the file the user wants to download
struct FileCursor {
	std::string filename = "";
	uint32_t fileChunkNumber = 0;
	uint32_t totalNumberChunks = 0;
};

class ClientData {
private:
	//Member variables
	bool supportsEncyption;
	std::string accountUsername;

	//Current directory the user is in
	std::string currentDirectory;

	//The files he wants to download
	std::vector<FileCursor> filesToDownload;
	std::vector<std::string> filesToUpload;

	//Diffie-Hellman variables
	std::string secret;
	std::string publicKey;
	std::string privateKey;
public:
	ClientData(const std::string publicKey, const std::string privateKey);
	~ClientData();

	//Accessors
	const std::string GetPrivateKey() const;
	const std::string GetSecret() const;
	const std::string GetAccountUsername() const;
	const std::string GetActiveDirectory() const;
	const FileCursor GetFileCursor(std::string filename) const;
	const bool IsFileInUploadList(const std::string& filename);
	bool SupportsEncryption();

	//Modifiers
	void SetSupportsEncryption(const bool supportsEncryption);
	void SetSecret(const std::string& secret);
	void SetAccountUsername(const std::string& username);
	void ChangeCurrentDirectory(const std::string& currentDirectory);
	void AddDownloadFile(std::string filename);
	void EraseDownloadFile(std::string filename);
	void IncrementChunkNumber(std::string filename);
	void AddUploadFile(const std::string& filename);
	void EraseUploadFile(const std::string& filename);

};


#endif
