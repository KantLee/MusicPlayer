// -----------------------------------------------------------
// kmusiclist.h
// 创建者： 李康
// 创建时间： 2024/05/31
// 功能描述： 歌曲列表信息
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KMUSICLIST_H_
#define _KMUSICPLAYER_KMUSICLIST_H_

#include <QObject>
#include <QMediaPlaylist>
#include <QListWidget> 
#include <QProgressDialog>
#include <QCoreApplication>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QtSql>
#include <algorithm>

#include "kmusic.h"
#include "kdatabase.h"

class KMusicListWidget;

class KMusicList
{
public:
	KMusicList(){}
	KMusicList(const QList<QUrl> &urls);
	~KMusicList();
	// 设定数据库标志
	void setSQL(bool on);
	// 从url添加歌曲
	void addMusic(const QList<QUrl> &urls);
	// 添加一首歌曲
	void addMusic(const KMusic &music);
	// 获取指定位置的歌曲
	KMusic getMusic(int pos);
	// 将本地音乐加入播放列表
	void addToPlayList(QMediaPlaylist *playlist);
	// 可视化歌曲列表
	void addToListWidget(KMusicListWidget *listWidget);
	// 移除指定的歌曲
	void removeMusic(int pos);
	// 在文件夹中打开
	void showInExplorer(int pos);
	// 显示指定歌曲详细信息
	void detail(int pos);
	// 清空歌曲列表
	void clear();
	QVector<KMusic> &getMusicArray();
	void setMusicArray(QVector<KMusic> musicArray);
	
private:
	// 控制是否需要与数据库交互（比如：当前播放列表删除歌曲时不需要更新数据库）
	bool m_sqlFlag = true;
	// 所存储的歌曲
	QVector<KMusic> m_musicArray;
};

#endif // _KMUSICPLAYER_KMUSICLIST_H_
