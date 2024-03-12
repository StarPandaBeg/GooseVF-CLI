#include <iostream>
#include <locale>
#include <string>
#include <vector>

#include "cli.h"
#include "cxxopts.hpp"

void configure(cxxopts::Options& opts);
bool readConfig(const cxxopts::Options& opts, const cxxopts::ParseResult& result);
void errorResponse(const cxxopts::Options& opts, const std::string& text);

ProgramConfig config;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    cxxopts::Options options("goosevf-cli", "Goose Virtual Filesystem - CLI Tool\n\nThis tool allows to create & extract .honk archives\n");
    configure(options);

    try {
        auto result = options.parse(argc, argv);
        if (!readConfig(options, result))
            return 0;
        runCli(config);
    } catch (const std::exception& e) {
        errorResponse(options, e.what());
        return -1;
    }

    return 0;
}

void configure(cxxopts::Options& opts) {
    opts.add_options()("h,help", "Show help")("c,create", "Create archive")("x,extract", "Extract archive")("t,list", "List files inside archive")("k,keep-old-files", "Don't replace existing files when extracting")("C,directory", "Change to DIR before performing any operations", cxxopts::value<std::string>(), "<path>")("file-version", "File content version", cxxopts::value<int>()->default_value("0"), "<version>")

        ("archive", "Archive name (with extension)", cxxopts::value<std::string>())("files", "Files and/or directories to archive", cxxopts::value<std::vector<std::string>>()->default_value(""));
    opts.allow_unrecognised_options();

    opts.parse_positional({"archive", "files"});
    opts.positional_help("<archive_name> [files or directories to archive]");
}

bool readConfig(const cxxopts::Options& opts, const cxxopts::ParseResult& result) {
    if (result.count("help") || result.unmatched().size() > 0 || result.arguments().size() == 0) {
        std::cout << opts.help();
        return false;
    }

    bool createMode = result.count("create");
    bool extractMode = result.count("extract");
    bool listMode = result.count("list");

    bool onlyOneSelected = ((createMode ^ extractMode ^ listMode) ^ (createMode && extractMode && listMode));
    if (!onlyOneSelected)
        throw std::runtime_error("Unknown mode");
    config.mode = createMode ? CLI_MODE_CREATE : extractMode ? CLI_MODE_EXTRACT
                                                             : CLI_MODE_LIST;

    config.keep_old = result.count("keep-old-files");
    if (result.count("directory")) {
        config.directory = result["directory"].as<std::string>();
    }
    if (result.count("file-version")) {
        config.version = result["file-version"].as<int>();
    }

    if (!result.count("archive"))
        throw std::runtime_error("Archive name is not specified");
    config.archive_name = result["archive"].as<std::string>();

    int filesCount = result.count("files");
    if (filesCount != 0 && config.mode != CLI_MODE_CREATE)
        throw std::runtime_error("File & directory names aren't supported in this mode");
    if (filesCount == 0 && config.mode == CLI_MODE_CREATE)
        throw std::runtime_error("You should specify which files & directories need to archive");

    if (config.mode == CLI_MODE_CREATE) {
        config.files = result["files"].as<std::vector<std::string>>();
    }
    return true;
}

void errorResponse(const cxxopts::Options& opts, const std::string& text) {
    std::cout << text << "\n\n";
    std::cout << opts.help();
}
