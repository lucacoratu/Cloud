#ifndef FILESYSTEMAPI_H
#define FILESYSTEMAPI_H

struct FileDetails {
	std::string name;
	std::string type;
};

class FilesystemAPI {
private:

public:
	static void InitEntryDirectory();
	static bool CreateUserDirectory(std::string username);
	static bool ExistsDirectory(const std::string& path);
	static bool CreateNewDirectory(const std::string& path);
	static bool CreateNewFile(const std::string& path);
	static std::vector<FileDetails> ViewFilesInDirectory(const std::string& path);
};


#endif
