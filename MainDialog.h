#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include "ui_MainDialog.h"
#include "ClipItem.h"

#include <QtWidgets/QDialog>
#include <QMenu>

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
	void on_pushButton_Clear_clicked();
	void on_pushButton_Exit_clicked();
	void on_pushButton_Cancel_clicked();
	void on_listWidget_currentRowChanged(int);

protected:
	bool nativeEvent(const QByteArray &, void *, long *) override;

private:
	Ui::MainDialogClass ui;

	QMenu contextMenu;
	QAction *copyAction;
	QAction *deleteAction;
	QAction *clearAction;
	QAction *HideAction;
	QAction *ExitAction;

private:
	bool isExit;
	const int WM_MYHOTKEY;
	QList<ClipItem *> clipItemList;

	void addClipItem(ClipItem * = nullptr);
	int checkDuplicate(QString);
	void initMenu();
};

#endif // MAINDIALOG_H
