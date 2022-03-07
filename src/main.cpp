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

int rotate(int px, int py, int r)
{
	switch (r % 4)
	{
	case 0: return py * 4 + px; // 0 degrees
	case 1: return 12 + py - (px * 4); // 90 degrees
	case 2: return 15 - (py * 4) - px; // 180 degrees
	case 3: return 3 - py + (px * 4); // 270 degrees
	}
	return 0;
}

bool doesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pieceIndex = rotate(px, py, nRotation);

			// Get index into field
			int fieldIndex = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					// Collision detection
					if (tetromino[nTetromino][pieceIndex] == L'X' && pField[fieldIndex] != 0)
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
	std::cout << "       Ready to play?\n\n";
	system("pause");
	system("CLS");

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

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Game logic variables
	bool bGameOver = false;
	
	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bKey[4];
	bool bRotateHold = false;

	int nSpeed = 20;
	int nSpeedCount = 0;
	bool bForceDown = false;
	int nPieceCount = 0;
	int nScore = 0;

	std::vector<int> vLines;

	while (!bGameOver) // Game loop
	{
		// GAME TIMING ============================================================================================================================
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		//std::this_thread::sleep_for(50ms);
		nSpeedCount++;
		bForceDown = (nSpeedCount == nSpeed);

		// INPUT ==================================================================================================================================
		for (int k = 0; k < 4; k++)								// R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// GAME LOGIC =============================================================================================================================
		// Handle player movement
		nCurrentX += (bKey[0] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentY += (bKey[2] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;
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

		// Test if piece can be moved down
		if (bForceDown)
		{
			if (doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; // It can, so do it!
			else
			{
				// It can't! So lock the current piece in the field
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][rotate(px, py, nCurrentRotation)] != L'.')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				nPieceCount++;
				if (nPieceCount % 10 == 0)
					if (nSpeed >= 10) nSpeed--;

				// Check have we got any lines
				for (int py = 0; py < 4; py ++)
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
				// nCurrentPiece = rand() % 7;

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
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ■■■■■■■=#"[pField[y * nFieldWidth + x]]; // ■■■■■■■=# ABCDEFG=#

		// Draw Current Piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetromino[nCurrentPiece][rotate(px, py, nCurrentRotation)] != L'.')
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = 254;

		// Draw Score
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

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
	std::cout << "         GAME OVER!!!\n";
	std::cout << "         Score: " << nScore << '\n';
	std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Delay a bit
	system("pause");
	return 0;
}
