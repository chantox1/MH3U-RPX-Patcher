#include "MH3U_WiiU_Patcher.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <boost/json.hpp>
#include <boost/program_options.hpp>
#include "util.h"

using namespace std;
namespace fs = filesystem;
using namespace boost;
namespace po = program_options;

int decompressRPX(fs::path path) {
	try {
		fs::copy_file(path, "temp.rpx",
			fs::copy_options::overwrite_existing);
		std::system("wiiurpxtool -d temp.rpx code.bin");
		fs::remove("temp.rpx");
	}
	catch (const fs::filesystem_error &fe) {
		cerr << fe.what() << endl;
		return fe.code().value();
	}
	return 0;
}

int recompressRPX(fs::path path) {
	try {
		std::system("wiiurpxtool -c code.bin temp.rpx");
		fs::rename("temp.rpx", path);
		fs::remove("code.bin");
	}
	catch (const fs::filesystem_error &fe) {
		cerr << fe.what() << endl;
		return fe.code().value();
	}
	return 0;
}

Payload readPayload(json::object entry) {
	json::string pld = entry.at("payload").get_string();
	pld.erase(remove_if(pld.begin(), pld.end(), ::isspace), pld.end());
	size_t len = pld.size() / 2;
	char *hexData = (char *)malloc(len);
	decodeHexStr(pld.data(), len, hexData);
	return Payload(hexData, len);
}

vector<int> readAddresses(json::object entry) {
	vector<int> addresses;
	json::array addStrList = entry.at("addresses").get_array();
	for (json::value y : addStrList) {
		json::string addStr = y.get_string();
		addresses.push_back(stoi(string(addStr), nullptr, 0));
	}
	return addresses;
}

int64_t readCount(json::object entry) {
	try {
		return entry.at("count").as_int64();
	}
	catch (...) {
		return 1;
	}
}

int main(int argc, char *argv[]) {
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Display this message")
		("rpx", po::value<string>(),
			"Set executable path (first positional argument)")
		("noprompt", "Install every patch without asking");
	po::positional_options_description pd;
	pd.add("rpx", 1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).
		options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return EXIT_SUCCESS;
	}

	if (!vm.count("rpx")) {
		cout << "Please drag the .rpx executable onto the patcher, "
			 << "or provide the path manually." << endl;
		return EXIT_SUCCESS;
	}
	fs::path rpxPath(vm["rpx"].as<string>());
	fs::path patchDir("patches");
	if (!existsAndIsFile(rpxPath) || !existsAndIsDir(patchDir)) {
		return EXIT_FAILURE;
	}

	/*
	int error = decompressRPX(rpxPath);
	if (error) {
		return error;
	}
	*/
	FILE *code = NULL;  // We'll worry about this when we need it

	fs::recursive_directory_iterator it(patchDir), end;
	while (it != end) {
		if (it->path().extension() == ".json") {
			json::error_code ec;
			string data = slurp(it->path().string());
			json::value patch = json::parse(data, ec);
			if (patch == NULL) {
				cerr << ec.what() << endl;
				cout << "skipping patch..." << endl;
			}
			else {
				cout << patch.at("title").get_string() << endl;
				try {
					cout << "- " << patch.at("desc").get_string() << endl;
				}
				catch (out_of_range) {
					cout << "- " << "No description." << endl;
				}

				string answer;

				bool yes = (vm.count("noprompt")) ? true : false;
				bool no = false;
				while (!yes && !no) {
					cout << "Apply patch? (Y/N)" << endl;
					getline(cin, answer);
					yes = (answer == "Y");
					no = (answer == "N");
				}
				if (yes) {
					cout << "applying patch..." << endl;
					json::array patchData = patch.at("patches").get_array();
					for (json::value x : patchData) {
						json::object entry = x.get_object();

						Payload pld = readPayload(entry);
						vector<int> adds = readAddresses(entry);
						uint64_t count = readCount(entry);

						if (code == NULL) {
							int error = decompressRPX(rpxPath);
							if (error) {
								return error;
							}
							code = fopen("code.bin", "rb+");
						}
						for (int add : adds) {
							for (int n = 0; n < count; n++) {
								fseek(code, add + n * pld.len, SEEK_SET);
								fwrite(pld.data, pld.len, 1, code);
							}
						}
					}
				}
			}
		}
		it++;
	}
	if (code == NULL) {
		cout << "No patches were applied." << endl;
		/*
		try {
			fs::remove("code.bin");
		}
		catch (const fs::filesystem_error &fe) {
			cerr << fe.what() << endl;
			return fe.code().value();
		}
		*/
		return EXIT_SUCCESS;
	}
	fclose(code);

	// Rename original to serve as backup
	fs::path bkpPath = rpxPath;
	bkpPath += ".bkp";
	fs::rename(rpxPath, bkpPath);

	// Recompress & replace original at path
	int error = recompressRPX(rpxPath);
	if (error) {
		return error;
	}
	return EXIT_SUCCESS;
}
