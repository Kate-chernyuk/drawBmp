#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

class BMPReader {
private:
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	unsigned char* pixelData;
	ifstream fileStream;
public:
	bool openBMP(const string& fileName) {
		fileStream.open(fileName, ios::binary);
		if (!fileStream.is_open()) {
			cout << "Не удалось открыть файл\n";
			return false;
		}

		fileStream.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));
		if (fileHeader.bfType != 0x4D42) {
			cout << "Неверный формат файла (не BMP)\n";
			fileStream.close();
			return false;
		}

		fileStream.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));

		if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
			cout << "Неверный формат файла (допустимые 24 или 32 бита)\n";
			fileStream.close();
			return false;
		}

		pixelData = new unsigned char[infoHeader.biSizeImage];
		fileStream.seekg(fileHeader.bfOffBits, ios::beg);
		fileStream.read((char*)pixelData, infoHeader.biSizeImage);

		return true;
	}

	void displayBMP() {
		for (int y = infoHeader.biHeight - 1; y >= 0; y--) {  
			for (int x = 0; x < infoHeader.biWidth; x++) {
				int index = (y * infoHeader.biWidth + x) * (infoHeader.biBitCount / 8);
				if (pixelData[index] == 0 && pixelData[index + 1] == 0 && pixelData[index + 2] == 0) {
					cout << "0";
				}
				else if (pixelData[index] == 255 && pixelData[index + 1] == 255 && pixelData[index + 2] == 255) {
					cout << "1";
				}
				else {
					cout << "Неверный цвет пикселя\n";
					return;
				}
			}
			cout << "\n";
		}
	}

	void closeBMP() {
		if (pixelData != nullptr) {
			delete[] pixelData;
			pixelData = nullptr;
		}
		fileStream.close();
	}

	~BMPReader() {
		closeBMP();
	}
};

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	if (argc != 2) {
		cout << "Необходимо передать BMP-файл\n";
		return 0;
	}

	BMPReader bmp;
	if (bmp.openBMP(argv[1])) {
		bmp.displayBMP();
	}

	return 0;
}
