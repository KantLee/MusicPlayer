// -----------------------------------------------------------
// kmusicplayer.h
// �����ߣ� �
// ����ʱ�䣺 2024/05/29
// ���������� ������ҳ
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_KMUSICPLAYER_H_
#define _KMUSICPLAYER_KMUSICPLAYER_H_

#include <QtWidgets/QWidget>
#include <QPainter>
#include <QScrollBar>
#include <QMediaPlayer>
#include <QSystemTrayIcon>
#include <QMediaPlaylist>
#include <QMenu>
#include <QMouseEvent>
#include <QFileDialog>
#include <QThread>

#include "ui_kmusicplayer.h"
#include "guiStyle.h"
#include "kdownloader.h"
#include "kdatabase.h"
#include "kdatabaseworker.h"

class KMusicPlayer : public QWidget
{
    Q_OBJECT

public:
    KMusicPlayer(QWidget *parent = Q_NULLPTR);
	~KMusicPlayer();
    // ��ʼ��������ʽ
    void initStyle();
	// ��ʼ�����ɲ�����ģ�鼰������
	void initPlay();
	// �Ҽ��˵���ʼ��
	void initActions();
	// ���ݿ��ʼ��
	void initSqlite();
	// �������ֳ�ʼ��
	void initMusicList();
	// �����Ѹ��ʼ��
	void initOnlineMusic();
	// ϵͳ���̳�ʼ��
	void initSystemTrayIcon();
	// ��ʼ��������
	void initDownloader();
	// ��ȡ�����������ݿ����
	void handleAllMusic(QVector<KMusic> music);
	// ���ò���λ��
	void setPosition(int position);
	// ���²���λ��
	void updatePosition(qint64 position);
	// ����λ�ø��²���ʱ��
	void updateDuration(qint64 duration);
	// ���²�����Ϣ
	void updateInfo();
	// ���²��Ű�ť
	void updatePlayBtn();
	// ��ǰ�����б�-�Ҽ��˵� �Ƴ�����
	void playlistRemoveMusic();
	// ����������ӵ������б�
	void localToPlaylist();
	// ���̼���
	void systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	// �����һ�װ�ť
	void onBtnPreClicked();
	// ������Ű�ť
	void onBtnPlayClicked();
	// �����һ�װ�ť
	void onBtnNextClicked();
	// ���ѭ�����Ű�ť
	void onBtnPlayModeClicked();
	// �˳�Ӧ��
	void quitMusicPlayer();
	// �л��������б����
	void onBtnCurMusicClicked();
	// �л����������ֽ���
	void onBtnLocalMusicClicked();
	// �л��������Ѹ����
	void onBtnOnlineMusicClicked();
	// �رմ���
	void onBtnQuitClicked();
	// ��С������
	void onBtnMinClicked();
	// ��ӱ�������
	void onBtnAddClicked();
	// �������������ť
	void onBtnVolumeClicked();
	// ��������
	void onVolumeSliderValueChanged(int value);
	// �����б�˫��
	void onPlayListWidgetDoubleClicked(const QModelIndex& index);
	// ��������˫��
	void onLocalMusicWidgetDoubleClicked(const QModelIndex& index);
	// �����б����������Ҽ��˵�
	void onPlayListWidgetCustomContextMenuRequested(const QPoint& pos);
	// �������ִ����������Ҽ��˵�
	void onLocalMusicWidgetCustomContextMenuRequested(const QPoint& pos);
	// �����Ѹ���������Ҽ��˵�
	void onOnlineListWidgetCustomContextMenuRequested(const QPoint& pos);
	// ��ʾ���
	void onBtnLyricClicked();
	// ��ղ����б�
	void onBtnClearPlayClicked();
	// ��ձ�������
	void onBtnClearLocalClicked();
	// ѡ���������
	void onComboBoxLinkSetIndex(int index);
	// ���������ť
	void onOnlineSearchClicked();
	// ��ȡjson������
	void onJsonDownloadDataReady(const QByteArray& data);
	void onJsonDownloadFinished();
	// ������ظ���
	void onDownloadMusic();
	// ���±߲�
	void onDownloadPlayMusic();
	void onMusicDownloadDataReady(const QByteArray& data);
	void onMusicDownloadFinished();
	// �������ؽ���
	void onMusicDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
	void initLocalMusic();
protected:
    // ��д��ͼ�¼��Ի��������ڱ���
    virtual void paintEvent(QPaintEvent *event) override;
	// ʵ�ִ��ڿ��϶�
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	// �ر�ʱ���˳������ǵ�ϵͳ����
	virtual void closeEvent(QCloseEvent* event) override;
	// ��ק�ļ�����
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;
	// ʱ��ת��
	QString formatTime(qint64 timeMilliSeconds);

private:
    Ui::KMusicPlayerClass ui;
	// ��ǰ������
	QMediaPlayer *m_player = Q_NULLPTR;
	// ��ǰ�����б�
	QMediaPlaylist *m_playlist = Q_NULLPTR;
	// ϵͳ����
	QSystemTrayIcon *m_systemTray = Q_NULLPTR;
	QAction *m_actionSystemTrayPlay = Q_NULLPTR;
	QAction *m_actionSystemTrayPlaymode = Q_NULLPTR;
	// �Ҽ��˵�
	// �������б����Ҽ��˵�
	QMenu *m_menuPlaylist = Q_NULLPTR;
	// ���������֡����Ҽ��˵�
	QMenu *m_menuLocallist = Q_NULLPTR;
	// �������Ѹ衱���Ҽ��˵�
	QMenu *m_menuOnlineSearch = Q_NULLPTR;
	// �����϶�ʱ��¼����ʼ��
	QPoint m_offset;
	// gitee�ֿ��Ŀ¼
	QString m_baseUrl;
	// ������������
	QUrl m_url;
	// ���ݿ����
	QThread m_queryThread;
	KDatabaseWorker* m_worker;
	KDataBase& m_database;
	// �������
	QThread m_musicWorkThread;
	QThread m_lyricWorkThread;
	QThread m_imgWorkThread;
	QThread m_jsonWorkThread;
	QThread m_musicPlayThread;
	KDownloader *m_jsonDownloader;
	KDownloader *m_musicDownloader;
	KDownloader *m_lyricDownloader;
	KDownloader *m_imgDownloader;
	KDownloader *m_musicPlayDownload;
	// ���ظ�������λ�ã�ȫ������һ�Σ�
	QString m_savePath;
	// ���������������Դ·��
	QVector<KMusic> m_onlineUrl;
	QBuffer *m_musicBuffer;
	QBuffer *m_jsonBuffer;
	// �������ظ����ı���·��
	QString m_saveMusicPath;
	QString m_saveLyricPath;
	QString m_saveImgPath;
	// ��ǰ�Ƿ��и�����������
	bool m_isDownloading;
};

#endif // _KMUSICPLAYER_KMUSICPLAYER_H_
