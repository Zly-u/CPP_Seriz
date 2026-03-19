#pragma once

#include <fstream>
#include <vector>
#include <cassert>

class Seriz;

template<typename T>
concept CSerializable = requires(T* obj, Seriz* serializer)
{
	{ obj->encode(serializer) } -> std::same_as<void>;
	{ obj->decode(serializer) } -> std::same_as<void>;
};

template<typename T>
concept CResizable = requires(T& container, typename T::size_type size)
{
	{ container.resize(size) } -> std::same_as<void>;
};


template <typename T>
struct Convert {
	static void decode(std::ifstream&, T&) {
		static_assert(false, "No decoding is implemented for this type");
	}

	static void encode(std::vector<std::byte>&, const T&) {
		static_assert(false, "No serialization methods are implemented for this type!");
	}
};


template <typename T>
requires std::is_arithmetic_v<T>
struct Convert<T> {
	static void decode(std::ifstream& file, T& OutData) {
		file.read(reinterpret_cast<char*>(&OutData), sizeof(T));
	}

	static void encode(std::vector<std::byte>& buffer, const T& InData) {
		const auto* ptr = reinterpret_cast<const std::byte*>(&InData);
		buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
	}
};


// This also assists a serialization for std::pair based containers.
template <typename K, typename V>
struct Convert<std::pair<K, V>> {
	using Type = std::pair<K, V>;
	using KeyType = std::remove_const_t<K>;

	static void decode(std::ifstream& file, Type& OutData)
	{
		Convert<KeyType>::decode(file, const_cast<KeyType&>(OutData.first));
		Convert<V>::decode(file, OutData.second);
	}

	static void encode(std::vector<std::byte>& buffer, const Type& InData)
	{
		Convert<K>::encode(buffer, InData.first);
		Convert<V>::encode(buffer, InData.second);
	}
};


// For std::array, std::vector, std::string, std::view_string and such.
template <typename T>
requires std::ranges::range<T> && std::ranges::contiguous_range<T>
struct Convert<T>
{
	using PrefixListSizeType = uint32_t;

	static void decode(std::ifstream& file, T& OutData)
	{
		using ElementType = std::ranges::range_value_t<T>;

		// Encode the size of the list.
		PrefixListSizeType FetchedSize;
		file.read(reinterpret_cast<char*>(&FetchedSize), sizeof(PrefixListSizeType));


		if constexpr (CResizable<T>)
		{
			OutData.resize(FetchedSize);
		}
		else
		{
			// Not sure if this is a good idea, but it was the only way
			// I could figure a size of the container via its type.
			// Tested with std::array, and from it i got this ::iterator::_EEN_SIZE solution.
			static_assert(T::iterator::_EEN_SIZE == OutData.size(), "Not the same static container size!");
		}


		if constexpr(std::is_arithmetic_v<ElementType>)
		{
			const size_t ElementSize = sizeof(ElementType);
			file.read(reinterpret_cast<char*>(OutData.data()), FetchedSize * ElementSize);
		}
		else
		{
			for (ElementType& element : OutData)
			{
				Convert<ElementType>::decode(file, element);
			}
		}
	}

	static void encode(std::vector<std::byte>& buffer, const T& InData)
	{
		using ElementType = std::ranges::range_value_t<T>;

		// Encode the size of the list.
		const PrefixListSizeType ContainerSize = std::ranges::size(InData);
		const auto* SizePtr = reinterpret_cast<const std::byte*>(&ContainerSize);
		buffer.insert(buffer.end(), SizePtr, SizePtr + sizeof(PrefixListSizeType));

		if constexpr(std::is_arithmetic_v<ElementType>)
		{
			const auto* DataPtr = reinterpret_cast<const std::byte*>(std::ranges::data(InData));
			buffer.insert(buffer.end(), DataPtr, DataPtr + (ContainerSize * sizeof(ElementType)));
		}
		else
		{
			for (const ElementType& element : InData)
			{
				Convert<ElementType>::encode(buffer, element);
			}
		}
	}
};


// For other iterable types, std::list, std::unordered_map and etc.
template <typename T>
requires std::ranges::range<T> && (!std::ranges::contiguous_range<T>)
struct Convert<T>
{
	using PrefixListSizeType = uint32_t;

	static void decode(std::ifstream& file, T& OutData)
	{
		using ElementType = std::ranges::range_value_t<T>;

		// Encode the size of the list.
		PrefixListSizeType FetchedSize;
		file.read(reinterpret_cast<char*>(&FetchedSize), sizeof(PrefixListSizeType));

		auto It = std::inserter(OutData, OutData.end());

		using NonConstElementType = std::remove_const_t<ElementType>;

		for (uint32_t i = 0; i < FetchedSize; ++i)
		{
			NonConstElementType OutElementData;
			Convert<NonConstElementType>::decode(file, OutElementData);

			*It = std::move(OutElementData);
		}
	}

	static void encode(std::vector<std::byte>& buffer, const T& InData)
	{
		using ElementType = std::ranges::range_value_t<T>;

		// Encode the size of the list.
		const PrefixListSizeType ContainerSize = std::ranges::size(InData);
		const auto* SizePtr = reinterpret_cast<const std::byte*>(&ContainerSize);
		buffer.insert(buffer.end(), SizePtr, SizePtr + sizeof(PrefixListSizeType));

		for (const ElementType& element : InData)
		{
			Convert<ElementType>::encode(buffer, element);
		}
	}
};