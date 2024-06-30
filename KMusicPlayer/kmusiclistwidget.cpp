#include "kmusiclistwidget.h"

KMusicListWidget::KMusicListWidget(QWidget *parent)
	: QListWidget(parent)
	,m_icon(QIcon(":/images/music.png"))
{
}

KMusicListWidget::~KMusicListWidget()
{
}

void KMusicListWidget::refresh()
{
	clear();// 先清空
	m_musicList.addToListWidget(this);
}


void KMusicListWidget::setMusicList_playing(const KMusicList &music)
{
	m_musicList = music;
	m_musicList.setSQL(false);
	refresh();
}

void KMusicListWidget::removeMusic()
{
	int pos = currentRow();
	// 移除
	m_musicList.removeMusic(pos);
	// 从ListWidget中移除
	QListWidgetItem *tempItem = takeItem(pos);
	removeItemWidget(tempItem);
	delete tempItem;
}

void KMusicListWidget::showInExplorer()
{
	int pos = currentRow();
	m_musicList.showInExplorer(pos);
}

void KMusicListWidget::detail()
{
	int pos = currentRow();
	m_musicList.detail(pos);
}

void KMusicListWidget::setIcon(QIcon icon)
{
	m_icon = icon;
}

QIcon KMusicListWidget::getIcon()
{
	return m_icon;
}
