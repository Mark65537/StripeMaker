#pragma once

using namespace System;
using namespace System::Drawing;

namespace Code128Lib {

	public ref class Code128
	{
	public:
		/// <summary>
		/// √енерирует Bitmap с штрихкодом Code128 (Code Set C). ¬ходна€ строка должна содержать только цифры и иметь четное число символов.
		/// moduleWidth Ч ширина минимального модул€ в пиксел€х (обычно 2..4), height Ч высота штрих-кода (включа€ подн€тие текста).
		/// quietZoneModules Ч ширина зоны молчани€ слева/справа в модул€х (рекомендуетс€ 10).
		/// </summary>
		static Bitmap^ GenerateCode128C(String^ digits, int moduleWidth, int height, int quietZoneModules);
	};
}

