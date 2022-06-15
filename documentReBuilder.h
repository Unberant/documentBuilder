#pragma once
#include "components.h"

class document
{
public:
	document();
	document(const document&);
	document(document&&) noexcept;

	explicit document(std::istream& stream);
	explicit document(const std::string& text);

	document& operator=(const document&);
	document& operator=(document&&) noexcept;

	bool empty() const;
	size_t count() const;
	void remove(size_t idx);
	void update(size_t idx, std::istream& stream);
	void update(size_t idx, const std::string& text);
	void clear();
	void load(std::istream& stream);
	void load(const std::string& text);

	void print()
	{
		std::cout << storage;
	}

private:
    std::string storage;
};

inline document::document()
{
	std::cout << "none";
}

inline document::document(const document&)
{
	
}

inline document::document(document&&) noexcept
{
	
}

inline document::document(std::istream& _file)
{
	if (_file) 
	{
		std::ostringstream osStream;
		osStream << _file.rdbuf(); // reading data
		storage = osStream.str();
	}
}

inline document::document(const std::string& text)
{
	storage = text;
}

inline document& document::operator=(const document&)
{
	
}

inline document& document::operator=(document&&) noexcept
{
	
}

inline bool document::empty() const
{
	
}

inline size_t document::count() const
{
	
}

inline void document::remove(size_t idx)
{
	
}

inline void document::update(size_t idx, const std::string& text)
{
	
}

inline void document::update(size_t idx, std::istream& stream)
{
	
}

inline void document::clear()
{
	
}

inline void document::load(std::istream& stream)
{
	
}

inline void document::load(const std::string& text)
{
	
}













