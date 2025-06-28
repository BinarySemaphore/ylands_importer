#ifndef CONFIG_H
#define CONFIG_H

#include <string>

extern char PGM_NAME[128];
extern const char* PGM_NAME_READABLE;
extern const char* PGM_VERSION;
extern const char* PGM_REF_LINK;
extern const char* CONFIG_FILE;

class Config {
public:
	std::string model_filename;
	std::string output_filename;
};

void printHelp();
void printHelp(bool version_only);

Config getConfigFromArgs(int argc, char** argv);

void updateConfigFromFile(Config& config, const char* filename);
void validateConfigAndPromptForFixes(Config& config, const char* filename, bool config_changed);

#endif // CONFIG_H