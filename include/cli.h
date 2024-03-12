#pragma once

#include <vector>
#include <string>

constexpr auto CLI_MODE_CREATE = 0;
constexpr auto CLI_MODE_EXTRACT = 1;
constexpr auto CLI_MODE_LIST = 2;

struct ProgramConfig {
	int mode = CLI_MODE_CREATE;
	int version = 0;
	bool keep_old = false;
	std::string directory = "./";

	std::string archive_name;
	std::vector<std::string> files;
};

void runCli(const ProgramConfig& config);