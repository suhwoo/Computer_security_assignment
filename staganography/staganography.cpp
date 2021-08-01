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
		BITMAPFILEHEADER hf; // ������� (bmp file header)
		BITMAPINFOHEADER hInfo; // ��Ʈ�� ������� (bitmap information header)
		int widthStep; // ���� �� ����Ʈ (bytes per a line)
		BYTE *lpImg; // �Է� ������ ������ (pointer for input image data)
		BYTE *lpOutImg; // ��� ������ ������ (pointer for output image data)
		int x, y;

		// �Է� ������ ����
		file = fopen("origin.bmp", "rb");
		if (file == NULL) {
			printf("�̹��� ������ �� �� �����ϴ�! \n");
			return -1;
		}
		fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // ���� ��� ����
		if (hf.bfType != 0x4D42) { // BMP ������ Ȯ��
			printf("BMP ������ �ƴմϴ�. \n");
			return -1;
		}
		fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, file); // ��Ʈ�� ������� ����

		// �Է� �����͸� ���� ���δ� ����Ʈ �� ���
		widthStep = WIDTHBYTES((hInfo.biBitCount / 8) * hInfo.biWidth);
		fseek(file, hf.bfOffBits, SEEK_SET); // ��Ʈ�� �����Ͱ� �����ϴ� ��ġ�� �̵�
		// �Է� �����͸� �����ϱ� ���� �޸� �Ҵ�
		lpImg = (BYTE *)malloc(widthStep * hInfo.biHeight);
		// �Է¿��� �����͸� ����
		fread(lpImg, sizeof(BYTE), widthStep*hInfo.biHeight, file);
		fclose(file);
		// ��� �����͸� �����ϱ� ���� �޸� �Ҵ�
		lpOutImg = (BYTE *)malloc(widthStep * hInfo.biHeight);

		//string�� ��������.
		vector<char> biry;
		char str[10000];
		memset(str, NULL, sizeof(str));
		scanf(" %[^\n]", str);
		char bt = 0x00;
		char c1, c2, c3, c4, c5, c6, c7, c8;
		char mask = 00000001;
		for (int i = 0; i < strlen(str); i++) {

			//�̰� �Է¹��� ���ڸ� �������� �ٲ���.
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
			//�ϴ� ���Ϳ� �����Ѵ�. ���߿� 8���� ����.
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
		// ��ȣȭ ����
		if (hInfo.biBitCount == 24) {
			for (y = 0; y < hInfo.biHeight; y++) {
				for (x = 0; x < hInfo.biWidth; x++) {
					if (bp < biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 2] = (lpImg[y*widthStep + 3 * x + 2] & 0xFE) | biry[bp]; /* R */
						bp++;
					}
					else if (bp >= biry.size()) {
						lpOutImg[y*widthStep + 3 * x + 2] = (lpImg[y*widthStep + 3 * x + 2] & 0xFE);//���� string�� ������ 000000�� ä���� string�� �������� �˸���
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
		
		fseek(file, hf.bfOffBits, SEEK_SET); // ��Ʈ�� �����Ͱ� �����ϴ� ��ġ�� �̵�
		fwrite(lpOutImg, sizeof(BYTE), widthStep*hInfo.biHeight, file);
		fclose(file);
		// �޸� ����|
		free(lpOutImg);
		free(lpImg);
		
	}
	else if (c == 'd') {
	FILE *file; // file pointer
	BITMAPFILEHEADER hf; // ������� (bmp file header)
	BITMAPINFOHEADER hInfo; // ��Ʈ�� ������� (bitmap information header)
	RGBQUAD rgb[256]; // Lookup Table
	int widthStep; // ���� �� ����Ʈ (bytes per a line)
	BYTE *lpImg; // �Է� ������ ������ (pointer for input image data)
	//BYTE *lpOutImg; // ��� ������ ������ (pointer for output image data)
	int x, y;

	// �Է� ������ ����
	file = fopen("stego.bmp", "rb");
	if (file == NULL) {
		printf("�̹��� ������ �� �� �����ϴ�! \n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, file); // ���� ��� ����
	if (hf.bfType != 0x4D42) { // BMP ������ Ȯ��
		printf("BMP ������ �ƴմϴ�. \n");
		return -1;
	}
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, file); // ��Ʈ�� ������� ����


	// �Է� �����͸� ���� ���δ� ����Ʈ �� ���
	widthStep = WIDTHBYTES((hInfo.biBitCount / 8) * hInfo.biWidth);
	fseek(file, hf.bfOffBits, SEEK_SET); // ��Ʈ�� �����Ͱ� �����ϴ� ��ġ�� �̵�
	// �Է� �����͸� �����ϱ� ���� �޸� �Ҵ�
	lpImg = (BYTE *)malloc(widthStep * hInfo.biHeight);
	// �Է����Ͽ��� �����͸� ����
	fread(lpImg, sizeof(BYTE), widthStep*hInfo.biHeight, file);
	fclose(file);
	// ��� �����͸� �����ϱ� ���� �޸� �Ҵ�
	//lpOutImg = (BYTE *)malloc(widthStep * hInfo.biHeight);
	
	//long long bp = 0;
	// ��ȣȭ ����
	
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
				//�ϳ��� lsb������.
				Alpa[alpaP] = (lpImg[y*widthStep + 3 * x + 2] & 0x01); /* R */
				alpaP++;
				if (alpaP >= 8) {//���� �ѱ��� �ϼ�.

					char letter = (Alpa[0] << 7) | (Alpa[1] << 6) | (Alpa[2] << 5) | (Alpa[3] << 4) | (Alpa[4] << 3) | (Alpa[5] << 2) | (Alpa[6] << 1) | (Alpa[7]);
					if (letter == 0x00) { flag = 1; break; }
					alpaP = 0;
					memset(Alpa, 0x00, sizeof(Alpa));
					msg[msgP] = letter;
					msgP++;
				}

				Alpa[alpaP] = (lpImg[y*widthStep + 3 * x + 1] & 0x01); /* G */
				alpaP++;
				if (alpaP >= 8) {//���� �ѱ��� �ϼ�.

					char letter = (Alpa[0] << 7) | (Alpa[1] << 6) | (Alpa[2] << 5) | (Alpa[3] << 4) | (Alpa[4] << 3) | (Alpa[5] << 2) | (Alpa[6] << 1) | (Alpa[7]);
					if (letter == 0x00) { flag = 1; break; }
					alpaP = 0;
					memset(Alpa, 0x00, sizeof(Alpa));
					msg[msgP] = letter;
					msgP++;
				}

				Alpa[alpaP] = (lpImg[y*widthStep + 3 * x + 0] & 0x01); /* B */
				alpaP++;
				if (alpaP >= 8) {//���� �ѱ��� �ϼ�.

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
	
	// �޸� ����|
	//free(lpOutImg);
	printf("%s\n", msg);
	free(lpImg);
	


	}
return 0;
}