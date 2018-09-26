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

		song->rseMasterEffects.reverb = reader.readUnsignedInt();

		auto numMeasures = reader.readUnsignedInt();
		auto numTracks = reader.readUnsignedInt();
		readMeasureHeaders(numMeasures, *song, directionSigns, reader);

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
			header.timeSignature.denominator = reader.readSignedByte();
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
}