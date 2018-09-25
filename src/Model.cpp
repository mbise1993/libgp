#include "Model.h"
#include <cmath>

namespace libgp
{
	constexpr float EPSILON = 0.00001;

	LyricLine::LyricLine() :
		startingMeasure(0),
		lyrics("")
	{

	}

	LyricLine::LyricLine(uint32_t startingMeasure, std::string lyrics) :
		startingMeasure(startingMeasure),
		lyrics(lyrics)
	{

	}

	PageSetup::PageSetup() :
		sizeX(210),
		sizeY(297),
		marginLeft(10),
		marginRight(10),
		marginTop(15),
		marginBottom(10),
		scoreSizeProportion(1.0),
		headerAndFooter(HeaderFooterElements::All),
		title("%TITLE%"),
		subtitle("%SUBTITLE%"),
		artist("%ARTIST%"),
		album("%ALBUM%"),
		wordsBy("Words by %WORDS%"),
		musicBy("Music by %MUSIC%"),
		wordsAndMusicBy("Words & Music by %WORDSMUSIC%"),
		copyright("Copyright %COPYRIGHT%\nAll Rights Reserved - International Copyright Secured\r\n"),
		pageNumber("Page %N%/%P%")
	{

	}

	constexpr uint8_t DEFAULT_PERCUSSION_CHANNEL = 9;

	bool MidiChannel::isPercussionChannel() const noexcept { return channel % 16 == DEFAULT_PERCUSSION_CHANNEL; }

	bool operator==(const LyricLine& lhs, const LyricLine& rhs) 
	{
		return lhs.startingMeasure == rhs.startingMeasure
			&& lhs.lyrics == rhs.lyrics;
	}

	bool operator==(const Lyrics& lhs, const Lyrics& rhs)
	{
		return lhs.trackNumber == rhs.trackNumber
			&& lhs.lines == rhs.lines;
	}

	bool operator==(const PageSetup& lhs, const PageSetup& rhs)
	{
		return lhs.sizeX == rhs.sizeX
			&& lhs.sizeY == rhs.sizeY
			&& lhs.marginLeft == rhs.marginLeft
			&& lhs.marginRight == rhs.marginRight
			&& lhs.marginTop == rhs.marginTop
			&& lhs.marginBottom == rhs.marginBottom
			&& std::abs(lhs.scoreSizeProportion - rhs.scoreSizeProportion) < EPSILON
			&& lhs.headerAndFooter == rhs.headerAndFooter
			&& lhs.title == rhs.title
			&& lhs.subtitle == rhs.subtitle
			&& lhs.artist == rhs.artist
			&& lhs.album == rhs.album
			&& lhs.wordsBy == rhs.wordsBy
			&& lhs.musicBy == rhs.musicBy
			&& lhs.wordsAndMusicBy == rhs.wordsAndMusicBy
			&& lhs.pageNumber == rhs.pageNumber;
	}

	bool operator==(const Tempo& lhs, const Tempo& rhs)
	{
		return lhs.name == rhs.name
			&& lhs.value == rhs.value
			&& lhs.isHidden == rhs.isHidden;
	}

	bool operator==(const MidiChannel& lhs, const MidiChannel& rhs)
	{
		return lhs.channel == rhs.channel
			&& lhs.effectChannel == rhs.effectChannel
			&& lhs.instrument == rhs.instrument
			&& lhs.volume == rhs.volume
			&& lhs.balance == rhs.balance
			&& lhs.chorus == rhs.channel
			&& lhs.reverb == rhs.reverb
			&& lhs.phaser == rhs.phaser
			&& lhs.tremolo == rhs.tremolo;
	}

	bool operator==(const RSEMasterEffects& lhs, const RSEMasterEffects& rhs)
	{
		return lhs.volume == rhs.volume
			&& lhs.reverb == rhs.reverb;
	}

	bool operator==(const Song& lhs, const Song& rhs)
	{
		return lhs.title == rhs.title
			&& lhs.subtitle == rhs.subtitle
			&& lhs.artist == rhs.artist
			&& lhs.album == rhs.album
			&& lhs.lyricsWriter == rhs.lyricsWriter
			&& lhs.musicWriter == rhs.musicWriter
			&& lhs.copyright == rhs.copyright
			&& lhs.tabAuthor == rhs.tabAuthor
			&& lhs.instructions == rhs.instructions
			&& lhs.comments == rhs.comments
			&& lhs.lyrics == rhs.lyrics
			&& lhs.tempo == rhs.tempo
			&& lhs.keySignature == rhs.keySignature
			&& lhs.octave == rhs.octave;
	}
}