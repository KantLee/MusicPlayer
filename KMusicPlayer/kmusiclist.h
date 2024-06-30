// -----------------------------------------------------------
// kmusiclist.h
// �����ߣ� �
// ����ʱ�䣺 2024/05/31
// ���������� �����б���Ϣ
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
	// �趨���ݿ��־
	void setSQL(bool on);
	// ��url��Ӹ���
	void addMusic(const QList<QUrl> &urls);
	// ���һ�׸���
	void addMusic(const KMusic &music);
	// ��ȡָ��λ�õĸ���
	KMusic getMusic(int pos);
	// ���������ּ��벥���б�
	void addToPlayList(QMediaPlaylist *playlist);
	// ���ӻ������б�
	void addToListWidget(KMusicListWidget *listWidget);
	// �Ƴ�ָ���ĸ���
	void removeMusic(int pos);
	// ���ļ����д�
	void showInExplorer(int pos);
	// ��ʾָ��������ϸ��Ϣ
	void detail(int pos);
	// ��ո����б�
	void clear();
	QVector<KMusic> &getMusicArray();
	void setMusicArray(QVector<KMusic> musicArray);
	
private:
	// �����Ƿ���Ҫ�����ݿ⽻�������磺��ǰ�����б�ɾ������ʱ����Ҫ�������ݿ⣩
	bool m_sqlFlag = true;
	// ���洢�ĸ���
	QVector<KMusic> m_musicArray;
};

#endif // _KMUSICPLAYER_KMUSICLIST_H_
