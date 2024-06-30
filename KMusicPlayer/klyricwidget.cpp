#include "klyricwidget.h"
#include "ui_klyricwidget.h"

KLyricWidget::KLyricWidget(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::KLyricWidget();
	ui->setupUi(this);
	// 先清空原来UI中存在的内容
	clearLyric();
}

KLyricWidget::~KLyricWidget()
{
	delete ui;
}

bool KLyricWidget::process(QString filePath)
{
	QFile lyricFile(filePath);
	lyricFile.open(QFile::ReadOnly);
	QString content(lyricFile.readAll());
	content.replace("\\r", "");
	content.replace("\\n", "");
	lyricFile.close();
	// 清空歌词
	m_lines.clear();
	const QRegExp rx("\\[(\\d+):(\\d+(\\.\\d+)?)\\]"); // 用来查找时间标签的正则表达式
	// 步骤1
	int pos = rx.indexIn(content);
	if (pos == -1)
	{
		return false;
	}
	else
	{
		int lastPos;
		QList<int> timeLabels;
		do
		{
			// 步骤2
			timeLabels << (rx.cap(1).toInt() * 60 + rx.cap(2).toDouble()) * 1000;
			lastPos = pos + rx.matchedLength();
			pos = rx.indexIn(content, lastPos);
			if (pos == -1)
			{
				QString text = content.mid(lastPos).trimmed();
				for (const int& time : timeLabels)
				{
					m_lines.push_back(KLyricLine(time, text));
				}
				break;
			}
			// 步骤3
			QString text = content.mid(lastPos, pos - lastPos);
			if (!text.isEmpty())
			{
				for (const int& time : timeLabels)
				{
					m_lines.push_back(KLyricLine(time, text.trimmed()));
				}
				timeLabels.clear();
			}
		}while (true);
		// 步骤4
		std::stable_sort(m_lines.begin(), m_lines.end());
	}
	if (m_lines.size())
		return true;
	return false;
}

int KLyricWidget::getIndex(qint64 position)
{
	if (!m_lines.size()) 
	{
		return -1;
	}
	else 
	{
		if (m_lines[0].time >= position) 
			return 0;
	}
	int i = 1;
	for (i = 1; i < m_lines.size(); i++) 
	{
		if (m_lines[i - 1].time < position && m_lines[i].time >= position)
			return i - 1;
	}
	return m_lines.size() - 1;
}

void KLyricWidget::show(qint64 position)
{
	int index = getIndex(position);
	if (index == -1)
	{
		ui->labelLyric1->setText("");
		ui->labelLyric2->setText("");
		ui->labelLyric3->setText("");
		ui->labelPlaying->setText(QStringLiteral("当前歌曲无歌词"));
		ui->labelLyric5->setText("");
		ui->labelLyric6->setText("");
		ui->labelLyric7->setText("");
	}
	else 
	{
		ui->labelLyric1->setText(getLyricText(index - 3));
		ui->labelLyric2->setText(getLyricText(index - 2));
		ui->labelLyric3->setText(getLyricText(index - 1));
		ui->labelPlaying->setText(getLyricText(index));
		ui->labelLyric5->setText(getLyricText(index + 1));
		ui->labelLyric6->setText(getLyricText(index + 2));
		ui->labelLyric7->setText(getLyricText(index + 3));
	}
}

QString KLyricWidget::getLyricText(int index)
{
	if (index < 0 || index >= m_lines.size())
		return "";
	return m_lines[index].text;
}

void KLyricWidget::clearLyric()
{
	ui->labelLyric1->setText("");
	ui->labelLyric2->setText("");
	ui->labelLyric3->setText("");
	ui->labelPlaying->setText("");
	ui->labelLyric5->setText("");
	ui->labelLyric6->setText("");
	ui->labelLyric7->setText("");
}

bool operator<(const KLyricLine& A, const KLyricLine& B)
{
	return A.time < B.time;
}
