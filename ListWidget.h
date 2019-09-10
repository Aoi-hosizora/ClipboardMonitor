#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QtWidgets/QListWidget>
#include <QMenu>
#include <QKeyEvent>

class ListWidget : public QListWidget {

public:
	ListWidget(QWidget *parent) : QListWidget(parent), contextMenu(nullptr) {}

	void setContextMenu(QMenu *m) {
		this->contextMenu = m;
	}

	QMenu *getContextMenu() {
		return contextMenu;
	}

protected:
	void keyPressEvent(QKeyEvent *e) override {
		if (e->type() == QKeyEvent::KeyPress) 
			if (e->matches(QKeySequence::Copy)) {
				e->accept();
				return;
			}
		QListWidget::keyPressEvent(e);
	}

	void contextMenuEvent(QContextMenuEvent *e) override {
		if (contextMenu != nullptr) {
			contextMenu->exec(QCursor::pos());
			e->accept();
		}
	}

private:
	QMenu *contextMenu;
};

#endif // LISTWIDGET_H