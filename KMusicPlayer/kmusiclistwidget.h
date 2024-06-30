// -----------------------------------------------------------
// kmusiclistwidget.h
// �����ߣ� �
// ����ʱ�䣺 2024/05/31
// ���������� ��ʾ�����б����
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

	// ˢ����ʾ����m_musicList�����仯ʱ����Ҫ���ã�
	void refresh();
	// ���ò����б�
	void setMusicList_playing(const KMusicList &music);
	// �Ƴ�����
	void removeMusic();
	// ���ļ����д�
	void showInExplorer();
	// ��������
	void detail();
	// ����ͼ��Icon
	void setIcon(QIcon icon);
	// ��ȡͼ��Icon
	QIcon getIcon();

	// ��ǰ�����б��洢���Ǹ�����Ϣ��
	KMusicList m_musicList;
private:
	QIcon m_icon;
};

#endif // _KMUSICPLAYER_KMUSICLISTWIDGET_H_
