// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>

#include "irbis.h"
#include "irbis_internal.h"


//====================================================================

// изображаем std::index_sequence из подручных средств

template <std::size_t ...>
struct index_sequence
{
};

template <std::size_t N, std::size_t ... Next>
struct index_sequence_helper
    : public index_sequence_helper <N - 1u, N - 1u, Next...>
{
};

template <std::size_t ... Next>
struct index_sequence_helper <0u, Next...>
{
    using type = index_sequence <Next...>;
};

template <std::size_t N>
using make_index_sequence = typename index_sequence_helper <N>::type;

//====================================================================

//====================================================================

//static constexpr unsigned char secretSequence[]
//{ 0xAE, 0x92, 0xC1, 0x58, 0xD5, 0x82, 0x4D, 0x30, 0x56, 0x88 };
//
//constexpr unsigned char secretByte (size_t pos, size_t seed)
//{
//    // генератор случайной последовательности
//    return secretSequence [(pos + seed) % sizeof (secretSequence)];
//}
//
//constexpr char encodeChar (char c, size_t pos, size_t seed)
//{
//    return (char) (((unsigned char) c) ^ secretByte (pos, seed));
//}
//
//constexpr char decodeChar (char c, size_t pos, size_t seed)
//{
//    return (char) (((unsigned char)c) ^ secretByte (pos, seed));
//}

//====================================================================

constexpr char encodeChar (char c)
{
    return c ^ 0x55;
}

template <std::size_t _size>
struct SecretString
{
    using dataType = const char (&) [_size + 1];
    char m_data [_size + 1];

//    constexpr SecretString (char data[_size+1])
//        : m_data { data }
//    {
//    }

    constexpr bool empty() const noexcept { return _size == 0; }
    constexpr std::size_t size() const noexcept { return _size; }

    template <char ... chrs>
    constexpr static SecretString make()
    {
        return SecretString();
    }

    std::string decoded() const
    {
        std::string result;
        result.reserve (this->size() + 1);
        for (std::size_t i = 0; i < _size; ++i) {
            result.push_back (m_data[i] ^ 0x55);
        }

        return result;
    }
};

void testSecret()
{
    SecretString<5> url = SecretString<5>::make<'H', 'e', 'l', 'l', 'o'>();
    std::cout << url.m_data << std::endl;
    std::cout << url.decoded() << std::endl;
}