// -----------------------------------------------------------
// klyricwidget.h
// �����ߣ� �
// ����ʱ�䣺 2024/05/31
// ���������� �����ʾ���
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KLYRICWIDGET_H_
#define _KMUSICPLAYER_KLYRICWIDGET_H_

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QVector>
#include <QTextCodec>
#include <algorithm>

namespace Ui { class KLyricWidget; };

// һ�и�ʣ�һ��ʱ���+��Ӧ�ĸ���ı���
class KLyricLine
{
public:
	KLyricLine() :time(0), text("") {}
	KLyricLine(qint64 time, QString text) :time(time), text(text) {}
	qint64 time = 0;
	QString text;
};

// ��ʰ�ʱ������
bool operator <(const KLyricLine& A, const KLyricLine& B);

class KLyricWidget : public QWidget
{
	Q_OBJECT

public:
	KLyricWidget(QWidget *parent = Q_NULLPTR);
	~KLyricWidget();
	// ������ļ������ݴ���Ϊ��ʽṹ��QList
	bool process(QString filePath);
	// ����ʱ���ҵ���Ӧλ�õĸ��
	int getIndex(qint64 position);
	// ��ʾ��ǰ���Ž��ȵĸ��
	void show(qint64 position);
	// �����±��ø������
	QString getLyricText(int index);
	// ��ո��Label
	void clearLyric();

private:
	Ui::KLyricWidget *ui;
	// �������и��
	QVector<KLyricLine> m_lines;
};

#endif // _KMUSICPLAYER_KLYRICWIDGET_H_
