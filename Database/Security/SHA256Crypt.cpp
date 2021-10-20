#include "SHA256Crypt.h"

SHA256Crypt::SHA256Crypt()
		: m_context(EVP_MD_CTX_new())
		, m_hash(std::make_unique<unsigned char[]>(EVP_MAX_MD_SIZE))
		, m_hash_length(0)
{
	if (m_context == nullptr)
	{
		LOG_FATAL("Error allocating digest context");
		throw std::runtime_error("Error allocating digest context");
	}
}

SHA256Crypt::~SHA256Crypt()
{
	m_hash.reset();
	EVP_MD_CTX_free(m_context);
}

const std::string SHA256Crypt::GenerateHash(const std::string& string)
{
	Init();
	Update(string);
	Final();

	return GetHashString();
}

const std::string SHA256Crypt::GenerateSaltedHash(const std::string& string, const std::string& salt)
{
	Init();
	Update(salt);
	Update(string);
	Final();

	return GetHashString();
}

void SHA256Crypt::Init()
{
	if (!EVP_DigestInit_ex(m_context, EVP_sha256(), nullptr))
	{
		LOG_FATAL("SHA256 init failed");
		throw std::runtime_error("SHA256 init failed");
	}
}

void SHA256Crypt::Update(const std::string& string)
{
	if (!EVP_DigestUpdate(m_context, string.c_str(), string.length()))
	{
		LOG_FATAL("SHA256 update failed");
		throw std::runtime_error("SHA256 update failed");
	}
}

void SHA256Crypt::Final()
{
	if (!EVP_DigestFinal_ex(m_context, m_hash.get(), &m_hash_length))
	{
		LOG_FATAL("SHA256 final failed");
		throw std::runtime_error("SHA256 final failed");
	}
}

std::string SHA256Crypt::GetHashString() const
{
	std::stringstream ss;

	LOG_DEBUG("Receiving hash string");
	for (unsigned int i = 0; i < m_hash_length; i++)
	{
		ss << std::setfill('0') << std::setw(HEX_WIDTH) << std::hex << (unsigned int) m_hash[i];
	}

	return ss.str();
}