#include "config.hpp"
#include "objwavefront.hpp"
#include "yim.hpp"

int main(int argc, char** argv) {
	Config config = getConfigFromArgs(argc, argv);
	ObjWavefront model;

	model.load(config.model_filename.c_str(), false);

	YIMObject yim(model);
	model.clear();

	yim.write("output.yim");

	return 0;
}