#include <filesystem>
#include <iostream>
#include <print>
#include "Serialization/Seriz.hpp"



int main() {
	try {
		Seriz SerizWriter;

		SerizWriter.Write<float>(10.f);
		SerizWriter.Write<uint32_t>(69);
		SerizWriter.Write<std::string>("sex");
		SerizWriter.Write<std::vector<uint8_t>>({1, 2, 3, 4, 5, 6});
		SerizWriter.Write<std::vector<uint16_t>>({11, 22, 33, 44, 55, 66});
		SerizWriter.Write<std::vector<uint32_t>>({111, 222, 333, 444, 555, 666});
		SerizWriter.Write<std::vector<uint64_t>>({1111, 2222, 3333, 4444, 5555, 6666});

		SerizWriter.Serialize("aaaa.bin");

		std::println("Serialization complete!");
	}
	catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}

	try {
		Seriz SerizReader;
		SerizReader.LoadFile("aaaa.bin");

		float a;
		uint32_t b;
		std::string c;
		std::vector<uint8_t> d1;
		std::vector<uint16_t> d2;
		std::vector<uint32_t> d3;
		std::vector<uint64_t> d4;
		SerizReader.Read<float>(a);
		SerizReader.Read<uint32_t>(b);
		SerizReader.Read<std::string>(c);
		SerizReader.Read<std::vector<uint8_t>>(d1);
		SerizReader.Read<std::vector<uint16_t>>(d2);
		SerizReader.Read<std::vector<uint32_t>>(d3);
		SerizReader.Read<std::vector<uint64_t>>(d4);

		std::println("Deserialization complete!");
	}
	catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}



	return 0;
}