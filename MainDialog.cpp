#include "MainDialog.h"
#include "ClipItem.h"

#include <QCloseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QClipboard>
#include <QMimeData>
#include <QtWidgets/QMessageBox>
#include <Windows.h>

MainDialog::MainDialog(QWidget *parent) : QDialog(parent), WM_MYHOTKEY(1) {
	ui.setupUi(this);
	initMenu();
	setWindowFlags(Qt::Window
		| Qt::WindowCloseButtonHint
		| Qt::WindowMinimizeButtonHint
		| Qt::WindowMaximizeButtonHint
		| Qt::WindowStaysOnTopHint);
	isExit = false;
	if (!RegisterHotKey((HWND) this->winId(), WM_MYHOTKEY, MOD_ALT, VK_F6)) {
		QMessageBox::critical(this, 
			tr("ショットカット Ctrl+F6 は既に使用されています。サービスの使用は影響される恐れがありますので、チェックしてください。"),
			tr("エラー"));
		isExit = true;
		this->close();
	}

	SetClipboardViewer((HWND) this->winId());

	on_listWidget_currentRowChanged(-1);
}

void MainDialog::initMenu() {
	copyAction = new QAction(tr("コピー(&C)"), this);
	deleteAction = new QAction(tr("削除(&D)"), this);
	clearAction = new QAction(tr("クリア(&L)"), this);
	HideAction = new QAction(tr("キャンセル(&H)"), this);
	ExitAction = new QAction(tr("終了(&X)"), this);

	connect(copyAction, SIGNAL(triggered()), this, SLOT(on_pushButton_Copy_clicked()));
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(on_pushButton_Delete_clicked()));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(on_pushButton_Clear_clicked()));
	connect(HideAction, SIGNAL(triggered()), this, SLOT(on_pushButton_Cancel_clicked()));
	connect(ExitAction, SIGNAL(triggered()), this, SLOT(on_pushButton_Exit_clicked()));

	copyAction->setShortcut(QKeySequence("Ctrl+C"));
	deleteAction->setShortcut(QKeySequence("Delete"));
	clearAction->setShortcut(QKeySequence("Ctrl+Delete"));
	HideAction->setShortcut(QKeySequence("Esc"));

	contextMenu.addAction(copyAction);
	contextMenu.addAction(deleteAction);
	contextMenu.addAction(clearAction);
	contextMenu.addSeparator();
	contextMenu.addAction(HideAction);
	contextMenu.addAction(ExitAction);

	ui.listWidget->setContextMenu(&contextMenu);
}

// Hotkey Show & Clip Monitor
bool MainDialog::nativeEvent(const QByteArray &eventType, void *message, long *result) {
	Q_UNUSED(eventType)
	Q_UNUSED(result)
	auto msg = reinterpret_cast<MSG *>(message);

	switch (msg->message) {
	case WM_HOTKEY:
		if (msg->wParam == WM_MYHOTKEY) {
			this->show();
			return true;
		}
		break;
	case WM_DRAWCLIPBOARD:
		addClipItem();
		return true;
		break;
	}
	return false;
}

MainDialog::~MainDialog() {}

// Win Exit
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

// ESC Exit
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

void MainDialog::on_listWidget_currentRowChanged(int row) {
	int currRow = ui.listWidget->currentRow();
	ui.pushButton_Copy->setEnabled(currRow != -1);
	ui.pushButton_Delete->setEnabled(currRow != -1 && currRow != ui.listWidget->count() - 1);
	deleteAction->setEnabled(ui.pushButton_Delete->isEnabled());
}

// Add ClipItem to List & QList
void MainDialog::addClipItem(ClipItem *clip) {
	QClipboard *clipboard = QApplication::clipboard();
	// check clipbrd data type
	if (clipboard->mimeData()->hasText()) {
		QString content = clipboard->text();
		// check text already exist
		int idx = checkDuplicate(content);
		if (idx != -1) {
			ui.listWidget->takeItem(idx);
			clipItemList.removeAt(idx);
		}

		// new clipitem constant
		auto newadd = new ClipItem(content);
		if (clip != nullptr) newadd = clip;
		clipItemList.append(newadd);
		
		// new ui constant
		auto itm = new QListWidgetItem(ui.listWidget);
		QString cnt = newadd->content;
		itm->setText(QString("%1: \n%2")
			.arg(newadd->copytime.toString("yyyy-MM-dd hh:mm:ss"))
			.arg(cnt.replace(*new QRegExp("[\r|\n|\r\n]"), "")));
		itm->setToolTip(newadd->content);

		// add to ui & set selection
		ui.listWidget->addItem(itm);
		ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
	}
	ui.label_Koukei->setText(QString("(全部 %1 項)").arg(ui.listWidget->count()));
}

// コピー(&C)
void MainDialog::on_pushButton_Copy_clicked() {
	int row = ui.listWidget->currentRow();
	QApplication::clipboard()->setText(clipItemList.at(row)->content);
}

// 削除(&D)
void MainDialog::on_pushButton_Delete_clicked() {
	int row = ui.listWidget->currentRow();
	ui.listWidget->takeItem(row);
	ClipItem *del = clipItemList.at(row);
	clipItemList.removeAll(del);
	delete del;

	ui.label_Koukei->setText(QString("(全部 %1 項)").arg(ui.listWidget->count()));
}

// クリア(&L)
void MainDialog::on_pushButton_Clear_clicked() {
	auto last = new ClipItem(*clipItemList.last());
	ui.listWidget->clear();
	clipItemList.clear();
	qDeleteAll(clipItemList);
	addClipItem(last);
}

// 終了(&X)
void MainDialog::on_pushButton_Exit_clicked() {
	isExit = true;
	this->close();
}

// キャンセル(&C)
void MainDialog::on_pushButton_Cancel_clicked() {
	this->close();
}

// Check already exist
int MainDialog::checkDuplicate(QString content) {
	foreach (auto item, clipItemList) {
		if (item->content == content)
			return clipItemList.indexOf(item);
	}
	return -1;
}