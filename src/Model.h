#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

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

	// TODO: support minor
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
		int32_t instrument;
		int16_t volume;
		int16_t balance;
		int16_t chorus;
		int16_t reverb;
		int16_t phaser;
		int16_t tremolo;
		uint8_t bank;

		bool isPercussionChannel() const noexcept;
	};

	struct Tuplet
	{
		uint8_t enters;
		uint8_t times;

		uint32_t convertTime(uint32_t time) const;
	};

	struct Duration
	{
		static const uint32_t QuarterTime = 960;
		static const uint8_t Whole = 1;
		static const uint8_t Half = 2;
		static const uint8_t Quarter = 4;
		static const uint8_t Eighth = 8;
		static const uint8_t Sixteenth = 16;
		static const uint8_t ThirtySecond = 32;
		static const uint8_t SixtyFourth = 64;
		static const uint8_t HundredTwentyEights = 128;
		static const uint8_t MinTime = (QuarterTime * (4 / SixtyFourth)) * 2 / 3;

		uint8_t value;
		bool isDotted;
		bool isDoubleDotted;
		Tuplet tuplet;

		Duration();

		uint32_t calcTime() const;
	};

	struct TimeSignature
	{
		int8_t numerator;
		Duration denominator;
		std::vector<uint8_t> beams;

		TimeSignature();
	};

	struct Color
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;

		Color();
	};

	struct Marker
	{
		std::string title;
		Color color;
	};

	enum class TripletFeel : uint8_t
	{
		None = 0, Eighth = 1, Sixteenth = 2
	};

	struct MeasureHeader
	{
		uint32_t number;
		uint32_t start;
		bool hasDoubleBar;
		KeySignature keySignature;
		TimeSignature timeSignature;
		uint32_t tempo;
		Marker marker;
		bool isRepeatOpen;
		uint8_t repeatAlternative;
		int8_t repeatClose;
		TripletFeel tripletFeel;
		std::string direction;
		std::string fromDirection;

		MeasureHeader();
		uint32_t calcLength() const;
	};

	enum class Clef : uint8_t
	{
		Treble = 0,
		Bass = 1,
		Tenor = 2,
		Alto = 3
	};

	enum class VoiceDirection : uint8_t
	{
		None = 0,
		Up = 1,
		Down = 2
	};

	enum class BendType : uint8_t
	{
		None = 0,
		Bend = 1,
		BendRelease = 2,
		BendReleaseBend = 3,
		Prebend = 4,
		PrebendRelease = 5,
		Dip = 6,
		Dive = 7,
		ReleaseUp = 8,
		InvertedDip = 9,
		Return = 10,
		ReleaseDown = 11
	};

	struct BendPoint
	{
		uint8_t position;
		uint8_t value;
		bool hasVibrato;
	};

	struct Bend
	{
		static const uint8_t SemitoneLength = 1;
		static const uint8_t MaxPosition = 12;
		static const uint8_t MaxValue = SemitoneLength * MaxPosition;

		BendType type;
		uint8_t value;
		std::vector<BendPoint> points;
	};

	enum class GraceTransition : uint8_t
	{
		None = 0,
		Slide = 1,
		Bend = 2,
		Hammer = 3
	};

	struct Grace
	{
		uint8_t duration;
		uint8_t fret;
		bool isDead;
		bool isOnBeat;
		GraceTransition transition;
		uint8_t velocity;
	};

	enum class HarmonicType : uint8_t
	{
		Natural = 1,
		Artificial = 2,
		Tapped = 3,
		Pinch = 4,
		Semi = 5
	};

	struct Harmonic
	{
		HarmonicType type;
		uint8_t pitch;
		uint8_t octave;
		uint8_t fret;
	};

	enum class Fingering : int8_t
	{
		Unknown = -2,
		Open = -1,
		Thumb = 0,
		Index = 1,
		Middle = 2,
		Annular = 3,
		Little = 4
	};

	enum class SlideType : int8_t
	{
		IntoFromAbove = -2,
		IntoFromBelow = -1,
		None = 0,
		ShiftSlideTo = 1,
		LegatoSlideTo = 2,
		OutDownwards = 3,
		OutUpwards = 4
	};

	struct TremoloPicking
	{
		uint8_t duration;
	};

	struct Trill
	{
		uint8_t fret;
		uint8_t duration;
	};

	struct NoteEffect
	{
		bool isAccentuated;
		Bend bend;
		bool isGhostNote;
		Grace grace;
		bool isHammer;
		Harmonic harmonic;
		bool isHeavyAccentuatedNote;
		Fingering leftHandFingering;
		bool letRing;
		bool palmMute;
		Fingering rightHandFingering;
		std::vector<SlideType> slides;
		bool isStaccato;
		TremoloPicking tremoloPicking;
		Trill trill;
		bool hasVibrato;
	};

	enum class NoteType : uint8_t
	{
		Rest = 0,
		Normal = 1,
		Tie = 2,
		Dead = 3
	};

	struct Note
	{
		uint8_t value;
		uint8_t velocity;
		uint8_t string;
		NoteEffect effect;
		float durationPercent;
		bool swapAccidentals;
		NoteType type;
	};

	enum class BeatStrokeDirection : uint8_t
	{
		None = 0,
		Up = 1,
		Down = 2
	};

	struct BeatStroke
	{
		BeatStrokeDirection direction;
		uint8_t value;
	};

	enum class SlapEffect : uint8_t
	{
		None = 0,
		Tapping = 1,
		Slapping = 2,
		Popping = 3
	};

	enum class ChordType : uint8_t
	{
		Major = 0,
		Seventh = 1,
		MajorSeventh = 2,
		Sixth = 3,
		Minor = 4,
		MinorSeventh = 5,
		MinorMajor = 6,
		MinorSixth = 7,
		SuspendedSecond = 8,
		SuspendedFourth = 9,
		SeventhSuspendedSecond = 10,
		SeventhSuspendedFourth = 11,
		Diminished = 12,
		Augmented = 13,
		Power = 14
	};

	enum class ChordExtension : uint8_t
	{
		None = 0,
		Ninth = 1,
		Eleventh = 2,
		Thirteenth = 3
	};

	struct GuitarString
	{
		uint8_t number;
		uint8_t value;
	};

	struct Barre
	{
		uint8_t fret;
		uint8_t start;
		uint8_t end;
	};

	struct Chord
	{
		uint8_t length;
		bool isSharp;
		uint8_t root;
		ChordType type;
		ChordExtension extension;
		uint8_t bass;
		uint8_t tonality;
		uint8_t add;
		std::string name;
		uint8_t fifth;
		uint8_t ninth;
		uint8_t eleventh;
		uint32_t firstFret;
		std::vector<int32_t> strings;
		std::vector<Barre> barres;
		std::vector<bool> omissions;
		std::vector<Fingering> fingerings;
		uint8_t show;
		uint8_t isNewFormat;
	};

	struct WahEffect
	{
		int8_t value = -1;
		bool display = false;
	};

	struct RSEInstrument
	{
		int32_t instrument = -1;
		uint32_t unknown = 1;
		int32_t soundBank = -1;
		int32_t effectNumber = -1;
		std::string effectCategory;
		std::string effect;
	};

	struct MixTableChange
	{
		uint8_t instrument;
		RSEInstrument rse;
		uint8_t volume;
		uint8_t balance;
		uint8_t chorus;
		uint8_t reverb;
		uint8_t phaser;
		uint8_t tremolo;
		Tempo tempo;
		WahEffect wah;
		bool useRSE;
	};

	struct BeatEffect
	{
		BeatStroke stroke;
		bool hasRasgueado;
		BeatStrokeDirection pickStroke;
		Chord chord;
		bool hasFadeIn;
		Bend tremoloBar;
		MixTableChange mixTableChange;
		SlapEffect slapEffect;
		uint8_t vibrato;
	};

	enum class Octave : uint8_t
	{
		None = 0,
		Ottava = 1,
		Quindicesima = 2,
		OttavaBassa = 3,
		QuindicesimaBassa = 4,
	};

	enum class TupletBracket : uint8_t
	{
		None = 0,
		Start = 1,
		End = 2
	};

	struct BeatDisplay
	{
		bool breakBeam;
		bool forceBeam;
		VoiceDirection beamDirection;
		TupletBracket tupletBracket;
		uint8_t breakSecondary;
		bool breakSecondaryTuplet;
		bool forceBracket;
	};

	enum class BeatStatus : uint8_t
	{
		Empty = 0,
		Normal = 1,
		Rest = 2
	};

	struct Beat
	{
		std::vector<Note> notes;
		uint8_t duration;
		std::string text;
		uint8_t start;
		BeatEffect effect;
		uint8_t index;
		Octave octave;
		BeatDisplay display;
		BeatStatus status;
	};

	struct Measure;
	struct Voice
	{
		Measure& measure;
		std::vector<Beat> beats;
		VoiceDirection direction;

		explicit Voice(Measure& measure);
	};

	enum class LineBreak : uint8_t
	{
		None = 0,
		Break = 1,
		Protect = 2
	};

	struct Track;
	struct Measure
	{
		static const uint8_t MaxVoices = 2;

		Track& track;
		MeasureHeader& header;
		Clef clef;
		std::vector<Voice> voices;
		LineBreak lineBreak;

		Measure(Track& track, MeasureHeader& header);
	};

	struct TrackSettings
	{
		bool showTablature = true;
		bool showNotation = true;
		bool diagramsAreBelow = false;
		bool showRhythm = false;
		bool forceHorizontal = false;
		bool forceChannels = false;
		bool showDiagramList = true;
		bool showDiagramsInScore = false;
		bool autoLetRing = false;
		bool autoBrush = false;
		bool extendRhythmic = false;
	};

	struct RSEEqualizer
	{
		std::vector<float> knobs;
		float gain;
	};

	enum class Accentuation : uint8_t
	{
		None = 0,
		VerySoft = 1,
		Soft = 2,
		Medium = 3,
		Strong = 4,
		VeryStrong = 5
	};

	struct TrackRSE
	{
		RSEInstrument instrument;
		RSEEqualizer equalizer;
		uint8_t humanize;
		Accentuation autoAccentuation;
	};

	struct Track
	{
		uint32_t number;
		uint32_t numFrets;
		uint32_t offset;
		bool isPercussionTrack;
		bool is12StringGuitarTrack;
		bool isBanjoTrack;
		bool isVisible;
		bool isSolo;
		bool isMute;
		bool indicateTuning;
		std::string name;
		std::vector<Measure> measures;
		std::vector<GuitarString> strings;
		uint8_t port;
		std::optional<MidiChannel> channel;
		Color color;
		TrackSettings settings;
		bool useRSE;
		TrackRSE rse;
	};

	struct RSEMasterEffect
	{
		uint8_t volume;
		uint32_t reverb;
		RSEEqualizer equalizer;
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

		RSEMasterEffect masterEffect;
		Lyrics lyrics;
		PageSetup pageSetup;
		Tempo tempo;
		KeySignature keySignature;
		uint32_t octave;
		std::vector<MeasureHeader> measureHeaders;
		std::vector<Track> tracks;
	};

	bool operator==(const Lyrics& lhs, const Lyrics& rhs);
	bool operator==(const Lyrics& lhs, const Lyrics& rhs);
	bool operator==(const PageSetup& lhs, const PageSetup& rhs);
	bool operator==(const Tempo& lhs, const Tempo& rhs);
	bool operator==(const MidiChannel& lhs, const MidiChannel& rhs);
	bool operator==(const RSEEqualizer& lhs, const RSEEqualizer& rhs);
	bool operator==(const RSEMasterEffect& lhs, const RSEMasterEffect& rhs);
	bool operator==(const Tuplet& lhs, const Tuplet& rhs);
	bool operator==(const Duration& lhs, const Duration& rhs);
	bool operator==(const TimeSignature& lhs, const TimeSignature& rhs);
	bool operator==(const Color& lhs, const Color& rhs);
	bool operator==(const Marker& lhs, const Marker& rhs);
	bool operator==(const MeasureHeader& lhs, const MeasureHeader& rhs);
	bool operator==(const Song& lhs, const Song& rhs);
}