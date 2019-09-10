#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include "ui_MainDialog.h"
#include <QtWidgets/QDialog>

class MainDialog : public QDialog {
	Q_OBJECT

public:
	MainDialog(QWidget *parent = nullptr);
	~MainDialog();

private slots:
	void closeEvent(QCloseEvent *) override;
	void keyPressEvent(QKeyEvent *) override;
	void on_pushButton_Copy_clicked();
	void on_pushButton_Delete_clicked();
	void on_pushButton_Exit_clicked();
	void on_pushButton_Cancel_clicked();

protected:
	bool nativeEvent(const QByteArray &, void *, long *) override;

private:
	Ui::MainDialogClass ui;
	bool isExit;
	const int WM_MYHOTKEY;
};

#endif // MAINDIALOG_H
