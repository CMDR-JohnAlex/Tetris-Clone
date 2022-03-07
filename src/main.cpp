// JOHN ALEXANDER LE ROUX
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include <Windows.h>

std::wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

int nScreenWidth = 120;			// Console Screen Size X (columns)
int nScreenHeight = 30;			// Console Screen Size Y (rows)

int rotate(int px, int py, int r) // Function for rotating the pieces
{
	int pieceIndex = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees					//  0  1  2  3
		pieceIndex = py * 4 + px;			//  4  5  6  7
		break;								//  8  9 10 11
											// 12 13 14 15

	case 1: // 90 degrees					// 12  8  4  0
		pieceIndex = 12 + py - (px * 4);	// 13  9  5  1
		break;								// 14 10  6  2
											// 15 11  7  3

	case 2: // 180 degrees					// 15 14 13 12
		pieceIndex = 15 - (py * 4) - px;	// 11 10  9  8
		break;								//  7  6  5  4
											//  3  2  1  0

	case 3: // 270 degrees					//  3  7 11 15
		pieceIndex = 3 - py + (px * 4);		//  2  6 10 14
		break;								//  1  5  9 13
											//  0  4  8 12
	}

	return pieceIndex;
}

bool doesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pieceIndex = rotate(px, py, nRotation);

			// Get index into field
			int fieldIndex = (nPosY + py) * nFieldWidth + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					// In Bounds so do collision check
					if (tetromino[nTetromino][pieceIndex] != L'.' && pField[fieldIndex] != 0)
						return false; // Fail on first hit
				}
			}

		}

	return true;
}

int main(int argc, char* argv[])
{
	std::cout << "==========================================\n";
	std::cout << " _______   _        _     \n";
	std::cout << "|__   __| | |      (_)    \n";
	std::cout << "   | | ___| |_ _ __ _ ___ \n";
	std::cout << "   | |/ _ \\ __| '__| / __|\n";
	std::cout << "   | |  __/ |_| |  | \\__ \\\n";
	std::cout << "   |_|\\___|\\__|_|  |_|___/ Console Clone\n";
	std::cout << "==========================================\n\n";
	std::cout << "            Ready to play?\n\n";
	system("pause");
	system("CLS");

	// Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Create assets
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");

	pField = new unsigned char[nFieldWidth * nFieldHeight]; // Create play field buffer
	for (int x = 0; x < nFieldWidth; x++) // Board Boundary
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	// Game logic
	bool bKey[4];
	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;
	int nSpeed = 20;
	int nSpeedCount = 0;
	bool bForceDown = false;
	bool bRotateHold = true;
	int nPieceCount = 0;
	int nScore = 0;
	std::vector<int> vLines;
	bool bGameOver = false;

	while (!bGameOver) // Main game loop
	{
		// TIMING =================================================================================================================================
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		nSpeedCount++;
		bForceDown = (nSpeedCount == nSpeed);

		// INPUT ==================================================================================================================================
		for (int k = 0; k < 4; k++)								//  R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// GAME LOGIC =============================================================================================================================
		// Handle player movement
		nCurrentX += (bKey[0] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentY += (bKey[2] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		// Rotate, but latch to stop wild spinning
		if (bKey[3])
		{
			nCurrentRotation += (bRotateHold && doesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
		}
		else
			bRotateHold = true;
		
		/*
		// Old handle player movement
		if (bKey[0])
		{
			if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
			{
				nCurrentX = nCurrentX + 1;
			}
		}

		if (bKey[1])
		{
			if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
			{
				nCurrentX = nCurrentX - 1;
			}
		}

		if (bKey[2])
		{
			if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			{
				nCurrentY = nCurrentY + 1;
			}
		}

		nCurrentRotation += (bKey[3] && doesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
		*/

		// Force the piece down the playfield if it's time
		if (bForceDown)
		{
			// Update difficulty every 50 pieces
			nSpeedCount = 0;
			nPieceCount++;
			if (nPieceCount % 50 == 0)
				if (nSpeed >= 10) nSpeed--;

			// Test if piece can be moved down
			if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; // It can, so do it!
			else
			{
				// It can't! Lock the piece in place
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][rotate(px, py, nCurrentRotation)] != L'.')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				nPieceCount++;
				if (nPieceCount % 10 == 0)
					if (nSpeed >= 10) nSpeed--;

				// Check for lines
				for (int py = 0; py < 4; py++)
					if (nCurrentY + py < nFieldHeight - 1)
					{
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++)
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

						if (bLine)
						{
							// Remove Line, set to =
							for (int px = 1; px < nFieldWidth - 1; px++)
								pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							vLines.push_back(nCurrentY + py);
						}
					}

				nScore += 25;
				if (!vLines.empty()) nScore += (1 << vLines.size()) * 100;

				// Choose next piece
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				// nCurrentPiece = rand() % 7; // Old random code

				// Get Random Number
				std::random_device dev; // for seeding
				std::default_random_engine gen{ dev() };
				std::uniform_int_distribution<int> dis{ 0, 6 };
				nCurrentPiece = dis(gen);

				// If piece does not fit straight away, game over!
				bGameOver = !doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
			nSpeedCount = 0;
		}

		// RENDER OUTPUT ==========================================================================================================================

		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ■■■■■■■=#"[pField[y * nFieldWidth + x]]; // ■■■■■■■=# ABCDEFG=# ███████=#

		// Draw Current Piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetromino[nCurrentPiece][rotate(px, py, nCurrentRotation)] != L'.')
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65; // `█` = 219  `■` = 254 `ABCDEFG` = nCurrentPiece + 65

		// Draw Score
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

		// Draw Credits
		swprintf_s(&screen[6  * nScreenWidth + nFieldWidth + 6], 14, L"Game made by:");
		swprintf_s(&screen[7  * nScreenWidth + nFieldWidth + 6], 16, L" @CMDR-JohnAlex");
		swprintf_s(&screen[9  * nScreenWidth + nFieldWidth + 6], 13, L"Tutorial by:");
		swprintf_s(&screen[10 * nScreenWidth + nFieldWidth + 6],  9, L" Javidx9");
		swprintf_s(&screen[12 * nScreenWidth + nFieldWidth + 6], 38, L"For source code and more information:");
		swprintf_s(&screen[13 * nScreenWidth + nFieldWidth + 6], 32, L"Github.com/CMDR-JohnAlex/Tetris");

		// Animate Line Completion
		if (!vLines.empty())
		{
			// Display Frame (cheekily to draw lines)
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Delay a bit

			for (auto& v : vLines)
				for (int px = 1; px < nFieldWidth - 1; px++)
				{
					for (int py = v; py > 0; py--)
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					pField[px] = 0;
				}

			vLines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	// Oh Dear
	CloseHandle(hConsole);
	std::cout << "==========================================\n";
	std::cout << " _______   _        _     \n";
	std::cout << "|__   __| | |      (_)    \n";
	std::cout << "   | | ___| |_ _ __ _ ___ \n";
	std::cout << "   | |/ _ \\ __| '__| / __|\n";
	std::cout << "   | |  __/ |_| |  | \\__ \\\n";
	std::cout << "   |_|\\___|\\__|_|  |_|___/ Console Clone\n";
	std::cout << "==========================================\n\n";
	std::cout << "              GAME  OVER!\n";
	std::cout << "              Score: " << nScore << '\n' << '\n';
	std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Delay a bit
	system("pause");
	return 0;
}
