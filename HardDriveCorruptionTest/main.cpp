#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cerr << "Usage: HardDriveCorruptionTest.exe <SizeInBytes>" << std::endl;
		return 1;
	}

	try {
		size_t fileSize = std::stoull(argv[1]);
		std::cout << "Checking with " << fileSize << " bytes..." << std::endl;

		return 0;
	}
	catch (std::exception& ex) {
		std::cerr << "An exception occurred: " << ex.what() << std::endl;
		return 1;
	}
}
