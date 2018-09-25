#include "Gp5Reader.h"
#include "StreamReader.h"
#include "Model.h"
#include <cstdint>
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
		auto directions = readDirections(reader);

		song->rseMasterEffects.reverb = reader.readUnsignedInt();

		auto numMeasures = reader.readUnsignedInt();
		auto numTracks = reader.readUnsignedInt();

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
		tempo.isHidden = (version().major() == 5 && version().minor() == 0)
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

	std::map<std::string, uint16_t> Gp5Reader::readDirections(StreamReader& reader) const
	{
		std::map<std::string, uint16_t> directions;

		directions["Coda"] = reader.readUnsignedShort();
		directions["Double Coda"] = reader.readUnsignedShort();
		directions["Segno"] = reader.readUnsignedShort();
		directions["Segno Segno"] = reader.readUnsignedShort();
		directions["Fine"] = reader.readUnsignedShort();

		directions["Da Capo"] = reader.readUnsignedShort();
		directions["Da Capo al Coda"] = reader.readUnsignedShort();
		directions["Da Capo al Double Coda"] = reader.readUnsignedShort();
		directions["Da Capo al Fine"] = reader.readUnsignedShort();
		directions["Da Segno"] = reader.readUnsignedShort();
		directions["Da Segno al Coda"] = reader.readUnsignedShort();
		directions["Da Segno al Double Coda"] = reader.readUnsignedShort();
		directions["Da Segno al Fine"] = reader.readUnsignedShort();
		directions["Da Segno Segno"] = reader.readUnsignedShort();
		directions["Da Segno Segno al Coda"] = reader.readUnsignedShort();
		directions["Da Segno Segno al Double Coda"] = reader.readUnsignedShort();
		directions["Da Segno Segno al Fine"] = reader.readUnsignedShort();
		directions["Da Coda"] = reader.readUnsignedShort();
		directions["Da Double Coda"] = reader.readUnsignedShort();

		return directions;
	}
}