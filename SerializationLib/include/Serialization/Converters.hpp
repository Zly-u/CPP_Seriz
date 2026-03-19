#pragma once

#include <fstream>
#include <vector>
#include <print>


class Seriz;

template<typename T>
concept CSerializable = requires(T* obj, Seriz* serializer, std::vector<std::byte>& buffer, std::ifstream& file)
{
	{ obj->encode(serializer, buffer) } -> std::same_as<void>;
	{ obj->decode(serializer, file)   } -> std::same_as<void>;
};


// TODO: Split each type into their own specializations
template <typename T>
struct Convert
{
	using PrefixListSizeType = uint32_t;

	static void decode(std::ifstream& file, T& OutData)
	{
		if constexpr (std::is_arithmetic_v<T>)
		{
			file.read(reinterpret_cast<char*>(&OutData), sizeof(T));
		}
		else if constexpr (std::ranges::range<T>) // For everything iteratable
		{
			using ElementType = std::ranges::range_value_t<T>;

			// Encode the size of the list.
			PrefixListSizeType FetchedSize;
			file.read(reinterpret_cast<char*>(&FetchedSize), sizeof(PrefixListSizeType));

			if constexpr (std::ranges::contiguous_range<T>) // For contiguous blocks of memory
			{
				if constexpr(std::is_arithmetic_v<ElementType>)
				{
					const size_t ElementSize = sizeof(ElementType);
					OutData.resize(FetchedSize);
					file.read(reinterpret_cast<char*>(OutData.data()), FetchedSize * ElementSize);
				}
				else
				{
					for (uint32_t i = 0; i < FetchedSize; ++i)
					{
						Convert<ElementType>::decode(file, OutData);
					}
				}
			}
			else if constexpr (std::ranges::range<T>) // for things that are spread around in the heap (e.g. std::list, std::unordered_map and etc)
			{
				// TODO: For other types, and test it.
				for (uint32_t i = 0; i < FetchedSize; ++i)
				{
					ElementType OutElementData;
					Convert<ElementType>::decode(file, OutElementData);
					OutData.push_back(OutElementData);
				}
			}
		}
		else if constexpr (CSerializable<T>)
		{
			OutData.decode(file, OutData);
		}
		else
		{
			static_assert(false, "No decoding is implemented for this type");
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	static void encode(std::vector<std::byte>& buffer, const T& InData)
	{
		if constexpr (std::is_arithmetic_v<T>)
		{
			const auto* ptr = reinterpret_cast<const std::byte*>(&InData);
			buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
		}
		else if constexpr (std::ranges::range<T>) // For everything iteratable
		{
			using ElementType = std::ranges::range_value_t<T>;

			// Encode the size of the list.
			const PrefixListSizeType ContainerSize = std::ranges::size(InData);
			const auto* SizePtr = reinterpret_cast<const std::byte*>(&ContainerSize);
			buffer.insert(buffer.end(), SizePtr, SizePtr + sizeof(PrefixListSizeType));

			if constexpr (std::ranges::contiguous_range<T>) // For contiguous blocks of memory
			{
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
			else if constexpr (std::ranges::range<T>) // for things that are spread around in the heap (e.g. std::list, std::unordered_map and etc)
			{
				for (const ElementType& element : InData)
				{
					Convert<ElementType>::encode(buffer, element);
				}
			}
		}
		else if constexpr (CSerializable<T>) // For custom types that implement serializing methods.
		{
			InData.encode(buffer);
		}
		else
		{
			static_assert(false, "No serialization methods are implemented for this type!");
		}
	}
};