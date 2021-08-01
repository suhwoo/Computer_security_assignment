#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <vector>
using namespace std;
#define WIDTHBYTES(bytes) (((bytes)+3)/4*4)
int main(int argc, char *argv[])
{

	


	char c;
	scanf("%c", &c);
	
	if (c == 'e') {
		
		FILE *file; // file pointer
		BITMAPFILEHEADER hf; // 파일헤더 (bmp file header)
		BITMAPINFOHEADER hInfo; // 비트맵 정보헤더 (bitmap information header)
		int widthStep; // 라인 당 바이트 (bytes per a line)
		BYTE *lpImg; // 입력 데이터 포인터 (pointer for input image data)
		BYTE *lpOutImg; // 출력 데이터 포인터 (pointer for output image data)
		int x, y;

		// 입력 파일을 연다
		file = fopen("origin.bmp", "rb");
		if (file == NULL) {
			printf("이미지 파일을 열 수 없습니다! \n");
			return -1;
		}
		fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // 파일 헤더 읽음
		if (hf.bfType != 0x4D42) { // BMP 인지를 확인
			printf("BMP 파일이 아닙니다. \n");
			return -1;
		}
		fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, file); // 비트맵 정보헤더 읽음

		// 입력 데이터를 위한 라인당 바이트 수 계산
		widthStep = WIDTHBYTES((hInfo.biBitCount / 8) * hInfo.biWidth);
		fseek(file, hf.bfOffBits, SEEK_SET); // 비트맵 데이터가 시작하는 위치로 이동
		// 입력 데이터를 저장하기 위한 메모리 할당
		lpImg = (BYTE *)malloc(widthStep * hInfo.biHeight);
		// 입력에서 데이터를 읽음
		fread(lpImg, sizeof(BYTE), widthStep*hInfo.biHeight, file);
		fclose(file);
		// 결과 데이터를 저장하기 위한 메모리 할당
		lpOutImg = (BYTE *)malloc(widthStep * hInfo.biHeight);

		//string을 이진수로.
		vector<char> biry;
		char str[10000];
		memset(str, NULL, sizeof(str));
		scanf(" %[^\n]", str);
		char bt = 0x00;
		char c1, c2, c3, c4, c5, c6, c7, c8;
		char mask = 00000001;
		for (int i = 0; i < strlen(str); i++) {

			//이게 입력받은 문자를 이진수로 바꿔줌.
			bt = str[i];
			c1 = bt >> 7;
			c2 = bt >> 6;
			c3 = bt >> 5;
			c4 = bt >> 4;
			c5 = bt >> 3;
			c6 = bt >> 2;
			c7 = bt >> 1;
			c8 = bt;
			c1 = c1 & mask;
			c2 = c2 & mask;
			c3 = c3 & mask;
			c4 = c4 & mask;
			c5 = c5 & mask;
			c6 = c6 & mask;
			c7 = c7 & mask;
			c8 = c8 & mask;
			//일단 벡터에 저장한다. 나중에 8개씩 끊기.
			biry.push_back(c1);
			biry.push_back(c2);
			biry.push_back(c3);
			biry.push_back(c4);
			biry.push_back(c5);
			biry.push_back(c6);
			biry.push_back(c7);
			biry.push_back(c8);

		}


		long long bp = 0;
		// 암호화 연산
		if (hInfo.biBitCount == 24) {
			for (y = 0; y < hInfo.biHeight; y++) {
				for (x = 0; x < hInfo.biWidth; x++) {
					if (bp < biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 2] = (lpImg[y*widthStep + 3 * x + 2] & 0xFE) | biry[bp]; /* R */
						bp++;
					}
					else if (bp >= biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 2] = (lpImg[y*widthStep + 3 * x + 2] & 0xFE);//만약 string다 썼으면 000000로 채워서 string이 끝났음을 알리자
					}
					if (bp < biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 1] = (lpImg[y*widthStep + 3 * x + 1] & 0xFE) | biry[bp]; /* G */
						bp++;
					}
					else if (bp >= biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 1] = (lpImg[y*widthStep + 3 * x + 1] & 0xFE);
					}
					if (bp < biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 0] = (lpImg[y*widthStep + 3 * x + 0] & 0xFE) | biry[bp]; /* B */
						bp++;
					}
					else if (bp >= biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 0] = (lpImg[y*widthStep + 3 * x + 0] & 0xFE);
					}
				}
			}
		}

		file = fopen("stego.bmp", "wb");
		fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), file);
		fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), file);
		
		fseek(file, hf.bfOffBits, SEEK_SET); // 비트맵 데이터가 시작하는 위치로 이동
		fwrite(lpOutImg, sizeof(BYTE), widthStep*hInfo.biHeight, file);
		fclose(file);
		// 메모리 해제|
		free(lpOutImg);
		free(lpImg);
		
	}
	else if (c == 'd') {
	FILE *file; // file pointer
	BITMAPFILEHEADER hf; // 파일헤더 (bmp file header)
	BITMAPINFOHEADER hInfo; // 비트맵 정보헤더 (bitmap information header)
	RGBQUAD rgb[256]; // Lookup Table
	int widthStep; // 라인 당 바이트 (bytes per a line)
	BYTE *lpImg; // 입력 데이터 포인터 (pointer for input image data)
	//BYTE *lpOutImg; // 출력 데이터 포인터 (pointer for output image data)
	int x, y;

	// 입력 파일을 연다
	file = fopen("stego.bmp", "rb");
	if (file == NULL) {
		printf("이미지 파일을 열 수 없습니다! \n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // 파일 헤더 읽음
	if (hf.bfType != 0x4D42) { // BMP 인지를 확인
		printf("BMP 파일이 아닙니다. \n");
		return -1;
	}
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, file); // 비트맵 정보헤더 읽음


	// 입력 데이터를 위한 라인당 바이트 수 계산
	widthStep = WIDTHBYTES((hInfo.biBitCount / 8) * hInfo.biWidth);
	fseek(file, hf.bfOffBits, SEEK_SET); // 비트맵 데이터가 시작하는 위치로 이동
	// 입력 데이터를 저장하기 위한 메모리 할당
	lpImg = (BYTE *)malloc(widthStep * hInfo.biHeight);
	// 입력파일에서 데이터를 읽음
	fread(lpImg, sizeof(BYTE), widthStep*hInfo.biHeight, file);
	fclose(file);
	// 결과 데이터를 저장하기 위한 메모리 할당
	//lpOutImg = (BYTE *)malloc(widthStep * hInfo.biHeight);
	
	//long long bp = 0;
	// 복호화 연산
	
	char msg[10000];
	char Alpa[8];
	int alpaP = 0;
	int msgP = 0;
	int flag = 0;
	memset(msg, NULL, sizeof(msg));
	memset(Alpa, 0x00, sizeof(Alpa));
	if (hInfo.biBitCount == 24) {
		for (y = 0; y < hInfo.biHeight; y++) {
			for (x = 0; x < hInfo.biWidth; x++) {
				//하나씩 lsb꺼내기.
				Alpa[alpaP] = (lpImg[y*widthStep + 3 * x + 2] & 0x01); /* R */
				alpaP++;
				if (alpaP >= 8) {//이제 한글자 완성.

					char letter = (Alpa[0] << 7) | (Alpa[1] << 6) | (Alpa[2] << 5) | (Alpa[3] << 4) | (Alpa[4] << 3) | (Alpa[5] << 2) | (Alpa[6] << 1) | (Alpa[7]);
					if (letter == 0x00) { flag = 1; break; }
					alpaP = 0;
					memset(Alpa, 0x00, sizeof(Alpa));
					msg[msgP] = letter;
					msgP++;
				}

				Alpa[alpaP] = (lpImg[y*widthStep + 3 * x + 1] & 0x01); /* G */
				alpaP++;
				if (alpaP >= 8) {//이제 한글자 완성.

					char letter = (Alpa[0] << 7) | (Alpa[1] << 6) | (Alpa[2] << 5) | (Alpa[3] << 4) | (Alpa[4] << 3) | (Alpa[5] << 2) | (Alpa[6] << 1) | (Alpa[7]);
					if (letter == 0x00) { flag = 1; break; }
					alpaP = 0;
					memset(Alpa, 0x00, sizeof(Alpa));
					msg[msgP] = letter;
					msgP++;
				}

				Alpa[alpaP] = (lpImg[y*widthStep + 3 * x + 0] & 0x01); /* B */
				alpaP++;
				if (alpaP >= 8) {//이제 한글자 완성.

					char letter = (Alpa[0] << 7) | (Alpa[1] << 6) | (Alpa[2] << 5) | (Alpa[3] << 4) | (Alpa[4] << 3) | (Alpa[5] << 2) | (Alpa[6] << 1) | (Alpa[7]);
					if (letter == 0x00) { flag = 1; break; }
					alpaP = 0;
					memset(Alpa, 0x00, sizeof(Alpa));
					msg[msgP] = letter;
					msgP++;
				}

			}
			if (flag == 1) { break; }
		}
	}
	
	// 메모리 해제|
	//free(lpOutImg);
	printf("%s\n", msg);
	free(lpImg);
	


	}
return 0;
}