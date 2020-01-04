///blackdragonx61 2020
#include <iostream>
#include "StringBuilder.h"

const auto Make(const std::string& path)
{
	static int stage;
	stage++;
	StringBuilder _Build(path);
	!_Build.Start() ? std::printf("Stage: %d Error: %s, Path: %s\n", stage, _Build.What().c_str(), _Build.GetLocaleName().c_str()) : std::printf("Stage: %d Completed. Path: %s\n", stage, _Build.GetLocaleName().c_str());
}

int main() {
	std::cout << "Please write locale name:";
	std::string locale;
	std::getline(std::cin, locale);

	if (locale == "all")
		for (const std::string& path : { "tr", "en", "de", "ae", "cz", "dk", "es", "fr", "gr", "hu", "it", "nl", "pl", "pt", "ro", "ru" })
			Make(path);
	else {
		const std::string space(" ");
		size_t pos{0};
		while ((pos = locale.find(space)) != std::string::npos) {
			Make(locale.substr(0, pos));
			locale.erase(0, pos + space.length());
		}
		Make(locale);
	}

	std::printf("\nCoded by blackdragonx61\n");
	system("pause");
	return 0;
}