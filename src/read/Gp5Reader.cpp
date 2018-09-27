#include "Gp5Reader.h"
#include "StreamReader.h"
#include "Model.h"
#include <utility>

namespace libgp
{
	Gp5Reader::Gp5Reader() :
		GpReaderBase({
			GpVersion::parse("FICHIER GUITAR PRO v5.00"), 
			GpVersion::parse("FICHIER GUITAR PRO v5.10")
		})
	{

	}

	std::unique_ptr<Song> Gp5Reader::readSong(std::istream& stream)
	{
		StreamReader reader(stream);
		readAndValidateVersion(reader);

		auto song = new Song();

		readSongInfo(*song, reader);
		song->lyrics = readLyrics(reader);

		// Possibly master RSE EQ?
		reader.skip(19);

		song->pageSetup = readPageSetup(reader);
		song->tempo = readTempo(reader);
		song->keySignature = static_cast<KeySignature>(reader.readSignedByte());
		song->octave = reader.readUnsignedInt();

		auto midiChannels = readMidiChannels(reader);
		auto directionSigns = readDirectionSigns(reader);

		song->masterEffect.reverb = reader.readUnsignedInt();

		auto numMeasures = reader.readUnsignedInt();
		auto numTracks = reader.readUnsignedInt();
		readMeasureHeaders(numMeasures, *song, directionSigns, reader);
		readTracks(numTracks, *song, midiChannels, reader);
		readMeasures(*song, reader);

		return std::unique_ptr<Song>(song);
	}

	void Gp5Reader::readSongInfo(Song& song, StreamReader& reader) const
	{
		song.title = reader.readIntByteSizedString();
		song.subtitle = reader.readIntByteSizedString();
		song.artist = reader.readIntByteSizedString();
		song.album = reader.readIntByteSizedString();
		song.lyricsWriter = reader.readIntByteSizedString();
		song.musicWriter = reader.readIntByteSizedString();
		song.copyright = reader.readIntByteSizedString();
		song.tabAuthor = reader.readIntByteSizedString();
		song.instructions = reader.readIntByteSizedString();
		
		auto numComments = reader.readUnsignedInt();
		for (auto i = 0; i < numComments; ++i) {
			song.comments.push_back(reader.readIntByteSizedString());
		}
	}

	Lyrics Gp5Reader::readLyrics(StreamReader& reader) const
	{
		Lyrics lyrics;
		lyrics.trackNumber = reader.readUnsignedInt();

		for (auto i = 0; i < 5; ++i) {
			LyricLine line;
			line.startingMeasure = reader.readUnsignedInt();
			line.lyrics = reader.readIntSizedString();

			lyrics.lines.push_back(line);
		}

		return lyrics;
	}

	PageSetup Gp5Reader::readPageSetup(StreamReader& reader) const
	{
		PageSetup pageSetup;

		pageSetup.sizeX = reader.readUnsignedInt();
		pageSetup.sizeY = reader.readUnsignedInt();
		pageSetup.marginLeft = reader.readUnsignedInt();
		pageSetup.marginRight = reader.readUnsignedInt();
		pageSetup.marginTop = reader.readUnsignedInt();
		pageSetup.marginBottom = reader.readUnsignedInt();
		pageSetup.scoreSizeProportion = reader.readUnsignedInt() / 100.0f;
		pageSetup.headerAndFooter = static_cast<HeaderFooterElements>(reader.readUnsignedShort());
		pageSetup.title = reader.readIntByteSizedString();
		pageSetup.subtitle = reader.readIntByteSizedString();
		pageSetup.artist = reader.readIntByteSizedString();
		pageSetup.album = reader.readIntByteSizedString();
		pageSetup.wordsBy = reader.readIntByteSizedString();
		pageSetup.musicBy = reader.readIntByteSizedString();
		pageSetup.wordsAndMusicBy = reader.readIntByteSizedString();
		auto copyrightLine1 = reader.readIntByteSizedString();
		auto copyRightLine2 = reader.readIntByteSizedString();
		pageSetup.copyright = copyrightLine1 + "\n" + copyRightLine2;
		pageSetup.pageNumber = reader.readIntByteSizedString();

		return pageSetup;
	}

	Tempo Gp5Reader::readTempo(StreamReader& reader) const
	{
		Tempo tempo;

		tempo.name = reader.readIntByteSizedString();
		tempo.value = reader.readUnsignedInt();
		tempo.isHidden = (version().major() == 5 && version().minor() > 0)
			? reader.readBoolean()
			: false;

		return tempo;
	}

	std::vector<MidiChannel> Gp5Reader::readMidiChannels(StreamReader& reader) const
	{
		std::vector<MidiChannel> channels{64};

		for (auto i = 0; i < 64; ++i) {
			MidiChannel channel;
			channel.channel = i;
			channel.effectChannel = i;
			auto instrument = reader.readSignedInt();
			channel.instrument = instrument == -1 ? 0 : instrument;
			channel.volume = byteToChannelShort(reader.readSignedByte());
			channel.balance = byteToChannelShort(reader.readSignedByte());
			channel.chorus = byteToChannelShort(reader.readSignedByte());
			channel.reverb = byteToChannelShort(reader.readSignedByte());
			channel.phaser = byteToChannelShort(reader.readSignedByte());
			channel.tremolo = byteToChannelShort(reader.readSignedByte());

			channels.push_back(channel);

			// Backwards compatibility with GP3
			reader.skip(2);
		}

		return channels;
	}

	std::tuple<Gp5Reader::DirectionSigns, Gp5Reader::DirectionSigns> Gp5Reader::readDirectionSigns(StreamReader& reader) const
	{
		DirectionSigns signs;
		signs["Coda"] = reader.readSignedShort();
		signs["Double Coda"] = reader.readSignedShort();
		signs["Segno"] = reader.readSignedShort();
		signs["Segno Segno"] = reader.readSignedShort();
		signs["Fine"] = reader.readSignedShort();

		DirectionSigns fromSigns;
		fromSigns["Da Capo"] = reader.readSignedShort();
		fromSigns["Da Capo al Coda"] = reader.readSignedShort();
		fromSigns["Da Capo al Double Coda"] = reader.readSignedShort();
		fromSigns["Da Capo al Fine"] = reader.readSignedShort();
		fromSigns["Da Segno"] = reader.readSignedShort();
		fromSigns["Da Segno al Coda"] = reader.readSignedShort();
		fromSigns["Da Segno al Double Coda"] = reader.readSignedShort();
		fromSigns["Da Segno al Fine"] = reader.readSignedShort();
		fromSigns["Da Segno Segno"] = reader.readSignedShort();
		fromSigns["Da Segno Segno al Coda"] = reader.readSignedShort();
		fromSigns["Da Segno Segno al Double Coda"] = reader.readSignedShort();
		fromSigns["Da Segno Segno al Fine"] = reader.readSignedShort();
		fromSigns["Da Coda"] = reader.readSignedShort();
		fromSigns["Da Double Coda"] = reader.readSignedShort();

		return std::make_tuple(signs, fromSigns);
	}

	void Gp5Reader::readMeasureHeaders(uint32_t numMeasures, Song& song, std::tuple<DirectionSigns, DirectionSigns> directionSigns, StreamReader& reader) const
	{
		std::optional<MeasureHeader> previous = std::nullopt;
		for (auto i = 1; i <= numMeasures; ++i) {
			auto header = readMeasureHeader(i, song, previous, reader);
			song.measureHeaders.push_back(header);
			previous = header;
		}

		auto signs = std::get<0>(directionSigns);
		for (auto const&[sign, number] : signs) {
			if (number > -1) {
				song.measureHeaders[number - 1].direction = sign;
			}
		}

		auto fromSigns = std::get<1>(directionSigns);
		for (auto const&[sign, number] : fromSigns) {
			if (number > -1) {
				song.measureHeaders[number - 1].fromDirection = sign;
			}
		}
	}

	MeasureHeader Gp5Reader::readMeasureHeader(uint32_t number, Song& song, std::optional<MeasureHeader> previous, StreamReader& reader) const
	{
		if (previous != std::nullopt) {
			reader.skip(1);
		}

		auto flags = reader.readUnsignedByte();
		
		MeasureHeader header;
		header.number = number;
		header.start = 0;
		header.tempo = song.tempo.value;

		if (flags & 0x01) {
			header.timeSignature.numerator = reader.readSignedByte();
		} else {
			header.timeSignature.numerator = previous->timeSignature.numerator;
		}

		if (flags & 0x02) {
			header.timeSignature.denominator.value = reader.readSignedByte();
		} else {
			header.timeSignature.denominator = previous->timeSignature.denominator;
		}

		header.isRepeatOpen = static_cast<bool>(flags & 0x04);

		if (flags & 0x08) {
			header.repeatClose = reader.readSignedByte();
		}

		if (flags & 0x10) {
			header.repeatAlternative = reader.readUnsignedByte();
		}

		if (flags & 0x20) {
			header.marker = readMarker(reader);
		}

		if (flags & 0x40) {
			auto root = reader.readSignedByte();
			auto type = reader.readSignedByte();
			header.keySignature = static_cast<KeySignature>(root);
		} else if (header.number > 1) {
			header.keySignature = previous->keySignature;
		}

		header.hasDoubleBar = static_cast<bool>(flags & 0x80);

		if (header.repeatClose > -1) {
			header.repeatClose--;
		}

		if (flags & 0x03) {
			header.timeSignature.beams = {
				reader.readUnsignedByte(),
				reader.readUnsignedByte(),
				reader.readUnsignedByte(),
				reader.readUnsignedByte()
			};
		} else {
			header.timeSignature.beams = previous->timeSignature.beams;
		}

		if (flags & 0x10 == 0) {
			reader.skip(1);
		}

		header.tripletFeel = static_cast<TripletFeel>(reader.readUnsignedByte());

		return header;
	}

	Marker Gp5Reader::readMarker(StreamReader& reader) const
	{
		Marker marker;
		marker.title = reader.readIntByteSizedString();
		marker.color = readColor(reader);
		return marker;
	}

	Color Gp5Reader::readColor(StreamReader& reader) const
	{
		Color color;
		color.red = reader.readUnsignedByte();
		color.green = reader.readUnsignedByte();
		color.blue = reader.readUnsignedByte();
		reader.skip(1);
		return color;
	}

	void Gp5Reader::readTracks(uint32_t numTracks, Song& song, std::vector<MidiChannel> midiChannels, StreamReader& reader) const
	{
		for (auto i = 1; i <= numTracks; ++i) {
			auto track = readTrack(i, midiChannels, reader);
			song.tracks.push_back(track);
		}

		if (version().major() == 5 && version().minor() == 0) {
			reader.skip(2);
		} else {
			reader.skip(1);
		}
	}

	Track Gp5Reader::readTrack(uint32_t number, std::vector<MidiChannel> midiChannels, StreamReader& reader) const
	{
		if (number == 1 || (version().major() == 5 && version().minor() == 0)) {
			reader.skip(1);
		}

		auto flags1 = reader.readUnsignedByte();

		Track track;
		track.isPercussionTrack = static_cast<bool>(flags1 & 0x01);
		track.is12StringGuitarTrack = static_cast<bool>(flags1 & 0x02);
		track.isBanjoTrack = static_cast<bool>(flags1 & 0x04);
		track.isVisible = static_cast<bool>(flags1 & 0x08);
		track.isSolo = static_cast<bool>(flags1 & 0x10);
		track.isMute = static_cast<bool>(flags1 & 0x20);
		track.useRSE = static_cast<bool>(flags1 & 0x40);
		track.indicateTuning = static_cast<bool>(flags1 & 0x80);
		track.name = reader.readString(40);
		
		auto numStrings = reader.readUnsignedInt();
		for (auto i = 1; i <= 7; ++i) {
			auto tuning = reader.readUnsignedInt();
			if (numStrings > i) {
				GuitarString string;
				string.number = i;
				string.value = tuning;
				track.strings.push_back(string);
			}
		}

		track.port = reader.readUnsignedInt();
		track.channel = readMidiChannel(midiChannels, reader);
		track.isPercussionTrack = track.channel->channel == 9;
		track.numFrets = reader.readUnsignedInt();
		track.offset = reader.readUnsignedInt();
		track.color = readColor(reader);

		auto flags2 = reader.readUnsignedShort();
		track.settings.showTablature = static_cast<bool>(flags2 & 0x001);
		track.settings.showNotation = static_cast<bool>(flags2 & 0x002);
		track.settings.diagramsAreBelow = static_cast<bool>(flags2 & 0x004);
		track.settings.showRhythm = static_cast<bool>(flags2 & 0x008);
		track.settings.forceHorizontal = static_cast<bool>(flags2 & 0x010);
		track.settings.forceChannels = static_cast<bool>(flags2 & 0x020);
		track.settings.showDiagramList = static_cast<bool>(flags2 & 0x040);
		track.settings.showDiagramsInScore = static_cast<bool>(flags2 & 0x080);
		// TODO: 0x100???
		track.settings.autoLetRing = static_cast<bool>(flags2 & 0x200);
		track.settings.autoBrush = static_cast<bool>(flags2 & 0x400);
		track.settings.extendRhythmic = static_cast<bool>(flags2 & 0x800);

		track.rse.autoAccentuation = static_cast<Accentuation>(reader.readUnsignedByte());
		track.channel->bank = reader.readUnsignedByte();
		readTrackRSE(track.rse, reader);

		return track;
	}

	std::optional<MidiChannel> Gp5Reader::readMidiChannel(std::vector<MidiChannel> midiChannels, StreamReader& reader) const
	{
		auto index = reader.readUnsignedInt() - 1;
		auto effectChannel = reader.readUnsignedInt() - 1;
		if (index < 0 || index >= midiChannels.size()) {
			return std::nullopt;
		}

		auto trackChannel = midiChannels[index];
		if (trackChannel.instrument < 0) {
			trackChannel.instrument = 0;
		}

		if (!trackChannel.isPercussionChannel()) {
			trackChannel.effectChannel = effectChannel;
		}

		return trackChannel;
	}

	void Gp5Reader::readTrackRSE(TrackRSE& rse, StreamReader& reader) const
	{
		rse.humanize = reader.readUnsignedByte();
		reader.skip(12); // TODO: 3 ints???
		reader.skip(12); // TODO: ???
		rse.instrument = readRSEInstrument(reader);

		if (version().major() >= 5 && version().minor() > 0) {
			rse.equalizer = readRSEEqualizer(4, reader);
			rse.instrument.effect = reader.readIntByteSizedString();
			rse.instrument.effectCategory = reader.readIntByteSizedString();
		}
	}

	RSEInstrument Gp5Reader::readRSEInstrument(StreamReader& reader) const
	{
		RSEInstrument instrument;
		instrument.instrument = reader.readSignedInt();
		instrument.unknown = reader.readUnsignedInt(); // TODO: ??? mostly 1
		instrument.soundBank = reader.readSignedInt();

		if (version().major() == 5 && version().minor() == 0) {
			instrument.effectNumber = reader.readSignedShort();
			reader.skip(1);
		} else {
			instrument.effectNumber = reader.readSignedInt();
		}

		return instrument;
	}

	RSEEqualizer Gp5Reader::readRSEEqualizer(uint8_t numKnobs, StreamReader& reader) const
	{
		RSEEqualizer equalizer;
		for (auto i = 0; i < numKnobs; ++i) {
			auto value = reader.readSignedByte();
			equalizer.knobs.push_back(-value / 10.0);
		}

		equalizer.gain = equalizer.knobs.back();
		return equalizer;
	}

	void Gp5Reader::readMeasures(Song& song, StreamReader& reader) const
	{
		auto tempo = song.tempo.value;
		auto start = Duration::QuarterTime;

		for (auto header : song.measureHeaders) {
			header.start = start;

			for (auto track : song.tracks) {
				Measure measure(track, header);
				measure.header = header;
				tempo = header.tempo;
				track.measures.push_back(measure);
				readMeasure(measure, reader);
			}

			header.tempo = tempo;
			start += header.calcLength();
		}
	}

	void Gp5Reader::readMeasure(Measure& measure, StreamReader& reader) const
	{
		auto start = measure.header.start;
		for (auto voice : measure.voices) {
			readVoice(start, voice, reader);
		}

		measure.lineBreak = static_cast<LineBreak>(reader.readUnsignedByte());
	}

	void Gp5Reader::readVoice(uint32_t start, Voice& voice, StreamReader& reader) const
	{
		auto numBeats = reader.readUnsignedInt();
		for (auto i = 0; i < numBeats; ++i) {
			auto duration = readBeat(start, voice, reader);
			start += duration;
		}
	}

	uint32_t Gp5Reader::readBeat(uint32_t start, Voice& voice, StreamReader& reader) const
	{
		auto flags = reader.readUnsignedByte();
		auto beat = getBeat(voice, start);

		beat.status = flags & 0x40
			? static_cast<BeatStatus>(reader.readUnsignedByte())
			: BeatStatus::Normal;

		auto duration = readDuration(flags, reader);

		if (flags & 0x02) {
			beat.effect.chord = readChord(voice.measure.track.strings.size(), reader);
		}
	}

	Beat& Gp5Reader::getBeat(Voice& voice, uint32_t start) const
	{
		for (auto it = voice.beats.rbegin(); it != voice.beats.rend(); ++it) {
			if (it->start == start) {
				return *it;
			}
		}

		Beat beat;
		beat.start = start;
		voice.beats.push_back(beat);
		return beat;
	}

	Duration Gp5Reader::readDuration(uint8_t flags, StreamReader& reader) const
	{
		Duration duration;
		duration.value = 1 << (reader.readSignedByte() + 2);
		duration.isDotted = static_cast<bool>(flags & 0x01);

		if (flags & 0x02) {
			auto tupletVal = reader.readUnsignedInt();
			if (tupletVal == 3) {
				duration.tuplet.enters = 3;
				duration.tuplet.times = 2;
			} else if (tupletVal == 5) {
				duration.tuplet.enters = 5;
				duration.tuplet.times = 4;
			} else if (tupletVal == 6) {
				duration.tuplet.enters = 6;
				duration.tuplet.times = 4;
			} else if (tupletVal == 7) {
				duration.tuplet.enters = 7;
				duration.tuplet.times = 4;
			} else if (tupletVal == 9) {
				duration.tuplet.enters = 9;
				duration.tuplet.times = 8;
			} else if (tupletVal == 10) {
				duration.tuplet.enters = 10;
				duration.tuplet.times = 8;
			} else if (tupletVal == 11) {
				duration.tuplet.enters = 11;
				duration.tuplet.times = 8;
			} else if (tupletVal == 12) {
				duration.tuplet.enters = 12;
				duration.tuplet.times = 8;
			}
		}

		return duration;
	}

	Chord Gp5Reader::readChord(size_t numStrings, StreamReader& reader) const
	{
		Chord chord;
		for (auto i = 0; i < numStrings; ++i) {
			chord.strings.push_back(-1);
		}

		chord.isNewFormat = reader.readBoolean();
		if (!chord.isNewFormat) {
			readOldChord(chord, reader);
		} else {
			readNewChord(chord, reader);
		}

		return chord;
	}

	void Gp5Reader::readOldChord(Chord& chord, StreamReader& reader) const
	{
		chord.name = reader.readIntByteSizedString();
		chord.firstFret = reader.readUnsignedInt();
		if (chord.firstFret) {
			for (auto i = 0; i < 6; ++i) {
				auto fret = reader.readSignedInt();
				if (i < chord.strings.size()) {
					chord.strings[i] = fret;
				}
			}
		}
	}

	void Gp5Reader::readNewChord(Chord& chord, StreamReader& reader) const
	{
		chord.isSharp = reader.readBoolean();
		auto intonation = chord.isSharp ? "sharp" : "flat";
		reader.skip(3);
		
	}
}