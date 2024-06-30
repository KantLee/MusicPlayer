// -----------------------------------------------------------
// kmusicplayer.h
// 创建者： 李康
// 创建时间： 2024/05/29
// 功能描述： 程序首页
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
    // 初始化界面样式
    void initStyle();
	// 初始化生成播放器模块及相关组件
	void initPlay();
	// 右键菜单初始化
	void initActions();
	// 数据库初始化
	void initSqlite();
	// 本地音乐初始化
	void initMusicList();
	// 在线搜歌初始化
	void initOnlineMusic();
	// 系统托盘初始化
	void initSystemTrayIcon();
	// 初始化下载器
	void initDownloader();
	// 读取本地音乐数据库完成
	void handleAllMusic(QVector<KMusic> music);
	// 设置播放位置
	void setPosition(int position);
	// 更新播放位置
	void updatePosition(qint64 position);
	// 根据位置更新播放时常
	void updateDuration(qint64 duration);
	// 更新播放信息
	void updateInfo();
	// 更新播放按钮
	void updatePlayBtn();
	// 当前播放列表-右键菜单 移除歌曲
	void playlistRemoveMusic();
	// 本地音乐添加到播放列表
	void localToPlaylist();
	// 托盘激活
	void systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
	// 点击上一首按钮
	void onBtnPreClicked();
	// 点击播放按钮
	void onBtnPlayClicked();
	// 点击下一首按钮
	void onBtnNextClicked();
	// 点击循环播放按钮
	void onBtnPlayModeClicked();
	// 退出应用
	void quitMusicPlayer();
	// 切换到播放列表界面
	void onBtnCurMusicClicked();
	// 切换到本地音乐界面
	void onBtnLocalMusicClicked();
	// 切换到在线搜歌界面
	void onBtnOnlineMusicClicked();
	// 关闭窗口
	void onBtnQuitClicked();
	// 最小化窗口
	void onBtnMinClicked();
	// 添加本地音乐
	void onBtnAddClicked();
	// 点击调整音量按钮
	void onBtnVolumeClicked();
	// 调整音量
	void onVolumeSliderValueChanged(int value);
	// 播放列表双击
	void onPlayListWidgetDoubleClicked(const QModelIndex& index);
	// 本地音乐双击
	void onLocalMusicWidgetDoubleClicked(const QModelIndex& index);
	// 播放列表创建上下文右键菜单
	void onPlayListWidgetCustomContextMenuRequested(const QPoint& pos);
	// 本地音乐创建上下文右键菜单
	void onLocalMusicWidgetCustomContextMenuRequested(const QPoint& pos);
	// 在线搜歌的上下文右键菜单
	void onOnlineListWidgetCustomContextMenuRequested(const QPoint& pos);
	// 显示歌词
	void onBtnLyricClicked();
	// 清空播放列表
	void onBtnClearPlayClicked();
	// 清空本地音乐
	void onBtnClearLocalClicked();
	// 选择测试链接
	void onComboBoxLinkSetIndex(int index);
	// 点击搜索按钮
	void onOnlineSearchClicked();
	// 获取json并解析
	void onJsonDownloadDataReady(const QByteArray& data);
	void onJsonDownloadFinished();
	// 点击下载歌曲
	void onDownloadMusic();
	// 边下边播
	void onDownloadPlayMusic();
	void onMusicDownloadDataReady(const QByteArray& data);
	void onMusicDownloadFinished();
	// 处理下载进度
	void onMusicDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
	void initLocalMusic();
protected:
    // 重写绘图事件以绘制主窗口背景
    virtual void paintEvent(QPaintEvent *event) override;
	// 实现窗口可拖动
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	// 关闭时不退出，而是到系统托盘
	virtual void closeEvent(QCloseEvent* event) override;
	// 拖拽文件进入
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;
	// 时间转换
	QString formatTime(qint64 timeMilliSeconds);

private:
    Ui::KMusicPlayerClass ui;
	// 当前播放器
	QMediaPlayer *m_player = Q_NULLPTR;
	// 当前播放列表
	QMediaPlaylist *m_playlist = Q_NULLPTR;
	// 系统托盘
	QSystemTrayIcon *m_systemTray = Q_NULLPTR;
	QAction *m_actionSystemTrayPlay = Q_NULLPTR;
	QAction *m_actionSystemTrayPlaymode = Q_NULLPTR;
	// 右键菜单
	// “播放列表”的右键菜单
	QMenu *m_menuPlaylist = Q_NULLPTR;
	// “本地音乐”的右键菜单
	QMenu *m_menuLocallist = Q_NULLPTR;
	// “在线搜歌”的右键菜单
	QMenu *m_menuOnlineSearch = Q_NULLPTR;
	// 窗口拖动时记录的起始点
	QPoint m_offset;
	// gitee仓库根目录
	QString m_baseUrl;
	// 歌曲测试链接
	QUrl m_url;
	// 数据库相关
	QThread m_queryThread;
	KDatabaseWorker* m_worker;
	KDataBase& m_database;
	// 下载相关
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
	// 下载歌曲保存位置（全局设置一次）
	QString m_savePath;
	// 保存歌曲的网络资源路径
	QVector<KMusic> m_onlineUrl;
	QBuffer *m_musicBuffer;
	QBuffer *m_jsonBuffer;
	// 保存下载歌曲的本地路径
	QString m_saveMusicPath;
	QString m_saveLyricPath;
	QString m_saveImgPath;
	// 当前是否有歌曲正在下载
	bool m_isDownloading;
};

#endif // _KMUSICPLAYER_KMUSICPLAYER_H_
