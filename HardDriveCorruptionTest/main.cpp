#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

const size_t FiftyMB = 50 * 1000 * 1000;

// All inclusive
int64_t RandomNumber(int64_t min, int64_t max) {
	return ((int64_t)rand() % ((max + 1) - min)) + min; // rand() only generates 32-bit of random data, so it's pseudo-pseudo-random
}

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cerr << "Usage: HardDriveCorruptionTest.exe <SizeInBytes> [Verbose]" << std::endl;
		return 1;
	}

	bool verbose = argc >= 3; // Has 2 arguments

	try {
		size_t fileSize = std::stoull(argv[1]);
		int64_t seed = time(NULL);
		std::cout << "Checking with " << fileSize << " bytes..." << std::endl;

		{
			std::ofstream file("corruptiontest.tmp", std::ios_base::binary);
			if (!file.is_open()) {
				std::cerr << "File not writable!" << std::endl;
				return 1;
			}

			srand(seed); // Init random with seed
			size_t bytesWritten = 0;

			while (bytesWritten < fileSize) {
				size_t nextSize = RandomNumber(1, std::min(FiftyMB, fileSize - bytesWritten)); // Max 50 MB
				char* buffer = new char[nextSize]; // Init buffer with given size

				for (size_t i = 0; i < nextSize; i++) { // Fill buffer
					buffer[i] = (char)RandomNumber(0, CHAR_MAX);
				}
				file.write(buffer, nextSize);

				delete[] buffer;
				if (verbose) {
					std::cout << "Buffer with size of " << nextSize << " written" << std::endl;
				}
				bytesWritten += nextSize;
			}

			std::cout << "Done writing!" << std::endl;
			file.close();
		}

		{
			std::ifstream file("corruptiontest.tmp", std::ios_base::binary);
			if (!file.is_open()) {
				std::cerr << "File not readable!" << std::endl;
				return 1;
			}

			srand(seed); // Init random with seed
			size_t bytesRead = 0;

			while (fileSize > bytesRead) { // Super code quality right here:
				size_t nextSize = RandomNumber(1, std::min(FiftyMB, fileSize - bytesRead)); // Max 50 MB
				char* readBuffer = new char[nextSize];

				file.read(readBuffer, nextSize); // Fill read buffer
				for (size_t i = 0; i < nextSize; i++) {
					if (readBuffer[i] != (char)RandomNumber(0, CHAR_MAX)) { // Check if the bytes are equal to the random func.
						std::cerr << "WARNING! BYTES MISMATCH AT " << (bytesRead + i) << "! YOUR DRIVE IS CORRUPT." << std::endl;
						return 0;
					}
				}

				delete[] readBuffer;
				if (verbose) {
					std::cout << "Buffer with size of " << nextSize << " read" << std::endl;
				}
				bytesRead += nextSize;
			}

			std::cout << "Done! No problems found." << std::endl;
		}

		return 0;
	}
	catch (std::exception& ex) {
		std::cerr << "An exception occurred: " << ex.what() << std::endl;
		return 1;
	}
}
