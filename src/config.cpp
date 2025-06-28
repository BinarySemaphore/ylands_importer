#include "config.hpp"

#include <iostream>
#include <fstream>

#include "utils.hpp"

char PGM_NAME[128];
const char* PGM_NAME_READABLE = "Ylands Importer";
const char* PGM_VERSION = "0.0.1";
const char* PGM_REF_LINK = "https://github.com/BinarySemaphore/ylands_importer";
const char* PGM_DESCRIPTION = ""
"\n"
"Description:\n"
"  Ylands Importer Converter v2\n"
"  Used with Ylands editor tools:\n"
"    - \"???.ytool\"\n"
"  Accepts 3D models and converts them into a format importable within Ylands.\n"
"  See README for more details.\n"
"  Authors: BinarySemaphore\n"
"  Updated: 2025-06-27\n";
const char* PGM_ARGS_HELP = ""
"<3d-model-file> [options]"
"Args:\n"
"        <3d-model-file> : File path to 3D model.\n"
"                          Supported models file types:\n"
"                            - OBJ : Object Wavefront\n"
"\n";
const char* PGM_OPTIONS_HELP = ""
"\n"
"Options:\n"
"          -v, --version : Show info.\n"
"             -h, --help : Show info and options help.\n"
"\n";

void printHelp() {
	printHelp(false);
}

void printHelp(bool version_only) {
	std::cout << "Program: " << PGM_NAME << std::endl << std::endl;
	std::cout << "Version: " << PGM_VERSION << std::endl;
	std::cout << PGM_DESCRIPTION;
	if (!version_only) {
		std::cout << PGM_NAME << PGM_ARGS_HELP;
		std::cout << PGM_OPTIONS_HELP;
	}
}

Config getConfigFromArgs(int argc, char** argv) {
	bool get_model_filename = true;
	bool missing_arg = false;
	char missing_arg_name[25];
	char missing_arg_expects[100];
	Config config;

	// Defaults (args)
	config.output_filename = "output.yim";

	for (int i = 0; i < argc; i++) {
		if (i == 0) {
			std::strcpy(PGM_NAME, argv[0]);
			continue;
		}

		// Get secondary args
		if (get_model_filename) {
			config.model_filename = argv[i];
			get_model_filename = false;
		}

		// Catch primary args
		else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
			printHelp();
			exit(0);
		} else if (std::strcmp(argv[i], "--version") == 0 || std::strcmp(argv[i], "-v") == 0) {
			printHelp(true);
			exit(0);
		} else {
			std::cerr << "Unrecongnized argument \"" << argv[i] << "\". "
					  << "Use -h or --help to show options." << std::endl;
			exit(1);
		}
	}

	// Check if secondary args missing
	missing_arg = get_model_filename;
	if (get_model_filename) {
		std::strcpy(missing_arg_name, "3D Model File");
		std::strcpy(missing_arg_expects, "a file path");
	}
	if (missing_arg) {
		std::cerr << "Arguement missing: \"" << missing_arg_name
				  << "\" expects " << missing_arg_expects << std::endl;
		exit(1);
	}

	return config;
}