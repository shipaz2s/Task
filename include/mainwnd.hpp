#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>

enum class Operation
{
	Add,
	Sub,
	Mul,
	Div,
	Enter
};

class MainWnd : public QMainWindow
{
	Q_OBJECT

public:
	MainWnd(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

	~MainWnd(){};

private:
	QGridLayout* layout;
	QVector<QPushButton*> btns_digit;
	QPushButton* btn_add;
	QPushButton* btn_sub;
	QPushButton* btn_mul;
	QPushButton* btn_div;
	QPushButton* btn_dot;
	QPushButton* btn_enter;
	QPushButton* btn_clear;
	QPushButton* btn_sign;
	QLineEdit* le;

	bool operation_clicked = false;
	Operation last_operation = Operation::Enter;

	QString val1 = "0";
	QString val2 = "0";
	QString result = "0";

private slots:
	void btnDigitClicked();
	void btnOperationClicked();
	void btnDotClicked();
	void btnClearClicked();
	void btnSignClicked();

};