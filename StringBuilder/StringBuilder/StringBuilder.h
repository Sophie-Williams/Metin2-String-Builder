#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <array>
constexpr auto source_path = "resource/source_locale_string.txt";
enum
{
	LOCALE_PATH,
	OUTPUT_PATH,
	PATH_MAX
};

class StringBuilder {
public:
	StringBuilder(const std::string& locale) {
		_path[LOCALE_PATH] = "resource/" + locale + "/locale_string.txt";
		_path[OUTPUT_PATH] = "locale_string_" + locale + ".txt";
	}
	~StringBuilder() = default;

	const auto& GetLocaleName() const { return _path[LOCALE_PATH]; }
	const auto& What() const { return what; }

	const auto ReadLocalePath() const
	{
		std::unordered_map<std::string, std::string> localemap;
		std::string line;
		std::ifstream myfile(_path.at(LOCALE_PATH).c_str());
		if (myfile.is_open()) {
			while (std::getline(myfile, line)) {
				const auto& pos = line.find("\t");
				if (pos != std::string::npos)
					localemap.emplace(line.substr(0, pos), line.substr(pos + 1));
			}
			myfile.close();
		}
		return localemap;
	}
	const auto ReadSourcePath() const
	{
		std::vector<std::string> sourcevec;
		std::string line;
		std::ifstream myfile(source_path);
		if (myfile.is_open()) {
			while (std::getline(myfile, line))
				if (line.size())
					sourcevec.emplace_back(std::move(line));
			myfile.close();
		}
		return sourcevec;
	}
	const auto Start()
	{
		const auto& localepath(ReadLocalePath());
		if (localepath.empty())
			what = "Cannot read locale path";

		const auto& sourcepath(ReadSourcePath());
		if (sourcepath.empty())
			what = "Cannot read source path";

		if (what.size())
			return false;

		std::unique_ptr<FILE, decltype(&std::fclose)> File(std::fopen(_path.at(OUTPUT_PATH).c_str(), "w"), &std::fclose);
		if (!File) {
			what = "Cannot open file for writing.";
			return false;
		}

		size_t sz(localepath.size());
		for (const auto& txt : localepath) {
			const auto& tex = "\"" + txt.first + "\";";
			const auto& iter = std::find(sourcepath.begin(), sourcepath.end(), tex);
			if (iter != sourcepath.end()) {
				std::fprintf(File.get(), "%s\n", sourcepath.at(std::distance(sourcepath.begin(), iter - 1)).c_str());
				std::fprintf(File.get(), "%s",std::string('"' + txt.second + '"' + ";").c_str());
				if (sz > 1)
					std::fprintf(File.get(), "\n\n");
			}
			sz--;
		}
		return true;
	}

private:
	std::array<std::string, PATH_MAX> _path;
	std::string what = "";
};