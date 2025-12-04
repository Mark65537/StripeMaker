#include "Code128Lib.h"
#include <vector>
#include <string>

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace Code128Lib;

/// <summary>
/// Таблица паттернов Code128: 107 строк (0..106). Каждая запись — строка '1' и '0' длиной 11 (1=бар,0=пробел)
/// </summary>
static const char* CODE128_PATTERNS[] = {
    "11011001100","11001101100","11001100110","10010011000","10010001100","10001001100","10011001000","10011000100","10001100100","11001001000",
    "11001000100","11000100100","10110011100","10011011100","10011001110","10111001100","10011101100","10011100110","11001110010","11001011100",
    "11001001110","11011100100","11001110100","11101101110","11101001100","11100101100","11100100110","11101100100","11100110100","11100110010",
    "11011011000","11011000110","11000110110","10100011000","10001011000","10001000110","10110001000","10001101000","10001100010","11010001000",
    "11000101000","11000100010","10110111000","10110001110","10001101110","10111011000","10111000110","10001110110","11101110110","11010001110",
    "11000101110","11011101000","11011100010","11011101110","11101011000","11101000110","11100010110","11101101000","11101100010","11100011010",
    "11101111010","11001000010","11110001010","10100110000","10100001100","10010110000","10010000110","10000101100","10000100110","10110010000",
    "10110000100","10011010000","10011000010","10000110100","10000110010","11000010010","11001010000","11110111010","11000010100","10001111010",
    "10100111100","10010111100","10010011110","10111100100","10011110100","10011110010","11110100100","11110010100","11110010010","11011011110",
    "11011110110","11110110110","10101111000","10100011110","10001011110","10111101000","10111100010","11110101000","11110100010","10111011110",
    "10111101110","11101011110","11110101110",
    // StartA StartB StartC and STOP patterns:
    "11010000100", // 103 Start A
    "11010010000", // 104 Start B
    "11010011100", // 105 Start C
    "11000111010"  // 106 STOP (последний)
};
 
/// <summary>
/// Преобразование pattern в массив ширин модулей : последовательность: bar, space, bar, space, bar, space(6 чисел).
/// </summary>
/// <remarks>Пример: "11000111010"→"233111"</remarks>
/// <param name="pat">ссылка на паттерн</param>
/// <returns></returns>
static std::vector<int> patternToWidths(const char* pat) {
    std::vector<int> widths;
    int len = (int)strlen(pat);
    int i = 0;
    while (i < len) {
        char curr = pat[i];
        int count = 0;
        while (i < len && pat[i] == curr) { ++count; ++i; }
        widths.push_back(count);
    }
    // Должно быть 6 значений (3 bars + 3 spaces), но стоп-символ имеет особенность — мы всё равно отрисуем по последовательности.
    return widths;
}

/// <summary>
/// Преобразует .NET String в std::string с проверкой цифр
/// </summary>
/// <param name="s"></param>
/// <param name="out"></param>
/// <returns></returns>
static bool toStdDigits(String^ s, std::string& out) {
    if (s == nullptr) return false;
    for each (wchar_t ch in s) {
        if (ch < L'0' || ch > L'9') return false;
        out.push_back(char(ch));
    }
    return true;
}

Bitmap^ Code128::GenerateCode128C(String^ digits, int moduleWidth, int height, int quietZoneModules) {
    // Проверки
    if (moduleWidth <= 0) moduleWidth = 2;
    if (height < 20) height = 60;
    if (quietZoneModules < 10) quietZoneModules = 10;

    std::string s;
    if (!toStdDigits(digits, s)) throw gcnew ArgumentException("Input must be digits only.");

    // Если длина нечетная выбросим исключение.
    if (s.size() % 2 != 0) throw gcnew ArgumentException("Size is not /2.");

    // Сборка последовательности значений: StartC (105), затем каждая пара -> value 0..99
    std::vector<int> values;
    const int START_C = 105;
    const int STOP = 106;
    values.push_back(START_C);

    for (size_t i = 0; i < s.size(); i += 2) {
        int v = (s[i] - '0') * 10 + (s[i + 1] - '0');
        values.push_back(v);
    }

    // Контрольная сумма
    int checksum = START_C;
    for (size_t i = 1; i < values.size(); ++i) {
        checksum += values[i] * (int)i;
    }
    checksum %= 103;
    values.push_back(checksum);
    values.push_back(STOP);

    // Построим последовательность модулей (ширин) в единицах модулей (1..4)
    std::vector<int> modules; // чередующиеся: barWidth, spaceWidth, barWidth, ...
    // quiet zone (в модулях) делаем как пробел (space)
    for (int q = 0;q < quietZoneModules;++q) modules.push_back(0); // placeholder для ширины quiet zone, учитывается в расчёте пикселей отдельно

    for (size_t idx = 0; idx < values.size(); ++idx) {
        int val = values[idx];
        if (val < 0 || val > 106) throw gcnew InvalidOperationException("Invalid code value.");
        const char* pattern = CODE128_PATTERNS[val];
        std::vector<int> w = patternToWidths(pattern);
        
        for (size_t k = 0; k < w.size(); ++k) {
            modules.push_back(w[k]);
        }
    }
    // После стоп символа Code128 добавляет финальный 2-модуль бар (у нас включен в паттерн стопа)
    // Теперь преобразуем в пиксели и рисуем
    // Рассчитаем ширину: quietZone left and right
    int leftQuietPx = quietZoneModules * moduleWidth;
    int rightQuietPx = leftQuietPx;

    // Рассчитать общую сумму модулей, исключая quiet placeholders
    int sumModules = 0;
    
    size_t pos = 0;
    while (pos < modules.size() && modules[pos] == 0) pos++;
    for (size_t i = pos; i < modules.size(); ++i) sumModules += modules[i];

    int totalWidth = leftQuietPx + rightQuietPx + sumModules * moduleWidth;

    Bitmap^ bmp = gcnew Bitmap(totalWidth, height);
    Graphics^ g = Graphics::FromImage(bmp);
    g->Clear(Color::White);

    // Нарисуем штрихкод: начинаем от leftQuietPx. Модули чередуются: первым модулем в каждом символе — bar (черный).
    int x = leftQuietPx;
    bool drawBar = true;
    // пропустим начальные нули
    size_t i = 0;
    while (i < modules.size() && modules[i] == 0) ++i;

    for (; i < modules.size(); ++i) {
        int wMod = modules[i];
        int wPx = wMod * moduleWidth;
        if (drawBar) {
            if (wPx > 0) {
                g->FillRectangle(Brushes::Black, x, 0, wPx, height);
            }
        }
        // смещаемся
        x += wPx;
        drawBar = !drawBar;
    }

    delete g;
    return bmp;
}

Bitmap^ Code128::GenerateCode128B(String^ text, int moduleWidth, int height, int quietZoneModules) {
	// Проверки параметров
	if (moduleWidth <= 0) moduleWidth = 2;
	if (height < 20) height = 60;
	if (quietZoneModules < 10) quietZoneModules = 10;

	if (text == nullptr) throw gcnew ArgumentNullException("text");
	if (text->Length == 0) throw gcnew ArgumentException("Text must not be empty.");

	// Формируем последовательность значений: StartB (104), затем каждый символ -> value 0..94
	// В Code128B символы соответствуют печатаемым ASCII 32..126, value = code - 32.
	std::vector<int> values;
	const int START_B = 104;
	const int STOP = 106;
	values.push_back(START_B);

	int index = 0;
	for each (wchar_t ch in text) {
		// Разрешаем только печатаемые ASCII 32..126
		if (ch < 32 || ch > 126) {
			throw gcnew ArgumentException("Unsupported character for Code128B (allowed ASCII 32..126).");
		}
		int v = int(ch) - 32; // 0..94
		values.push_back(v);
		++index;
	}

	// Контрольная сумма
	int checksum = START_B;
	for (size_t i = 1; i < values.size(); ++i) {
		checksum += values[i] * (int)i;
	}
	checksum %= 103;
	values.push_back(checksum);
	values.push_back(STOP);

	// Построим последовательность модулей (ширин) в единицах модулей (1..4)
	std::vector<int> modules; // чередующиеся: barWidth, spaceWidth, barWidth, ...
	// quiet zone (в модулях) делаем как пробел (space)
	for (int q = 0; q < quietZoneModules; ++q) modules.push_back(0); // placeholder для ширины quiet zone, учитывается в расчёте пикселей отдельно

	for (size_t idx = 0; idx < values.size(); ++idx) {
		int val = values[idx];
		if (val < 0 || val > 106) throw gcnew InvalidOperationException("Invalid code value.");
		const char* pattern = CODE128_PATTERNS[val];
		std::vector<int> w = patternToWidths(pattern);

		for (size_t k = 0; k < w.size(); ++k) {
			modules.push_back(w[k]);
		}
	}

	// Рассчитаем ширину: quietZone left and right
	int leftQuietPx = quietZoneModules * moduleWidth;
	int rightQuietPx = leftQuietPx;

	// Рассчитать общую сумму модулей, исключая quiet placeholders
	int sumModules = 0;

	size_t pos = 0;
	while (pos < modules.size() && modules[pos] == 0) pos++;
	for (size_t i = pos; i < modules.size(); ++i) sumModules += modules[i];

	int totalWidth = leftQuietPx + rightQuietPx + sumModules * moduleWidth;

	Bitmap^ bmp = gcnew Bitmap(totalWidth, height);
	Graphics^ g = Graphics::FromImage(bmp);
	g->Clear(Color::White);

	// Нарисуем штрихкод: начинаем от leftQuietPx. Модули чередуются: первым модулем в каждом символе — bar (черный).
	int x = leftQuietPx;
	bool drawBar = true;
	// пропустим начальные нули
	size_t i = 0;
	while (i < modules.size() && modules[i] == 0) ++i;

	for (; i < modules.size(); ++i) {
		int wMod = modules[i];
		int wPx = wMod * moduleWidth;
		if (drawBar) {
			if (wPx > 0) {
				g->FillRectangle(Brushes::Black, x, 0, wPx, height);
			}
		}
		// смещаемся
		x += wPx;
		drawBar = !drawBar;
	}

	delete g;
	return bmp;
}

Bitmap^ Code128::GenerateCode128A(String^ text, int moduleWidth, int height, int quietZoneModules) {
	// Проверки параметров
	if (moduleWidth <= 0) moduleWidth = 2;
	if (height < 20) height = 60;
	if (quietZoneModules < 10) quietZoneModules = 10;

	if (text == nullptr) throw gcnew ArgumentNullException("text");
	if (text->Length == 0) throw gcnew ArgumentException("Text must not be empty.");

	// Формируем последовательность значений: StartA (103), затем каждый символ -> value 0..95
	// В Code128A символы соответствуют ASCII 0..95, value = code (0..95).
	std::vector<int> values;
	const int START_A = 103;
	const int STOP = 106;
	values.push_back(START_A);

	for each (wchar_t ch in text) {
		// Допускаем только диапазон ASCII 0..95
		if (ch < 0 || ch > 95) {
			throw gcnew ArgumentException("Unsupported character for Code128A (allowed ASCII 0..95).");
		}
		int v = int(ch); // 0..95
		values.push_back(v);
	}

	// Контрольная сумма
	int checksum = START_A;
	for (size_t i = 1; i < values.size(); ++i) {
		checksum += values[i] * (int)i;
	}
	checksum %= 103;
	values.push_back(checksum);
	values.push_back(STOP);

	// Построим последовательность модулей (ширин) в единицах модулей (1..4)
	std::vector<int> modules; // чередующиеся: barWidth, spaceWidth, barWidth, ...
	// quiet zone (в модулях) делаем как пробел (space)
	for (int q = 0; q < quietZoneModules; ++q) modules.push_back(0); // placeholder для ширины quiet zone, учитывается в расчёте пикселей отдельно

	for (size_t idx = 0; idx < values.size(); ++idx) {
		int val = values[idx];
		if (val < 0 || val > 106) throw gcnew InvalidOperationException("Invalid code value.");
		const char* pattern = CODE128_PATTERNS[val];
		std::vector<int> w = patternToWidths(pattern);

		for (size_t k = 0; k < w.size(); ++k) {
			modules.push_back(w[k]);
		}
	}

	// Рассчитаем ширину: quietZone left and right
	int leftQuietPx = quietZoneModules * moduleWidth;
	int rightQuietPx = leftQuietPx;

	// Рассчитать общую сумму модулей, исключая quiet placeholders
	int sumModules = 0;

	size_t pos = 0;
	while (pos < modules.size() && modules[pos] == 0) pos++;
	for (size_t i = pos; i < modules.size(); ++i) sumModules += modules[i];

	int totalWidth = leftQuietPx + rightQuietPx + sumModules * moduleWidth;

	Bitmap^ bmp = gcnew Bitmap(totalWidth, height);
	Graphics^ g = Graphics::FromImage(bmp);
	g->Clear(Color::White);

	// Нарисуем штрихкод: начинаем от leftQuietPx. Модули чередуются: первым модулем в каждом символе — bar (черный).
	int x = leftQuietPx;
	bool drawBar = true;
	// пропустим начальные нули
	size_t i = 0;
	while (i < modules.size() && modules[i] == 0) ++i;

	for (; i < modules.size(); ++i) {
		int wMod = modules[i];
		int wPx = wMod * moduleWidth;
		if (drawBar) {
			if (wPx > 0) {
				g->FillRectangle(Brushes::Black, x, 0, wPx, height);
			}
		}
		// смещаемся
		x += wPx;
		drawBar = !drawBar;
	}

	delete g;
	return bmp;
}