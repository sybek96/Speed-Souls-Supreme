#include "gui\Label.h"


Label::Label()
{
}

/// <summary>
/// Constructor of a label
/// </summary>
/// <param name="contents">message to display</param>
/// <param name="font">font</param>
/// <param name="fontSize">char size</param>
/// <param name="position">position on screen</param>
Label::Label(sf::String contents, unsigned int fontSize, sf::Vector2f position, sf::Font & font, sf::Color color)
	: Widget()
	, m_text()
	, m_position(position)
	, m_font(font)
	, m_fontSize(fontSize)
{
	m_font = font;
	m_text.setFont(m_font);
	m_text.setColor(color);
	m_text.setString(contents);
	m_text.setPosition(position);
	m_text.setCharacterSize(m_fontSize);
	m_text.setOrigin(m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().height / 2);
}

/// <summary>
/// Destructor of the label object
/// </summary>
Label::~Label()
{
}

/// <summary>
/// Set the font of label to passed font
/// </summary>
/// <param name="font">font</param>
void Label::setFont(sf::Font font)
{
	m_font = font;
}

/// <summary>
/// An overloaded draw function
/// draws the label to screen
/// </summary>
/// <param name="window">game window</param>
/// <param name="state">render state</param>
void Label::draw(sf::RenderTarget & window, sf::RenderStates state) const
{
	window.draw(m_text, state);
}

/// <summary>
/// A getter for size of the label
/// will return a width and height of the text
/// </summary>
/// <returns></returns>
sf::Vector2f Label::getSize()
{
	return sf::Vector2f(m_text.getLocalBounds().width, m_text.getLocalBounds().height);
}

/// <summary>
/// method to scale the text
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void Label::setScale(float x, float y)
{
	m_text.setScale(x, y);
}

/// <summary>
/// Method to set position of the label
/// </summary>
/// <param name="position"></param>
void Label::setPosition(sf::Vector2f position)
{
	m_position = position;
	m_text.setPosition(m_position);
}

/// <summary>
/// Returns the position of the label
/// </summary>
/// <returns></returns>
sf::Vector2f Label::getPosition()
{
	return m_position;
}

/// <summary>
/// Sets the string of text object to passed parameter
/// </summary>
/// <param name="string"></param>
void Label::setText(sf::String string)
{
	m_text.setString(string);
}

void Label::getFocus()
{
}

void Label::loseFocus()
{
}

/// <summary>
/// check if this widget should process input
/// </summary>
/// <param name="controller"></param>
/// <returns></returns>
bool Label::processInput(Xbox360Controller & controller)
{
	return false;
}

/// <summary>
/// set origin of text about its centre
/// </summary>
void Label::setOrigin()
{
	m_text.setOrigin(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height / 2);
}
