#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace libgp
{
	struct LyricLine
	{
		uint32_t startingMeasure;
		std::string lyrics;

		LyricLine();
		LyricLine(uint32_t startingMeasure, std::string lyrics);
	};

	struct Lyrics
	{
		uint32_t trackNumber;
		std::vector<LyricLine> lines;
	};

	enum class HeaderFooterElements : uint16_t
	{
		None = 0x0000,
		Title = 0x0001,
		Subtitle = 0x0002,
		Artist = 0x0004,
		Album = 0x0008,
		WordsBy = 0x0010,
		MusicBy = 0x0020,
		WordsAndMusicBy = 0x0040,
		Copyright = 0x0080,
		PageNumber = 0x0100,
		All = Title | Subtitle | Artist | Album | WordsBy | MusicBy | WordsAndMusicBy | Copyright | PageNumber
	};

	struct PageSetup
	{
		uint32_t sizeX;
		uint32_t sizeY;
		uint32_t marginLeft;
		uint32_t marginRight;
		uint32_t marginTop;
		uint32_t marginBottom;
		float scoreSizeProportion;
		HeaderFooterElements headerAndFooter;
		std::string title;
		std::string subtitle;
		std::string artist;
		std::string album;
		std::string wordsBy;
		std::string musicBy;
		std::string wordsAndMusicBy;
		std::string copyright;
		std::string pageNumber;

		PageSetup();
	};

	struct Tempo
	{
		std::string name;
		uint32_t value;
		bool isHidden;
	};	

	enum class KeySignature : int8_t
	{
		FMajorFlat = -8,
		CMajorFlat = -7,
		GMajorFlat = -6,
		DMajorFlat = -5,
		AMajorFlat = -4,
		EMajorFlat = -3,
		BMajorFlat = -2,
		FMajor = -1,
		CMajor = 0,
		GMajor = 1,
		DMajor = 2,
		AMajor = 3,
		EMajor = 4,
		BMajor = 5,
		FMajorSharp = 6,
		CMajorSharp = 7,
		GMajorSharp = 8
	};

	struct MidiChannel
	{
		uint8_t channel;
		uint8_t effectChannel;
		uint32_t instrument;
		int16_t volume;
		int16_t balance;
		int16_t chorus;
		int16_t reverb;
		int16_t phaser;
		int16_t tremolo;

		bool isPercussionChannel() const noexcept;
	};

	struct RSEMasterEffects
	{
		uint8_t volume;
		uint32_t reverb;
	};

	struct Song
	{
		std::string title;
		std::string subtitle;
		std::string artist;
		std::string album;
		std::string lyricsWriter;
		std::string musicWriter;
		std::string copyright;
		std::string tabAuthor;
		std::string instructions;
		std::vector<std::string> comments;

		RSEMasterEffects rseMasterEffects;
		Lyrics lyrics;
		PageSetup pageSetup;
		Tempo tempo;
		KeySignature keySignature;
		uint32_t octave;
	};

	bool operator==(const Lyrics& lhs, const Lyrics& rhs);
	bool operator==(const Lyrics& lhs, const Lyrics& rhs);
	bool operator==(const PageSetup& lhs, const PageSetup& rhs);
	bool operator==(const Tempo& lhs, const Tempo& rhs);
	bool operator==(const MidiChannel& lhs, const MidiChannel& rhs);
	bool operator==(const RSEMasterEffects& lhs, const RSEMasterEffects& rhs);
	bool operator==(const Song& lhs, const Song& rhs);
}