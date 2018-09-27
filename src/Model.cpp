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

	uint32_t Tuplet::convertTime(uint32_t time) const { return time * times / enters; }

	Duration::Duration() :
		value(Quarter)
	{

	}

	uint32_t Duration::calcTime() const
	{
		auto result = static_cast<uint32_t>(QuarterTime * (4.0 / value));
		if (isDotted) {
			result += result / 2;
		} else if (isDoubleDotted) {
			result += (result / 4) * 3;
		}

		return tuplet.convertTime(result);
	}

	TimeSignature::TimeSignature() :
		numerator(4),
		beams({2, 2, 2, 2})
	{

	}

	Color::Color() :
		red(255),
		green(0),
		blue(0),
		alpha(1)
	{

	}

	MeasureHeader::MeasureHeader() :
		start(Duration::QuarterTime),
		hasDoubleBar(false),
		keySignature(KeySignature::CMajor),
		tempo(120),
		isRepeatOpen(false),
		repeatAlternative(0),
		repeatClose(-1),
		tripletFeel(TripletFeel::None),
		direction(""),
		fromDirection("")
	{

	}

	uint32_t MeasureHeader::calcLength() const { return timeSignature.numerator * timeSignature.denominator.calcTime(); }

	Voice::Voice(Measure& measure) :
		measure(measure)
	{

	}

	Measure::Measure(Track& track, MeasureHeader& header) :
		track(track),
		header(header)
	{
		for (auto i = 0; i < MaxVoices; ++i) {
			voices.push_back(Voice(*this));
		}
	}

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

	bool operator==(const RSEEqualizer& lhs, const RSEEqualizer& rhs)
	{
		return lhs.gain == rhs.gain
			&& lhs.knobs == rhs.knobs;
	}

	bool operator==(const RSEMasterEffect& lhs, const RSEMasterEffect& rhs)
	{
		return lhs.volume == rhs.volume
			&& lhs.reverb == rhs.reverb
			&& lhs.equalizer == rhs.equalizer;
	}

	bool operator==(const Tuplet& lhs, const Tuplet& rhs)
	{
		return lhs.times == rhs.times
			&& lhs.enters == rhs.enters;
	}

	bool operator==(const Duration& lhs, const Duration& rhs)
	{
		return lhs.value == rhs.value
			&& lhs.isDotted == rhs.isDotted
			&& lhs.isDoubleDotted == rhs.isDoubleDotted
			&& lhs.tuplet == rhs.tuplet;
	}

	bool operator==(const TimeSignature& lhs, const TimeSignature& rhs)
	{
		return lhs.numerator == rhs.numerator
			&& lhs.denominator == rhs.denominator
			&& lhs.beams == rhs.beams;
	}

	bool operator==(const Color& lhs, const Color& rhs)
	{
		return lhs.red == rhs.red
			&& lhs.green == rhs.green
			&& lhs.blue == rhs.blue
			&& lhs.alpha == rhs.alpha;
	}

	bool operator==(const Marker& lhs, const Marker& rhs)
	{
		return lhs.title == rhs.title
			&& lhs.color == rhs.color;
	}

	bool operator==(const MeasureHeader& lhs, const MeasureHeader& rhs)
	{
		return lhs.number == rhs.number
			&& lhs.start == rhs.start
			&& lhs.hasDoubleBar == rhs.hasDoubleBar
			&& lhs.keySignature == rhs.keySignature
			&& lhs.timeSignature == rhs.timeSignature
			&& lhs.tempo == rhs.tempo
			&& lhs.marker == rhs.marker
			&& lhs.isRepeatOpen == rhs.isRepeatOpen
			&& lhs.repeatAlternative == rhs.repeatAlternative
			&& lhs.repeatClose == rhs.repeatClose
			&& lhs.tripletFeel == rhs.tripletFeel
			&& lhs.direction == rhs.direction
			&& lhs.fromDirection == rhs.fromDirection;
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