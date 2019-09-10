#include "MainDialog.h"

#include <QCloseEvent>
#include <QKeyEvent>
#include <QtWidgets/QMessageBox>
#include <Windows.h>

MainDialog::MainDialog(QWidget *parent) : QDialog(parent), WM_MYHOTKEY(1) {
	ui.setupUi(this);
	setWindowFlags(Qt::Window
		| Qt::WindowCloseButtonHint
		| Qt::WindowMinimizeButtonHint
		| Qt::WindowStaysOnTopHint);
	isExit = false;
	if (!RegisterHotKey((HWND) this->winId(), WM_MYHOTKEY, MOD_ALT, VK_F6)) {
		QMessageBox::critical(this, 
			tr("ショットカット Ctrl+F6 は既に使用されています。サービスの使用は影響される恐れがありますので、チェックしてください。"),
			tr("エラー"));
		isExit = true;
		this->close();
	}
}

bool MainDialog::nativeEvent(const QByteArray &eventType, void *message, long *result) {
	Q_UNUSED(eventType)
	Q_UNUSED(result)
	auto msg = reinterpret_cast<MSG *>(message);

	if (msg->message == WM_HOTKEY) {
		if (msg->wParam == WM_MYHOTKEY) {
			this->show();
			return true;
		}
	}
	return false;
}

MainDialog::~MainDialog() {}

void MainDialog::closeEvent(QCloseEvent *e) {
	if (isExit) {
		UnregisterHotKey((HWND) this->winId(), WM_MYHOTKEY);
		e->accept();
	}
	else {
		this->hide();
		e->ignore();
	}
}

void MainDialog::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Escape:
		isExit = false;
		this->close();
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}

void MainDialog::on_pushButton_Copy_clicked() {

}

void MainDialog::on_pushButton_Delete_clicked() {

}

void MainDialog::on_pushButton_Exit_clicked() {
	isExit = true;
	this->close();
}

void MainDialog::on_pushButton_Cancel_clicked() {
	this->close();
}