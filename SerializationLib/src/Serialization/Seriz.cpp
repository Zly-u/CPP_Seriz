#include "Serialization/Seriz.hpp"

#include <filesystem>

namespace zly {
	Seriz::Seriz() {

	}

	Seriz::~Seriz()
	{
		if (loaded_file.is_open()) {
			loaded_file.close();
		}
	}


	void Seriz::Serialize(std::filesystem::path&& filePath) {
		std::ofstream OutFile(filePath, std::ios::binary);

		buffer.shrink_to_fit();

		OutFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
		OutFile.close();

		buffer.clear();
	}


	void Seriz::LoadFile(std::filesystem::path&& filePath) {
		loaded_file = std::ifstream(filePath, std::ios::binary);

		if (!loaded_file) {
			throw std::runtime_error("Failed to load file");
		}
	}
}