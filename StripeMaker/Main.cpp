#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace StripeMaker;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Создаём и запускаем основную форму
    Application::Run(gcnew MainForm());
    return 0;
}
