#pragma once

#include "SFML\Audio.hpp"
#include "SFML\Graphics.hpp"
#include "Thor\Resources.hpp"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief namespace for all type of game assets.
/// 
/// 
/// </summary>
namespace Assets
{
	/// <summary>
	/// Different types of shaders
	/// </summary>
	enum class Shaders
	{
		Simple, Light
	};

	/// <summary>
	/// @brief Contains all types of textures.
	/// 
	/// 
	/// </summary>
	namespace Textures
	{
		enum class Particles
		{
			Radial,
			Cone
		};

		enum class Tracks
		{
			Track,
			Background,
			Preview,
			Prev1,
			Prev2,
			Prev3,
			Last
		};

		/// <summary>
		/// Different track types
		/// </summary>
		enum class Track
		{
			CheckStart,
			Straight,
			CurveRight,
			CurveLeft,
			CurveLongLeft,
			CurveLongRight
		};

		/// <summary>
		/// Different car types
		/// </summary>
		enum class Car
		{
			Silver = 1,
			Blue,
			Yellow,
			Red,
			Last
		};
		/// <summary>
		/// Different GUI textures
		/// </summary>
		enum class GUI
		{
			Button,
			Slider,
			Checkbox,
			Background,
			Last
		};
	}

	/// <summary>
	/// @brief Contains all types of sounds.
	/// 
	/// 
	/// </summary>
	namespace Sounds
	{
		/// <summary>
		/// 
		/// </summary>
		enum class SoundEffects
		{
			ButtonClick,
			SliderMove,
			Engine,
			Handbrake,
			None
		};

		enum class Songs
		{
			SandyBeach,
			GreenHill,
			RockyRoad,
			MenuSong,
			None
		};
		/// <summary>
		/// Different engine types,
		/// order is very important as its in the
		/// same order as its loaded in.
		/// </summary>
		enum class Engine
		{
			Idle,
			Start
		};
	}

	

	/// <summary>
	/// @brief Contains all types of fonts.
	/// 
	/// 
	/// </summary>
	namespace Fonts
	{
		/// <summary>
		/// Different GUI fonts
		/// </summary>
		enum class GUI
		{
			Content
		};
	}
}

typedef thor::ResourceHolder<sf::Texture, Assets::Textures::Car> TextureCarHolder;
typedef thor::ResourceHolder<sf::Texture, Assets::Textures::Tracks> TextureTrackHolder;
typedef thor::ResourceHolder<sf::Texture, Assets::Textures::GUI> TextureGuiHolder;
typedef thor::ResourceHolder<sf::Texture, Assets::Textures::Particles> TextureParticleHolder;
typedef thor::ResourceHolder<sf::Font, Assets::Fonts::GUI> FontGuiHolder;
typedef thor::ResourceHolder<sf::SoundBuffer, Assets::Sounds::SoundEffects> SoundEffectsHolder;
typedef std::map<Assets::Sounds::Songs, std::shared_ptr<sf::Music>> SongHolder;
typedef thor::ResourceHolder<sf::SoundBuffer, Assets::Sounds::Engine> SoundEngineHolder;
typedef std::vector<std::shared_ptr<sf::Shader>> ShaderHolder;

struct TextureBank
{
	TextureBank()
		: m_car(std::make_shared<TextureCarHolder>())
		, m_track(std::make_shared<TextureTrackHolder>())
		, m_gui(std::make_shared<TextureGuiHolder>())
		, m_particle(std::make_shared<TextureParticleHolder>())
	{}
	std::shared_ptr<TextureCarHolder> m_car;
	std::shared_ptr<TextureTrackHolder> m_track;
	std::shared_ptr<TextureGuiHolder> m_gui;
	std::shared_ptr<TextureParticleHolder> m_particle;
};

struct FontBank
{
	FontBank()
		: m_gui(std::make_shared<FontGuiHolder>())
	{}
	std::shared_ptr<FontGuiHolder> m_gui;
};

struct SoundBank
{
	SoundBank()
		: m_soundEffects(std::make_shared<SoundEffectsHolder>())
		, m_songs(std::make_shared<SongHolder>())
	{}
	std::shared_ptr<SoundEffectsHolder> m_soundEffects;
	std::shared_ptr<SongHolder> m_songs;
};

struct ResourceHolders
{
	ResourceHolders()
		: m_textures(std::make_shared<TextureBank>())
		, m_fonts(std::make_shared<FontBank>())
		, m_sounds(std::make_shared<SoundBank>())
		, m_shaders(std::make_shared<ShaderHolder>())
	{}
	std::shared_ptr<TextureBank> m_textures;
	std::shared_ptr<FontBank> m_fonts;
	std::shared_ptr<SoundBank> m_sounds;
	std::shared_ptr<ShaderHolder> m_shaders;
};