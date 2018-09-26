#include "catch2/catch.hpp"
#include "../src/read/Gp5Reader.h"
#include "../src/Model.h"
#include "../src/GpVersion.h"
#include <fstream>
#include <memory>

using namespace libgp;

std::unique_ptr<Song> createExpectedSong();

SCENARIO("Can parse a Guitar Pro version string")
{
	GIVEN("A valid version string")
	{
		std::string versionStr = "FICHIER GUITAR PRO v5.00";

		WHEN("The string is parsed")
		{
			auto version = GpVersion::parse(versionStr);

			THEN("The version info is correct")
			{
				REQUIRE(version.full() == "FICHIER GUITAR PRO v5.00");
				REQUIRE(version.major() == 5);
				REQUIRE(version.minor() == 0);
			}
		}
	}

	GIVEN("An invalid verison string")
	{
		std::string versionStr = "FICHIER GUITAR PRO va.00";

		WHEN("The string is parsed")
		{
			THEN("An exception is thrown")
			{
				REQUIRE_THROWS(GpVersion::parse(versionStr));
			}
		}
	}
}

SCENARIO("Can read a Guitar Pro 5 file")
{
	GIVEN("A .gp5 file")
	{
		std::ifstream fileStream;
		fileStream.open("./resources/test.gp5", std::ios::binary);

		WHEN("The file is read")
		{
			Gp5Reader reader;
			auto song = reader.readSong(fileStream);
			auto expectedSong = createExpectedSong();

			THEN("The song info is correct")
			{
				REQUIRE(song->title == expectedSong->title);
				REQUIRE(song->subtitle == expectedSong->subtitle);
				REQUIRE(song->artist == expectedSong->artist);
				REQUIRE(song->album == expectedSong->album);
				REQUIRE(song->lyricsWriter == expectedSong->lyricsWriter);
				REQUIRE(song->musicWriter == expectedSong->musicWriter);
				REQUIRE(song->copyright == expectedSong->copyright);
				REQUIRE(song->tabAuthor == expectedSong->tabAuthor);
				REQUIRE(song->instructions == expectedSong->instructions);
				REQUIRE(song->comments == expectedSong->comments);
			}

			THEN("The lyrics are correct")
			{
				REQUIRE(song->lyrics == expectedSong->lyrics);
			}

			THEN("The page setup info is correct")
			{
				REQUIRE(song->pageSetup == expectedSong->pageSetup);
			}

			THEN("The tempo is correct")
			{
				REQUIRE(song->tempo == expectedSong->tempo);
			}

			THEN("The key signature is correct")
			{
				REQUIRE(song->keySignature == expectedSong->keySignature);
			}

			THEN("The octave is correct")
			{
				REQUIRE(song->octave == expectedSong->octave);
			}

			THEN("The measure headers are correct")
			{
				REQUIRE(song->measureHeaders == expectedSong->measureHeaders);
			}
		}
	}
}

std::unique_ptr<Song> createExpectedSong()
{
	auto song = std::make_unique<Song>();

	song->title = "title";
	song->subtitle = "subtitle";
	song->artist = "artist";
	song->album = "album";
	song->lyricsWriter = "lyrics";
	song->musicWriter = "music";
	song->copyright = "copyright";
	song->tabAuthor = "tab";
	song->instructions = "instructions";

	song->lyrics.trackNumber = 1;
	song->lyrics.lines = {
		LyricLine(1, "these are the lyrics "),
		LyricLine(1, ""),
		LyricLine(1, ""),
		LyricLine(1, ""),
		LyricLine(1, "")
	};

	song->tempo.name = "";
	song->tempo.value = 120;
	song->tempo.isHidden = false;

	song->keySignature = KeySignature::CMajor;
	song->octave = 0;

	MeasureHeader header;
	header.number = 1;
	header.start = 0;
	header.tempo = song->tempo.value;
	header.timeSignature.numerator = 4;
	header.timeSignature.denominator = 4;
	header.keySignature = KeySignature::GMajor;
	song->measureHeaders.push_back(header);

	return song;
}