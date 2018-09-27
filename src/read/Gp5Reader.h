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
	struct Track;
	struct TrackRSE;
	struct RSEInstrument;
	struct RSEEqualizer;
	struct Measure;
	struct Voice;
	struct Beat;
	struct Duration;
	struct Chord;

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
		void readTracks(uint32_t numTracks, Song& song, std::vector<MidiChannel> midiChannels, StreamReader& reader) const;
		Track readTrack(uint32_t number, std::vector<MidiChannel> midiChannels, StreamReader& reader) const;
		std::optional<MidiChannel> readMidiChannel(std::vector<MidiChannel> midiChannels, StreamReader& reader) const;
		void readTrackRSE(TrackRSE& rse, StreamReader& reader) const;
		RSEInstrument readRSEInstrument(StreamReader& reader) const;
		RSEEqualizer readRSEEqualizer(uint8_t numKnobs, StreamReader& reader) const;
		void readMeasures(Song& song, StreamReader& reader) const;
		void readMeasure(Measure& measure, StreamReader& reader) const;
		void readVoice(uint32_t start, Voice& voice, StreamReader& reader) const;
		uint32_t readBeat(uint32_t start, Voice& voice, StreamReader& reader) const;
		Beat& getBeat(Voice& voice, uint32_t start) const;
		Duration readDuration(uint8_t flags, StreamReader& reader) const;
		Chord readChord(size_t numStrings, StreamReader& reader) const;
		void readOldChord(Chord& chord, StreamReader& reader) const;
		void readNewChord(Chord& chord, StreamReader& reader) const;
	};
}