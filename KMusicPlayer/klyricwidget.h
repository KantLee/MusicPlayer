// -----------------------------------------------------------
// klyricwidget.h
// 创建者： 李康
// 创建时间： 2024/05/31
// 功能描述： 歌词显示组件
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

// 一行歌词（一个时间点+对应的歌词文本）
class KLyricLine
{
public:
	KLyricLine() :time(0), text("") {}
	KLyricLine(qint64 time, QString text) :time(time), text(text) {}
	qint64 time = 0;
	QString text;
};

// 歌词按时间排序
bool operator <(const KLyricLine& A, const KLyricLine& B);

class KLyricWidget : public QWidget
{
	Q_OBJECT

public:
	KLyricWidget(QWidget *parent = Q_NULLPTR);
	~KLyricWidget();
	// 将歌词文件的内容处理为歌词结构的QList
	bool process(QString filePath);
	// 根据时间找到对应位置的歌词
	int getIndex(qint64 position);
	// 显示当前播放进度的歌词
	void show(qint64 position);
	// 根据下标获得歌词内容
	QString getLyricText(int index);
	// 清空歌词Label
	void clearLyric();

private:
	Ui::KLyricWidget *ui;
	// 储存所有歌词
	QVector<KLyricLine> m_lines;
};

#endif // _KMUSICPLAYER_KLYRICWIDGET_H_
