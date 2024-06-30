// -----------------------------------------------------------
// kmusiclistwidget.h
// 创建者： 李康
// 创建时间： 2024/05/31
// 功能描述： 显示歌曲列表组件
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KMUSICLISTWIDGET_H_
#define _KMUSICPLAYER_KMUSICLISTWIDGET_H_

#include <QListWidget>
#include "kmusiclist.h"

class KMusicListWidget : public QListWidget
{
	Q_OBJECT

public:
	KMusicListWidget(QWidget *parent);
	~KMusicListWidget();

	// 刷新显示（当m_musicList有所变化时，需要调用）
	void refresh();
	// 设置播放列表
	void setMusicList_playing(const KMusicList &music);
	// 移除歌曲
	void removeMusic();
	// 在文件夹中打开
	void showInExplorer();
	// 歌曲详情
	void detail();
	// 设置图标Icon
	void setIcon(QIcon icon);
	// 获取图标Icon
	QIcon getIcon();

	// 当前歌曲列表（存储的是歌曲信息）
	KMusicList m_musicList;
private:
	QIcon m_icon;
};

#endif // _KMUSICPLAYER_KMUSICLISTWIDGET_H_
