#pragma once

#include "GpReaderBase.h"
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <optional>
#include <tuple>

namespace libgp
{ 
	struct Lyrics;
	struct PageSetup;
	struct Tempo;
	struct MidiChannel;
	struct MeasureHeader;
	struct Marker;
	struct Color;

	class Gp5Reader : private GpReaderBase
	{
	public:
		Gp5Reader();

		std::unique_ptr<Song> readSong(std::istream& stream) override;

	private:
		using DirectionSigns = std::map<std::string, int16_t>;

		void readSongInfo(Song& song, StreamReader& reader) const;
		Lyrics readLyrics(StreamReader& reader) const;
		PageSetup readPageSetup(StreamReader& reader) const;
		Tempo readTempo(StreamReader& reader) const;
		std::vector<MidiChannel> readMidiChannels(StreamReader& reader) const;
		std::tuple<DirectionSigns, DirectionSigns> readDirectionSigns(StreamReader& reader) const;
		void readMeasureHeaders(uint32_t numMeasures, Song& song, std::tuple<DirectionSigns, DirectionSigns> directionSigns, StreamReader& reader) const;
		MeasureHeader readMeasureHeader(uint32_t number, Song& song, std::optional<MeasureHeader> previous, StreamReader& reader) const;
		Marker readMarker(StreamReader& reader) const;
		Color readColor(StreamReader& reader) const;
	};
}