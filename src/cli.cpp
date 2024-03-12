#include "cli.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

#include "FileReader.h"
#include "FileWriter.h"
#include "Utility.h"

#ifdef _WIN32
#include <direct.h>
// MSDN recommends against using getcwd & chdir names
#define cwd _getcwd
#define cd _chdir
#else
#include "unistd.h"
#define cwd getcwd
#define cd chdir
#endif

using namespace GooseVF;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

void modeCreate(const ProgramConfig& config);
void modeExtract(const ProgramConfig& config);
void modeList(const ProgramConfig& config);
std::string trim(const std::string& str, const std::string& whitespace = " \t");
std::string convertPath(const std::string& str);

void runCli(const ProgramConfig& config) {
    if (config.mode == CLI_MODE_EXTRACT) {
        std::filesystem::create_directories(config.directory);
    }

    std::filesystem::current_path(config.directory);
    cd(config.directory.c_str());

    if (config.mode == CLI_MODE_CREATE)
        return modeCreate(config);
    if (config.mode == CLI_MODE_EXTRACT)
        return modeExtract(config);
    if (config.mode == CLI_MODE_LIST)
        return modeList(config);
}

void modeCreate(const ProgramConfig& config) {
    FileWriter writer;
    std::cout << "Creating archive " << config.archive_name << "\n";
    writer.setFileVersion(config.version);

    for (auto& f : config.files) {
        if (std::filesystem::is_directory(f)) {
            for (const auto& dirEntry : recursive_directory_iterator(f)) {
                if (dirEntry.is_directory())
                    continue;
                std::string path = convertPath(dirEntry.path().string());

                writer.addFile(dirEntry.path().string(), path);
                std::cout << "\n+ " << dirEntry.path().string() << " -> " << path;
            }
            continue;
        }

        auto path = convertPath(f);
        writer.addFile(f, path);
        std::cout << "\n+ " << f << " -> " << path;
    }

    writer.save(config.archive_name);
    std::cout << "\n\nSuccess.";
}

void modeExtract(const ProgramConfig& config) {
    FileReader reader(config.archive_name);
    auto buffer = std::vector<char>();

    std::cout << "Extracting archive " << config.archive_name << "\n";
    std::cout << "Content version: " << reader.contentVersion() << "\n";
    reader.iterateFiles(
        [&config, &reader, &buffer](const std::string& path) {
            std::vector<std::string> directories = GooseVF::splitPath(path);
            directories.pop_back();
            if (directories.size() > 0) {
                std::filesystem::create_directories(GooseVF::buildPath(directories));
            }

            if (std::filesystem::exists(path) && config.keep_old) {
                std::cout << "\n- " << path << " (skipped)";
                return;
            }
            reader.readFile(path, buffer);
            std::cout << "\n* " << path;

            std::ofstream f(path, std::ios::binary);
            f.write(buffer.data(), buffer.size());
            f.close();
        });
    std::cout << "\n\nSuccess.";
}

void modeList(const ProgramConfig& config) {
    FileReader reader(config.archive_name);

    std::cout << "Reading archive " << config.archive_name << "\n";
    std::cout << "Content version: " << reader.contentVersion() << "\n";
    reader.iterateFiles(
        [&reader](const std::string& path) {
            std::cout << "\n* " << path;
        });
    std::cout << "\n\nSuccess.";
}

std::string trim(const std::string& str,
                 const std::string& whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string convertPath(const std::string& str) {
    std::string path(str);
    std::replace(path.begin(), path.end(), '/', '\\');
    return trim(path, "\\");
}
