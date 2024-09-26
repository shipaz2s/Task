#include "mainwnd.hpp"
#include <QString>
#include <QPushButton>
#include <QGridLayout>

extern "C" {
#include "calculator.h"
}

MainWnd::MainWnd(QWidget *parent, Qt::WindowFlags flags) :
	QMainWindow(parent, flags)
{
	QFont cur_font = font();
	cur_font.setPixelSize(16);
	setFont(cur_font);

	setWindowTitle("Calculator");
	

	setCentralWidget(new QWidget(this));
	layout = new QGridLayout;
	centralWidget()->setLayout(layout);

	for (int i = 0; i < 16; ++i) {
		btns_digit.push_back(new QPushButton( QString::number(i, 16).toUpper() , this));
		connect(btns_digit.last(), &QPushButton::clicked, this, &MainWnd::btnDigitClicked);
	}

	btn_add = new QPushButton("+", this);
	btn_sub = new QPushButton("-", this);
	btn_div = new QPushButton("/", this);
	btn_mul = new QPushButton("*", this);
	btn_enter = new QPushButton("=", this);
	btn_dot = new QPushButton(",", this);
	btn_clear = new QPushButton("CE", this);
	btn_sign = new QPushButton("+/-", this);

	le = new QLineEdit(this);
	QFont fnt = font();
	fnt.setPixelSize(28);
	le->setFont(fnt);
	le->setReadOnly(true);
	le->setAlignment(Qt::AlignRight);

	connect(btn_add, &QPushButton::clicked, this, &MainWnd::btnOperationClicked);
	connect(btn_sub, &QPushButton::clicked, this, &MainWnd::btnOperationClicked);
	connect(btn_div, &QPushButton::clicked, this, &MainWnd::btnOperationClicked);
	connect(btn_mul, &QPushButton::clicked, this, &MainWnd::btnOperationClicked);
	connect(btn_enter, &QPushButton::clicked, this, &MainWnd::btnOperationClicked);
	connect(btn_dot, &QPushButton::clicked, this, &MainWnd::btnDotClicked);
	connect(btn_clear, &QPushButton::clicked, this, &MainWnd::btnClearClicked);
	connect(btn_sign, &QPushButton::clicked, this, &MainWnd::btnSignClicked);

	layout->addWidget(btns_digit[0], 4, 2);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			layout->addWidget(btns_digit[i + j*3 + 1], 3 - j, 2 + i);
		}
	}

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 3; ++j) {
			layout->addWidget(btns_digit[10 + i + j*2], j + 1, i );
		}
	}

	layout->addWidget(btn_clear, 4, 0);
	layout->addWidget(btn_sign, 4, 1);
	layout->addWidget(btn_dot, 4, 3);
	layout->addWidget(btn_enter, 4, 4);
	layout->addWidget(btn_add, 4, 5);
	layout->addWidget(btn_sub, 3, 5);
	layout->addWidget(btn_mul, 2, 5);
	layout->addWidget(btn_div, 1, 5);
	layout->addWidget(le, 0, 0, 1, 6);
}

void MainWnd::btnDigitClicked()
{
	QObject* obj = sender();
	QPushButton* button = qobject_cast<QPushButton*>(sender());

	if (operation_clicked) {
		operation_clicked = false;
		le->clear();
	}

	le->setText( le->text().append(button->text()) );
}

void MainWnd::btnOperationClicked()
{
	QObject* obj = sender();
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	char txt = button->text().toStdString()[0];
	

	Operation prev_operation = last_operation;
	
	if (prev_operation == Operation::Enter) {
		val1 = le->text();
	} else {
		val2 = le->text();
	}

	sFixed f1 = str2Fixed(val1.toStdString().c_str());
	sFixed f2 = str2Fixed(val2.toStdString().c_str());
	sFixed result;

	char* buffer = NULL;

	switch (prev_operation)
	{
	case Operation::Add:
		result = FixedAdd(&f1, &f2);
		break;
	case Operation::Sub:
		result = FixedSub(&f1, &f2);
		break;
	case Operation::Mul:
		result = FixedMul(&f1, &f2);
		break;
	case Operation::Div:
		result = FixedDiv(&f1, &f2);
		break;
	case Operation::Enter:
		break;
	
	default:
		break;
	}
	
	if (prev_operation != Operation::Enter) {
		buffer = Fixed2str(&result, buffer);
		le->setText( buffer );
		if (le->text() != "error") {
			val1 = le->text();
		}
	}

	free(buffer);

	switch (txt)
	{
	case '+':
		last_operation = Operation::Add;
		break;
	case '-':
		last_operation = Operation::Sub;
		break;
	case '*':
		last_operation = Operation::Mul;
		break;
	case '/':
		last_operation = Operation::Div;
		break;
	case '=':
		last_operation = Operation::Enter;
		break;
	
	default:
		break;
	}
	
	operation_clicked = true;
}

void MainWnd::btnDotClicked()
{
	if (operation_clicked) {
		operation_clicked = false;
		le->clear();
	}

	if (le->text().isEmpty() || le->text() == "-") {
		le->setText("0,");
		return;
	}

	if (le->text().contains(',') || le->text().contains('.')) {
		return;
	} else {
		le->setText(le->text().append(','));
	}
}

void MainWnd::btnClearClicked()
{
	val1 = "0";
	val2 = "0";
	le->clear();
	last_operation = Operation::Enter;
}

void MainWnd::btnSignClicked()
{
	QString txt = le->text();
	if (txt[0] != '-') {
		txt.push_front('-');
	} else {
		txt.remove('-');
	}
	le->setText(txt);
}