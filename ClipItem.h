#ifndef CLIPITEM_H
#define CLIPITEM_H

#include <QDateTime>

class ClipItem {
public:
	ClipItem(QString content) 
		: content(content), copytime(QDateTime::currentDateTime()) { }

	QString content;
	QDateTime copytime;
};

#endif // CLIPITEM_H