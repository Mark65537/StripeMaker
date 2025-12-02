#pragma once

namespace StripeMaker {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Code128Lib;

	/// <summary>
	/// —водка дл€ MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ tB_Number;
	private: System::Windows::Forms::PictureBox^ pB_Main;
	protected:

	protected:

	private: System::Windows::Forms::Button^ b_Convert;


	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tB_Number = (gcnew System::Windows::Forms::TextBox());
			this->pB_Main = (gcnew System::Windows::Forms::PictureBox());
			this->b_Convert = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pB_Main))->BeginInit();
			this->SuspendLayout();
			// 
			// tB_Number
			// 
			this->tB_Number->Location = System::Drawing::Point(12, 12);
			this->tB_Number->Name = L"tB_Number";
			this->tB_Number->Size = System::Drawing::Size(373, 20);
			this->tB_Number->TabIndex = 0;
			// 
			// pB_Main
			// 
			this->pB_Main->Location = System::Drawing::Point(12, 76);
			this->pB_Main->Name = L"pB_Main";
			this->pB_Main->Size = System::Drawing::Size(373, 178);
			this->pB_Main->TabIndex = 1;
			this->pB_Main->TabStop = false;
			// 
			// b_Convert
			// 
			this->b_Convert->Location = System::Drawing::Point(13, 39);
			this->b_Convert->Name = L"b_Convert";
			this->b_Convert->Size = System::Drawing::Size(87, 23);
			this->b_Convert->TabIndex = 2;
			this->b_Convert->Text = L"√енерировать";
			this->b_Convert->UseVisualStyleBackColor = true;
			this->b_Convert->Click += gcnew System::EventHandler(this, &MainForm::b_Convert_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(398, 290);
			this->Controls->Add(this->b_Convert);
			this->Controls->Add(this->pB_Main);
			this->Controls->Add(this->tB_Number);
			this->Name = L"MainForm";
			this->Text = L"StripeMaker";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pB_Main))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void b_Convert_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ input = tB_Number->Text->Trim();
		if (input->Length == 0) { MessageBox::Show("¬ведите цифры."); return; }
		// TODO решить что делать с нечетной длиной
		try {
			Bitmap^ bmp = Code128Lib::Code128::GenerateCode128C(input, 2, 100, 10);

			pB_Main->Image = bmp;
			pB_Main->SizeMode = PictureBoxSizeMode::CenterImage;
		}
		catch (Exception^ ex) {
			MessageBox::Show("ќшибка: " + ex->Message);
		}
	}
	};
}
