#pragma once

#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

#define GENERATED_TOKEN_LEN 8
#define TOKEN_CHARS_ALLOWED "1234567890bcdfghjklnmnpqrstvwxyz"

// selected prime number used to generate a random number
#define TOKEN_INCREMENT 21'643'785'587

class TokenGenerator
{
public:
	TokenGenerator(const std::uint32_t token_length = GENERATED_TOKEN_LEN);
	void GetNextToken(char*& output_token_ptr);
	void ResetGenerator();

private:
	void GetTokenFromNumber(const std::uint64_t token_number, char*& output_token_ptr, const std::uint32_t token_length) const;
	std::uint64_t GenerateNextNumber();
	std::uint64_t BinaryPow(std::uint64_t base, std::uint64_t exp) const;
	std::uint64_t GenerateSeed() const;

	const char* const m_alphabet;
	std::uint32_t m_alphabet_length;
	std::uint32_t m_token_length;
	std::uint64_t m_multiplier;
	std::uint64_t m_increment;
	std::uint64_t m_modulus;
	std::uint64_t m_init_generated_number;
	std::uint64_t m_last_generated_number;
};
