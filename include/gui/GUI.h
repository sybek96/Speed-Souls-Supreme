#ifndef GUI_H
#define GUI_H


#include <SFML\Graphics.hpp>
#include "Widget.h"
#include "Label.h"
#include "Button.h"
#include "Slider.h"
#include "CheckBox.h"
#include <vector>
#include <memory>
#include <functional>

/// <summary>
/// @author Sebastian Kruzel & Rafael Plugge
/// @version 9000.0
/// @brief The GUI class will controll all widgets currently on screen.
/// 
/// 
/// </summary>
class GUI
{
public:
	/// <summary>
	/// defines the different types of widgets,
	/// used in file loading
	/// </summary>
	enum class Widgets
	{
		Label, Button, Slider, Checkbox
	};

	/// <summary>
	/// defines the different types of layouts
	/// </summary>
	enum class Layouts
	{
		Custom, StackVertically, StripDiagonal
	};

	// constructor
	GUI(std::shared_ptr<Xbox360Controller>, bool stripDraw = false);

	// destructor
	~GUI();

	// Update GUI
	void update(float dt);

	// Draw the GUI
	void draw(sf::RenderTarget&, sf::RenderStates rendState = sf::RenderStates::Default) const;

	// Configure the GUI
	void configure(const Layouts&);

	// Add a label to widgets vector
	void addLabel(sf::String contents, unsigned int fontSize, sf::Vector2f position, sf::Font & font, sf::Color color = sf::Color::White);

	// Add a button to widgets vector
	void addButton(	std::function<void()> function
					, sf::String message
					, sf::Vector2f position
					, sf::Font & font
					, unsigned int fontSize
					, sf::Texture & texture
					, sf::IntRect leftTextRect
					, sf::IntRect middleTextRect
					, sf::IntRect rightTextRect);

	// Add a slider to widgets vector
	void addSlider( sf::Font & font
					,sf::String name
					,unsigned int fontSize
					,sf::Vector2f position
					,float sliderWidth
					,float sliderHeight
					,float minValue
					,float maxValue
					,float& currentValue
					,sf::Texture & texture
					,sf::IntRect emptyTextRect
					,sf::IntRect filledTextRect
					,sf::IntRect squareTextRect);

	// Add a checkbox to the GUI
	void addCheckbox( sf::Font & font
					, sf::String name
					, sf::Vector2f position
					, float scale
					, sf::Texture & m_texture
					, sf::IntRect textRectOn
					, sf::IntRect textRectOff
					, bool & state
					, unsigned charSize);

	void clear();

private:
	// link last widget, to its previous eligible element 
	void linkWidget();
	// processes xbox controller widget navigation
	void processInput();

	/// vector of shared pointers to widgets
	std::vector<std::shared_ptr<Widget>> m_widgets;

	/// number of widgets that are can be layed out
	int m_layoutNr;

	/// screen margin
	const sf::Vector2f m_screenMargin = sf::Vector2f(10.0f, 100.0f);

	/// shared pointer to controller
	std::shared_ptr<Xbox360Controller> m_controller;
	/// widget the controller is currently selected
	std::shared_ptr<Widget> m_selectedWidget;

	//the rectangle strip that is underneath the menu layer
	sf::RectangleShape m_rectangleStrip;

	//bool that checks if we should draw the rectangle strip
	bool m_drawStrip;
};
#endif // !GUI_
