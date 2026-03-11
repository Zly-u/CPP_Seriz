#pragma once

#include <fstream>
#include <vector>

#include "Converters.hpp"
#include "SerializationExports.hpp"


class SERIZ_API Seriz {
	public:
		Seriz();
		~Seriz();

		void Serialize(std::filesystem::path&& filePath);
		void LoadFile(std::filesystem::path&& filePath);

		template <typename T>
		void Write(T& InVal) {
			Convert<T>::encode(buffer, InVal);
		}

		template <typename T>
		void Write(T&& InVal) {
			Convert<T>::encode(buffer, InVal);
		}


		template<typename T>
		void Read(T& OutVal) {
			Convert<T>::decode(loaded_file, OutVal);
		}



	private:
		std::vector<std::byte> buffer;
		std::ifstream loaded_file;
};