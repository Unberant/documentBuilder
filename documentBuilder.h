#pragma once
#include "components.h"

enum docAttributes
{
	HASH = '#',
	DOLLAR = '$',
	BRACE_1 = '{',
	BRACE_0 = '}',
};

class document
{
	typedef baseBlock* ptrBase;
public:
	document() = default;
	document(const document&);

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

	void render() const;

private:
    std::string storage;

	std::vector<ptrBase> elements;

	std::vector<ptrBase> setUpElements(const std::string& text, unsigned st_x, unsigned st_y);
};