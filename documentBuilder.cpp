#include "documentBuilder.h"

document::document(const document& oldFile)
{
	storage = oldFile.storage;
	for (auto element : oldFile.elements)
		elements.push_back(element->clone());
}
document::document(std::istream& _file)
{
	storage = functions::file_to_string(_file);
	elements = setUpElements(storage, variables::console_x, variables::console_y);
}
document::document(const std::string& _text)
{
	storage = _text;
	elements = setUpElements(_text, variables::console_x, variables::console_y);

}
document& document::operator=(const document& _old)
{
	storage = _old.storage;
	for (auto element : _old.elements)
		elements.push_back(element->clone());
	return *this;
}
document& document::operator=(document&& _old) noexcept
{
	storage = std::move(_old.storage);
	elements = std::move(_old.elements);
	return *this;
}
bool document::empty() const
{
	if (elements.size() == 0)
		return true;
	else
		return false;
}
size_t document::count() const
{
	return elements.size();
}
void document::remove(size_t idx)
{
	if (idx > count() - 1)
		return;
	if (idx == 0)
		clear();
	else
	{
		elements.at(idx)->deleteConnection();
		std::vector<size_t> indices;
		for (size_t i = 1; i < elements.size(); ++i)
			if (elements.at(i)->getParent() == nullptr)
				indices.push_back(i);

		size_t last = indices.at(indices.size() - 1) + 1;
		elements.erase(elements.begin() + indices.at(0), elements.begin() + last);
		elements.at(0)->configureDimensions(coordinates(0, 0));
		elements.at(0)->configureCoordinatesByAlign(0);
		elements.at(0)->configureTextByMask();
	}
}
void document::update(size_t idx, const std::string& text)
{
	coordinates input_start = elements.at(idx)->get_coordinates();
	std::vector<ptrBase> input = setUpElements(text, input_start.x, input_start.y);

	size_t parent_index = 0;
	for (size_t i = 0; i < elements.size(); ++i)
		if (elements.at(i) == elements.at(idx)->getParent())
		{
			parent_index = i;
			break;
		}
	elements.at(parent_index)->addElementAfter(elements.at(idx), input.at(0));

	remove(idx);
	elements.insert(elements.begin() + idx, input.begin(), input.end());

	elements.at(0)->configureDimensions(coordinates(0, 0));
	elements.at(0)->configureCoordinatesByAlign(0);
	elements.at(0)->configureTextByMask();
}
void document::update(size_t idx, std::istream& _file)
{
	update(idx, functions::file_to_string(_file));
}
void document::clear()
{
	elements.at(0)->deleteConnection();
	elements.clear();
}
void document::load(std::istream& _file)
{
	load(functions::file_to_string(_file));
}
void document::load(const std::string& _text)
{
	storage = _text;
	elements.clear();
	elements = setUpElements(_text, variables::console_x, variables::console_y);
}
void document::render() const
{
	for (auto var : elements)
	{
		var->render();
	}
	variables::console.GotoXY(variables::console_x, variables::console_y);
}
// private
typedef baseBlock* ptrBase;
std::vector<ptrBase> document::setUpElements(const std::string& text, unsigned st_x, unsigned st_y)
{
	std::vector<ptrBase> new_elements;
	size_t doc_start = text.find(BRACE_1);
	size_t doc_end = text.find_last_of(BRACE_0);

	if (doc_start == std::string::npos || doc_end == std::string::npos)
		return std::vector<ptrBase>{};

	size_t root = new_elements.size();
	new_elements.push_back(new blocksBorders(st_x, st_y));
	size_t elementIndex = root;

	std::vector<size_t> blocksIndices = { root };

	size_t currentPosition = doc_start;
	while (currentPosition < doc_end)
	{
		currentPosition += 1;
		currentPosition = functions::minimum(text.find(BRACE_1, currentPosition), text.find(BRACE_0, currentPosition),
			text.find(HASH, currentPosition), text.find(DOLLAR, currentPosition));

		switch (text.at(currentPosition))
		{
		case BRACE_1:
		{
			++elementIndex;
			new_elements.push_back(new blocksBorders(variables::console_x, variables::console_y));
			new_elements.at(functions::maxInVector(blocksIndices))->addConnection(new_elements.at(elementIndex));
			blocksIndices.push_back(elementIndex);
			break;
		}
		case BRACE_0:
		{
			blocksIndices.pop_back();
			break;
		}
		case HASH:
		{
			size_t strGap = text.find(' ', currentPosition++);
			std::string attribute(text.substr(currentPosition, strGap - currentPosition));
			currentPosition += attribute.size() + 1;
			if (attribute == "align")
			{
				size_t strEnd = text.find('\n', currentPosition);
				new_elements.at(functions::maxInVector(blocksIndices))
					->set_align(functions::stringToAlignment(text.substr(currentPosition, strEnd - currentPosition)));
			}
			else if (attribute == "border")
			{
				size_t strGap = text.find(' ', currentPosition);
				unsigned count = std::stoi(text.substr(currentPosition, strGap - currentPosition));

				new_elements.at(functions::maxInVector(blocksIndices))->set_border(count, text.at(strGap + 1));
			}
			else if (attribute == "margin")
			{
				size_t strEnd = text.find('\n', currentPosition);

				new_elements.at(functions::maxInVector(blocksIndices))
					->set_margin(functions::stringToIndents(text.substr(currentPosition, strEnd - currentPosition)));
			}
			else if (attribute == "padding")
			{
				size_t strEnd = text.find('\n', currentPosition);

				new_elements.at(functions::maxInVector(blocksIndices))
					->set_padding(functions::stringToIndents(text.substr(currentPosition, strEnd - currentPosition)));
			}
			else if (attribute == "mask")
			{
				new_elements.at(functions::maxInVector(blocksIndices))->set_mask(text.at(currentPosition));
				new_elements.at(functions::maxInVector(blocksIndices))->set_mask_mode(true);
			}
			else if (attribute == "width")
			{
				size_t strEnd = text.find('\n', currentPosition);
				unsigned number = std::stoi(text.substr(currentPosition, strEnd - currentPosition));
				new_elements.at(functions::maxInVector(blocksIndices))->set_width(number);
			}
			break;
		}
		case DOLLAR:
		{
			++elementIndex;
			new_elements.push_back(new blocksLine(st_x, st_y));
			new_elements.at(functions::maxInVector(blocksIndices))->addConnection(new_elements.at(elementIndex));

			size_t strEnd = text.find('\n', currentPosition);
			currentPosition += 1;
			new_elements.at(elementIndex)->set_text(text.substr(currentPosition, strEnd - currentPosition));
			break;
		}
		}
	}
	new_elements.at(root)->configureDimensions(coordinates(st_x, st_y));
	new_elements.at(root)->configureCoordinatesByAlign(0);
	new_elements.at(root)->configureTextByMask();

	return new_elements;
}