#pragma once

using namespace System;
using namespace System::Drawing;

namespace Code128Lib {

	public ref class Code128
	{
	public:
		/// <summary>
		/// Генерирует Bitmap с штрихкодом Code128 (Code Set C). Входная строка должна содержать только цифры и иметь четное число символов.
		/// moduleWidth — ширина минимального модуля в пикселях (обычно 2..4), height — высота штрих-кода (включая поднятие текста).
		/// quietZoneModules — ширина зоны молчания слева/справа в модулях (рекомендуется 10).
		/// </summary>
		static Bitmap^ GenerateCode128C(String^ digits, int moduleWidth, int height, int quietZoneModules);

		/// <summary>
		/// Генерирует Bitmap с штрихкодом Code128 (Code Set B) для произвольной строковой информации
		/// из печатаемых ASCII-символов (код 32..126). Остальные символы не поддерживаются.
		/// </summary>
		static Bitmap^ GenerateCode128B(String^ text, int moduleWidth, int height, int quietZoneModules);

		/// <summary>
		/// Генерирует Bitmap с штрихкодом Code128 (Code Set A).
		/// Допускаются символы с кодами ASCII 0..95 (включая управляющие и заглавные буквы).
		/// </summary>
		static Bitmap^ GenerateCode128A(String^ text, int moduleWidth, int height, int quietZoneModules);
	};
}

