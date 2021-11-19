#include "cloudpch.h"
#include "FilesystemAPI.h"

#include <filesystem>
#include <fstream>

#include "Core/Log.h"

void FilesystemAPI::InitEntryDirectory()
{
	/*
	* Creates the entry directory if it is inexistent
	* If the entry directory doesn't exist and it can't be created then the server will terminate
	*/
	const std::filesystem::path path = "./entry";
	if (!std::filesystem::exists(path)) {
		bool res = std::filesystem::create_directory(path);
		if (res == false) {
			SV_ERROR("Could not create the entry directory! Server will exit now!");
			exit(-1);
		}
		else {
			SV_INFO("Entry directory created!");
		}
	}
	else {
		SV_INFO("Entry directory found and ready to be used!");
	}
}

bool FilesystemAPI::CreateUserDirectory(std::string username)
{
	/*
	* Creates the directory inside the entry directory for each user
	*/
	std::filesystem::path path = "./entry/" + username;
	
	bool res = std::filesystem::create_directory(path);

	return res;
}

bool FilesystemAPI::ExistsDirectory(const std::string& path)
{
	/*
	* Verifies if a directory exists or not
	*/
	std::filesystem::path path_filesystem = path;

	return (std::filesystem::exists(path_filesystem));
}

bool FilesystemAPI::CreateNewDirectory(const std::string& path)
{
	/*
	* Creates a new directory in the path specified
	*/
	std::filesystem::path path_filesystem = path;

	return std::filesystem::create_directory(path_filesystem);
}

bool FilesystemAPI::CreateNewFile(const std::string& path)
{
	std::ifstream in(path);
	if (in.is_open()) {
		in.close();
		return false;
	}

	std::ofstream out(path);
	return true;
}

std::vector<FileDetails> FilesystemAPI::ViewFilesInDirectory(const std::string& path)
{
	std::vector<FileDetails> filenames;
	std::filesystem::path path_filesystem = path;

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory())
			filenames.push_back({ entry.path().filename().string(), "directory" });
		else
			filenames.push_back({ entry.path().filename().string(), "file" });
	}

	return filenames;
}
