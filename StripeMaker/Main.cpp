#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace StripeMaker;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Создание формы с обработкой аргументов командной строки
    String^ commandLineArg = (args->Length > 0) ? args[0] : nullptr;
    Application::Run(gcnew MainForm(commandLineArg));
    return 0;
}
