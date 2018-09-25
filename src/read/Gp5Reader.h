#pragma once

#include "GpReaderBase.h"
#include <vector>
#include <map>
#include <string>
#include <cstdint>

namespace libgp
{ 
	struct Lyrics;
	struct PageSetup;
	struct Tempo;
	struct MidiChannel;

	class Gp5Reader : private GpReaderBase
	{
	public:
		Gp5Reader();

		std::unique_ptr<Song> readSong(std::istream& stream) override;

	private:
		void readSongInfo(Song& song, StreamReader& reader) const;
		Lyrics readLyrics(StreamReader& reader) const;
		PageSetup readPageSetup(StreamReader& reader) const;
		Tempo readTempo(StreamReader& reader) const;
		std::vector<MidiChannel> readMidiChannels(StreamReader& reader) const;
		std::map<std::string, uint16_t> readDirections(StreamReader& reader) const;
	};
}