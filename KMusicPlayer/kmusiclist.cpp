#include "kmusiclist.h"
#include "kmusiclistwidget.h"

KMusicList::KMusicList(const QList<QUrl> &urls)
{
	addMusic(urls);
}

KMusicList::~KMusicList()
{
}

void KMusicList::setSQL(bool on)
{
	m_sqlFlag = on;
}


void KMusicList::addMusic(const QList<QUrl>& urls)
{
	// 实测这里耗时较长，所以添加一个进度显示对话框
	QProgressDialog proDialog(QStringLiteral("添加进度"), QStringLiteral("取消"), 0, urls.size());
	proDialog.setMinimumSize(350, 150);
	proDialog.setWindowModality(Qt::WindowModal);
	proDialog.setWindowTitle(QStringLiteral("添加中...请稍后"));
	proDialog.show();
	int x = 0;
	for (QUrl i : urls)
	{
		x++;
		proDialog.setValue(x);
		// 过滤Url的类型
		QMimeDatabase db;
		QMimeType mime = db.mimeTypeForFile(i.toLocalFile());
		if (mime.name() != "audio/mpeg" && mime.name() != "audio/flac")
			continue;
		// 剩下的符合类型
		m_musicArray.push_back(KMusic(i));
		if (m_sqlFlag)
			// 插入到数据库中
			m_musicArray[m_musicArray.size() - 1].insertSQL();
		if (proDialog.wasCanceled())
			break;
	}
}

void KMusicList::addMusic(const KMusic& music)
{
	m_musicArray.push_back(music);
	if (m_sqlFlag)
		m_musicArray[m_musicArray.size() - 1].insertSQL();
}

KMusic KMusicList::getMusic(int pos)
{
	return m_musicArray[pos];
}

void KMusicList::addToPlayList(QMediaPlaylist *playlist)
{
	for (auto i = m_musicArray.begin(); i != m_musicArray.end(); i++)
	{
		playlist->addMedia(i->getUrl());
	}
}

void KMusicList::addToListWidget(KMusicListWidget *listWidget)
{
	for (KMusic &i : m_musicArray)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setIcon(listWidget->getIcon());
		item->setText(i.getInfo());
		listWidget->addItem(item);
	}
}

void KMusicList::removeMusic(int pos)
{
	if (m_sqlFlag)
	{
		int i = 0;
		for (auto it = m_musicArray.begin(); it != m_musicArray.end();)
		{
			if (i == pos)
			{
				KDataBase::getInstance().removeMusic(it->getUrl());
				it = m_musicArray.erase(it);
				break;
			}
			else
			{
				it++;
			}
			i++;
		}
	}
	else
	{
		int i = 0;
		for (auto it =m_musicArray.begin(); it != m_musicArray.end();) 
		{
			if (i == pos)
			{
				it = m_musicArray.erase(it);
				break;
			}
			else
			{
				it++;
			}
			i++;
		}
	}
}

void KMusicList::showInExplorer(int pos)
{
	QString str = m_musicArray[pos].getUrl().toString();
	str.remove(str.split("/").last());	// 切割字符串获得所在的文件夹路径
	QDesktopServices::openUrl(str);
}

void KMusicList::detail(int pos)
{
	m_musicArray[pos].detail();
}

void KMusicList::clear()
{
	m_musicArray.clear();
	if (m_sqlFlag)
		KDataBase::getInstance().removeAllMusic();
}

QVector<KMusic> &KMusicList::getMusicArray()
{
	return m_musicArray;
}

void KMusicList::setMusicArray(QVector<KMusic> musicArray)
{
	m_musicArray = musicArray;
}
