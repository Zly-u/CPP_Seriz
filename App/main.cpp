#include <array>
#include <filesystem>
#include <iostream>
#include <list>
#include <map>
#include <print>
#include <unordered_map>

#include "Serialization/Seriz.hpp"



struct TestStruct1 {
	uint8_t a;
	uint16_t b;
	uint32_t c;
	uint64_t d;

	void encode(zly::Seriz* serializer) const {
		std::print("TestStruct::encode()\n");
		serializer->Write<decltype(a)>(a);
		serializer->Write<decltype(b)>(b);
		serializer->Write<decltype(c)>(c);
		serializer->Write<decltype(d)>(d);
	}

	void decode(zly::Seriz* deserializer) {
		std::print("TestStruct::decode()\n");
		deserializer->Read<decltype(a)>(a);
		deserializer->Read<decltype(b)>(b);
		deserializer->Read<decltype(c)>(c);
		deserializer->Read<decltype(d)>(d);
	}
};


struct TestStruct2 {
	uint8_t a;
	uint16_t b;
	uint32_t c;
	uint64_t d;
};


template<>
struct zly::Convert<TestStruct2> {
	static void encode(std::vector<std::byte>& buffer, const TestStruct2& InData)
	{
		std::print("Convert<TestStruct2>::encode() SPECIALIZATION \n");

		// This encodes the whole thing at once, if that's desired.
		const auto* ptr = reinterpret_cast<const std::byte*>(&InData);
		buffer.insert(buffer.end(), ptr, ptr + sizeof(InData));
	}


	static void decode(std::ifstream& file, TestStruct2& OutData)
	{
		std::print("Convert<TestStruct2>::encode() SPECIALIZATION \n");

		// This decodes the whole thing at once, if that's desired.
		file.read(reinterpret_cast<char*>(&OutData), sizeof(OutData));
	}
};




int main() {
	try {
		zly::Seriz SerizWriter;

		SerizWriter.Write<float>(10.f);
		SerizWriter.Write<uint32_t>(69);
		SerizWriter.Write<std::string>("say gex");

		std::string_view d = "test";
		SerizWriter.Write(d);

		SerizWriter.Write<std::vector<uint8_t>>({1, 2, 3, 4, 5, 6});
		SerizWriter.Write<std::vector<uint16_t>>({11, 22, 33, 44, 55, 66});
		SerizWriter.Write<std::vector<uint32_t>>({111, 222, 333, 444, 555, 666});
		SerizWriter.Write<std::vector<uint64_t>>({1111, 2222, 3333, 4444, 5555, 6666});
		SerizWriter.Write<std::vector<std::string>>({"sv1", "sv2", "sv3", "sv4", "sv5", "sv6"});

		SerizWriter.Write<std::list<uint32_t>>({1, 2, 3, 4});
		SerizWriter.Write<std::list<std::string>>({"l1", "l2", "l3", "l4"});

		TestStruct1 ts1{81, 82, 83, 84};
		TestStruct2 ts2{91, 92, 93, 94};
		SerizWriter.Write(ts1);
		SerizWriter.Write(ts2);

		std::unordered_map<std::string, uint8_t> unmap{
			{"um-test-1", 1},
			{"um-test-2", 2},
			{"um-test-3", 3},
			{"um-test-4", 4},
		};
		SerizWriter.Write(unmap);

		std::map<std::string, uint16_t> map{
			{"m-test-1", 1},
			{"m-test-2", 2},
			{"m-test-3", 3},
			{"m-test-4", 4},
		};
		SerizWriter.Write(map);

		SerizWriter.Serialize("aaaa.bin");

		std::println("Serialization complete!");
	}
	catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	try {
		zly::Seriz SerizReader;
		SerizReader.LoadFile("aaaa.bin");

		float a;
		uint32_t b;
		std::string c;
		std::string d;

		std::array<uint8_t, 6> d1;
		std::vector<uint16_t> d2;
		std::vector<uint32_t> d3;
		std::vector<uint64_t> d4;
		std::vector<std::string> d5;

		std::list<uint32_t> l1;
		std::list<std::string> l2;

		TestStruct1 ts1{81, 82, 83, 84};
		TestStruct2 ts2{91, 92, 93, 94};

		std::unordered_map<std::string, uint8_t> unmap;
		std::map<std::string, uint16_t> map;

		SerizReader.Read<float>(a);
		SerizReader.Read<uint32_t>(b);
		SerizReader.Read<std::string>(c);
		SerizReader.Read<std::string>(d);

		SerizReader.Read<std::array<uint8_t, 6>>(d1);
		SerizReader.Read<std::vector<uint16_t>>(d2);
		SerizReader.Read<std::vector<uint32_t>>(d3);
		SerizReader.Read<std::vector<uint64_t>>(d4);
		SerizReader.Read<std::vector<std::string>>(d5);

		SerizReader.Read<std::list<uint32_t>>(l1);
		SerizReader.Read<std::list<std::string>>(l2);

		SerizReader.Read(ts1);
		SerizReader.Read(ts2);

		SerizReader.Read(unmap);
		SerizReader.Read(map);

		std::println("Deserialization complete!");
	}
	catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}



	return 0;
}