#pragma once

namespace StripeMaker {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
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
			if (m_lastBarcode != nullptr)
			{
				delete m_lastBarcode;
				m_lastBarcode = nullptr;
			}
			if (pB_Main != nullptr && pB_Main->Image != nullptr)
			{
				delete pB_Main->Image;
				pB_Main->Image = nullptr;
			}
		}
	private: System::Windows::Forms::TextBox^ tB_Number;
	private: System::Windows::Forms::PictureBox^ pB_Main;
	private: System::Windows::Forms::Label^ l_Input;
	private: System::Windows::Forms::Label^ l_Preview;
	private: System::Drawing::Bitmap^ m_lastBarcode;
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
			this->l_Input = (gcnew System::Windows::Forms::Label());
			this->l_Preview = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pB_Main))->BeginInit();
			this->SuspendLayout();
			// 
			// tB_Number
			// 
			this->tB_Number->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tB_Number->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->tB_Number->Location = System::Drawing::Point(15, 32);
			this->tB_Number->Name = L"tB_Number";
			this->tB_Number->Size = System::Drawing::Size(371, 25);
			this->tB_Number->TabIndex = 0;
			// 
			// pB_Main
			// 
			this->pB_Main->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pB_Main->BackColor = System::Drawing::SystemColors::Window;
			this->pB_Main->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_Main->Location = System::Drawing::Point(15, 108);
			this->pB_Main->Name = L"pB_Main";
			this->pB_Main->Size = System::Drawing::Size(571, 190);
			this->pB_Main->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_Main->TabIndex = 1;
			this->pB_Main->TabStop = false;
			this->pB_Main->Resize += gcnew System::EventHandler(this, &MainForm::pB_Main_Resize);
			// 
			// b_Convert
			// 
			this->b_Convert->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->b_Convert->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->b_Convert->Location = System::Drawing::Point(392, 31);
			this->b_Convert->Name = L"b_Convert";
			this->b_Convert->Size = System::Drawing::Size(194, 27);
			this->b_Convert->TabIndex = 2;
			this->b_Convert->Text = L"—генерировать штрихкод";
			this->b_Convert->UseVisualStyleBackColor = true;
			this->b_Convert->Click += gcnew System::EventHandler(this, &MainForm::b_Convert_Click);
			// 
			// l_Input
			// 
			this->l_Input->AutoSize = true;
			this->l_Input->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->l_Input->Location = System::Drawing::Point(12, 12);
			this->l_Input->Name = L"l_Input";
			this->l_Input->Size = System::Drawing::Size(171, 15);
			this->l_Input->TabIndex = 3;
			this->l_Input->Text = L"¬ведите число / код дл€ C128:";
			// 
			// l_Preview
			// 
			this->l_Preview->AutoSize = true;
			this->l_Preview->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->l_Preview->Location = System::Drawing::Point(12, 88);
			this->l_Preview->Name = L"l_Preview";
			this->l_Preview->Size = System::Drawing::Size(157, 15);
			this->l_Preview->TabIndex = 4;
			this->l_Preview->Text = L"ѕредпросмотр штрихкода:";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::WhiteSmoke;
			this->ClientSize = System::Drawing::Size(604, 321);
			this->Controls->Add(this->l_Preview);
			this->Controls->Add(this->b_Convert);
			this->Controls->Add(this->pB_Main);
			this->Controls->Add(this->l_Input);
			this->Controls->Add(this->tB_Number);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F));
			this->MinimumSize = System::Drawing::Size(620, 360);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"StripeMaker Ч генератор штрихкодов Code128";
			this->Resize += gcnew System::EventHandler(this, &MainForm::MainForm_Resize);
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
			if (m_lastBarcode != nullptr) {
				delete m_lastBarcode;
				m_lastBarcode = nullptr;
			}

			Bitmap^ bmp = Code128Lib::Code128::GenerateCode128C(input, 2, 100, 10);
			m_lastBarcode = bmp;
			UpdatePictureBoxImage();
		}
		catch (Exception^ ex) {
			MessageBox::Show("ќшибка: " + ex->Message);
		}
	}

	private: System::Void pB_Main_Resize(System::Object^ sender, System::EventArgs^ e) {
		UpdatePictureBoxImage();
	}

	private: System::Void MainForm_Resize(System::Object^ sender, System::EventArgs^ e) {
		UpdatePictureBoxImage();
	}

	private: Rectangle GetFitRectangle(System::Drawing::Size sourceSize, System::Drawing::Size targetSize) {
		if (sourceSize.Width <= 0 || sourceSize.Height <= 0) {
			return Rectangle(0, 0, targetSize.Width, targetSize.Height);
		}

		double scaleW = static_cast<double>(targetSize.Width) / static_cast<double>(sourceSize.Width);
		double scaleH = static_cast<double>(targetSize.Height) / static_cast<double>(sourceSize.Height);
		double scale = System::Math::Min(scaleW, scaleH);

		int width = System::Math::Max(1, static_cast<int>(System::Math::Round(sourceSize.Width * scale)));
		int height = System::Math::Max(1, static_cast<int>(System::Math::Round(sourceSize.Height * scale)));
		int offsetX = (targetSize.Width - width) / 2;
		int offsetY = (targetSize.Height - height) / 2;

		return Rectangle(offsetX, offsetY, width, height);
	}

	private: Bitmap^ CreateFittedBarcodeBitmap(Bitmap^ sourceBarcode, System::Drawing::Size targetSize) {
		if (sourceBarcode == nullptr || targetSize.Width <= 0 || targetSize.Height <= 0) {
			return nullptr;
		}

		Bitmap^ canvas = gcnew Bitmap(targetSize.Width, targetSize.Height);
		Graphics^ g = Graphics::FromImage(canvas);
		try {
			g->Clear(Color::White);
			g->InterpolationMode = InterpolationMode::NearestNeighbor;
			g->PixelOffsetMode = PixelOffsetMode::Half;
			Rectangle dest = GetFitRectangle(sourceBarcode->Size, targetSize);
			g->DrawImage(sourceBarcode, dest);
		}
		finally {
			delete g;
		}

		return canvas;
	}

	private: System::Void UpdatePictureBoxImage(void) {
		if (m_lastBarcode == nullptr || pB_Main == nullptr) {
			return;
		}

		System::Drawing::Size targetSize = pB_Main->ClientSize;
		Bitmap^ scaled = CreateFittedBarcodeBitmap(m_lastBarcode, targetSize);
		if (scaled == nullptr) {
			return;
		}

		if (pB_Main->Image != nullptr) {
			delete pB_Main->Image;
			pB_Main->Image = nullptr;
		}

		pB_Main->Image = scaled;
		pB_Main->SizeMode = PictureBoxSizeMode::Normal;
	}
	};
}
