#include "kmusicplayer.h"

KMusicPlayer::KMusicPlayer(QWidget *parent)
    : QWidget(parent)
	, m_worker(new KDatabaseWorker())
	, m_database(KDataBase::getInstance())
	, m_isDownloading(false)
{
    ui.setupUi(this);
	// 使窗口接受拖放操作
	setAcceptDrops(true); 
	// 一些初始化操作
    initStyle();
	initPlay();
	initActions();
	initSqlite();
	initMusicList();
	initOnlineMusic();
	initSystemTrayIcon();
	initDownloader();
	// 连接信号和槽
	(void)connect(ui.btnPre, &QPushButton::clicked, this, &KMusicPlayer::onBtnPreClicked);
	(void)connect(ui.btnPlay, &QPushButton::clicked, this, &KMusicPlayer::onBtnPlayClicked);
	(void)connect(ui.btnNext, &QPushButton::clicked, this, &KMusicPlayer::onBtnNextClicked);
	(void)connect(ui.btnPlayMode, &QPushButton::clicked, this, &KMusicPlayer::onBtnPlayModeClicked);
	(void)connect(ui.btnCurMusic, &QPushButton::clicked, this, &KMusicPlayer::onBtnCurMusicClicked);
	(void)connect(ui.btnLocalMusic, &QPushButton::clicked, this, &KMusicPlayer::onBtnLocalMusicClicked);
	(void)connect(ui.btnOnlineMusic, &QPushButton::clicked, this, &KMusicPlayer::onBtnOnlineMusicClicked);
	(void)connect(ui.btnQuit, &QPushButton::clicked, this, &KMusicPlayer::onBtnQuitClicked);
	(void)connect(ui.btnMin, &QPushButton::clicked, this, &KMusicPlayer::onBtnMinClicked);
	(void)connect(ui.btnAdd, &QPushButton::clicked, this, &KMusicPlayer::onBtnAddClicked);
	(void)connect(ui.btnVolume, &QPushButton::clicked, this, &KMusicPlayer::onBtnVolumeClicked);
	(void)connect(ui.volumeSlider, &QSlider::valueChanged, this, &KMusicPlayer::onVolumeSliderValueChanged);
	(void)connect(ui.playListWidget
		,&KMusicListWidget::doubleClicked
		, this
		, &KMusicPlayer::onPlayListWidgetDoubleClicked);
	(void)connect(ui.localMusicWidget
		, &KMusicListWidget::doubleClicked
		, this
		, &KMusicPlayer::onLocalMusicWidgetDoubleClicked);
	(void)connect(ui.playListWidget
		, &KMusicListWidget::customContextMenuRequested
		, this
		, &KMusicPlayer::onPlayListWidgetCustomContextMenuRequested);
	(void)connect(ui.localMusicWidget
		, &KMusicListWidget::customContextMenuRequested
		, this
		, &KMusicPlayer::onLocalMusicWidgetCustomContextMenuRequested);
	(void)connect(ui.onlineListWidget
		, &KMusicListWidget::customContextMenuRequested
		, this
		, &KMusicPlayer::onOnlineListWidgetCustomContextMenuRequested);
	(void)connect(ui.btnLyric, &QPushButton::clicked, this, &KMusicPlayer::onBtnLyricClicked);
	(void)connect(ui.btnClearPlay, &QPushButton::clicked, this, &KMusicPlayer::onBtnClearPlayClicked);
	(void)connect(ui.btnClearLocal, &QPushButton::clicked, this, &KMusicPlayer::onBtnClearLocalClicked);
	(void)connect(ui.comboBoxLink, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KMusicPlayer::onComboBoxLinkSetIndex);
}

KMusicPlayer::~KMusicPlayer()
{
	m_database.closeSqlite();
	m_queryThread.quit();
	m_queryThread.wait();
	m_musicWorkThread.quit();
	m_musicWorkThread.wait();
	m_imgWorkThread.quit();
	m_imgWorkThread.wait();
	m_jsonWorkThread.quit();
	m_jsonWorkThread.wait();
	m_lyricWorkThread.quit();
	m_lyricWorkThread.wait();
	m_musicPlayThread.quit();
	m_musicPlayThread.wait();
}

void KMusicPlayer::initStyle()
{
	// 隐藏标题栏（无边框）
	setWindowFlags(Qt::FramelessWindowHint);
	// 启用样式背景绘制
	setAttribute(Qt::WA_StyledBackground);
	// 背景透明
	setAttribute(Qt::WA_TranslucentBackground);
	ui.volumeSlider->hide();
	ui.playListWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
	ui.localMusicWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
	ui.playListWidget->setIcon(QIcon(":/images/music.png"));
	ui.localMusicWidget->setIcon(QIcon(":/images/music-file.png"));
	// 打开默认为本地音乐界面
	ui.stackedWidget->setCurrentIndex(1);
}

void KMusicPlayer::initPlay()
{
	// 播放器初始化
	m_player = new QMediaPlayer(this);
	m_playlist = new QMediaPlaylist;
	m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
	m_player->setPlaylist(m_playlist);
	// 连接槽与信号
	(void)connect(ui.positionSlider, &QAbstractSlider::valueChanged, this, &KMusicPlayer::setPosition);
	(void)connect(m_player, &QMediaPlayer::positionChanged, this, &KMusicPlayer::updatePosition);
	(void)connect(m_player, &QMediaPlayer::durationChanged, this, &KMusicPlayer::updateDuration);
	(void)connect(m_player, &QMediaPlayer::metaDataAvailableChanged, this, &KMusicPlayer::updateInfo);
	(void)connect(m_player, &QMediaPlayer::stateChanged, this, &KMusicPlayer::updatePlayBtn);
}

void KMusicPlayer::initActions()
{
	// “播放列表”右键菜单初始化
	ui.playListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	QAction *actionPlaylistDelete = new QAction(QIcon(":/images/remove.png"), QStringLiteral("移除"), this);
	(void)connect(actionPlaylistDelete, &QAction::triggered, this, &KMusicPlayer::playlistRemoveMusic);
	QAction *actionPlaylistShowfile = new QAction(QIcon(":/images/music-dir.png"), QStringLiteral("打开所在文件夹"), this);
	(void)connect(actionPlaylistShowfile, &QAction::triggered, ui.playListWidget, &KMusicListWidget::showInExplorer);
	QAction *actionPlaylistDetail = new QAction(QIcon(":/images/detail.png"), QStringLiteral("歌曲详情"), this);
	(void)connect(actionPlaylistDetail, &QAction::triggered, ui.playListWidget, &KMusicListWidget::detail);
	m_menuPlaylist = new QMenu(this);
	m_menuPlaylist->addAction(actionPlaylistDelete);
	m_menuPlaylist->addAction(actionPlaylistShowfile);
	m_menuPlaylist->addAction(actionPlaylistDetail);

	// “本地音乐”右键菜单初始化
	ui.localMusicWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	QAction *actionLocallistDelete = new QAction(QIcon(":/images/remove.png"), QStringLiteral("移除"), this);
	(void)connect(actionLocallistDelete, &QAction::triggered, ui.localMusicWidget, &KMusicListWidget::removeMusic);
	QAction *actionLocallistShowfile = new QAction(QIcon(":/images/music-dir.png"), QStringLiteral("打开所在文件夹"), this);
	(void)connect(actionLocallistShowfile, &QAction::triggered, ui.localMusicWidget, &KMusicListWidget::showInExplorer);
	QAction *actionLocallistDetail = new QAction(QIcon(":/images/detail.png"), QStringLiteral("歌曲详情"), this);
	(void)connect(actionLocallistDetail, &QAction::triggered, ui.localMusicWidget, &KMusicListWidget::detail);
	QAction* actionLocalToPlaylist = new QAction(QIcon(":/images/To-playlist.png"), QStringLiteral("添加到当前播放列表"), this);
	(void)connect(actionLocalToPlaylist, &QAction::triggered, this, &KMusicPlayer::localToPlaylist);
	m_menuLocallist = new QMenu(this);
	m_menuLocallist->addAction(actionLocallistDelete);
	m_menuLocallist->addAction(actionLocallistShowfile);
	m_menuLocallist->addAction(actionLocallistDetail);
	m_menuLocallist->addAction(actionLocalToPlaylist);

	// “在线搜歌”右键菜单初始化
	ui.onlineListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	QAction *actionDownload = new QAction(QIcon(":/images/downloadIcon.png"), QStringLiteral("下载"), this);
	// 信号和槽
	(void)connect(actionDownload, &QAction::triggered, this, &KMusicPlayer::onDownloadMusic);
	QAction *actionDownloadPlaying = new QAction(QIcon(":/images/downloadOkIcon.png"), QStringLiteral("边下边播"), this);
	// 信号和槽
	(void)connect(actionDownloadPlaying, &QAction::triggered, this, &KMusicPlayer::onDownloadPlayMusic);
	m_menuOnlineSearch = new QMenu(this);
	m_menuOnlineSearch->addAction(actionDownload);
	m_menuOnlineSearch->addAction(actionDownloadPlaying);
}

void KMusicPlayer::initSqlite()
{
	if (!m_database.initSqlite())
	{
		QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("数据库初始化失败"));
		exit(-1);
	}
	connect(&m_queryThread, &QThread::finished, m_worker, &QObject::deleteLater);
	connect(this, &KMusicPlayer::initLocalMusic, m_worker, &KDatabaseWorker::getAllMusic);
	connect(m_worker, &KDatabaseWorker::onAllMusicReady, this, &KMusicPlayer::handleAllMusic);
	m_queryThread.start();
}

void KMusicPlayer::initMusicList()
{
	// 本地音乐初始化
	emit initLocalMusic();
}

void KMusicPlayer::initOnlineMusic()
{
	// 《孤勇者》json链接
	m_url.setUrl("https://gitee.com/MarkYangUp/music/raw/master/%E5%AD%A4%E5%8B%87%E8%80%85/music.json");
	ui.lineEditLink->setText(m_url.toString());
	m_baseUrl = "https://gitee.com/MarkYangUp/music/raw/master";
}

void KMusicPlayer::initSystemTrayIcon()
{
	m_systemTray = new QSystemTrayIcon(this);
	m_systemTray->setIcon(QIcon(":/images/systemTrayIcon.png"));
	m_systemTray->setToolTip(QStringLiteral("音乐播放器"));
	(void)connect(m_systemTray, &QSystemTrayIcon::activated, this, &KMusicPlayer::systemTrayIconActivated);
	// 添加菜单项
	QAction *actionSystemTrayPre = new QAction(QIcon(":/images/pre2.png"), QStringLiteral("上一首"));
	(void)connect(actionSystemTrayPre, &QAction::triggered, this, &KMusicPlayer::onBtnPreClicked);
	m_actionSystemTrayPlay = new QAction(QIcon(":/images/play2.png"), QStringLiteral("播放"));
	(void)connect(m_actionSystemTrayPlay, &QAction::triggered, this, &KMusicPlayer::onBtnPlayClicked);
	QAction *actionSystemTrayNext = new QAction(QIcon(":/images/next2.png"), QStringLiteral("下一首"));
	(void)connect(actionSystemTrayNext, &QAction::triggered, this, &KMusicPlayer::onBtnNextClicked);
	m_actionSystemTrayPlaymode = new QAction(QIcon(":/images/loop2.png"), QStringLiteral("循环播放"));
	(void)connect(m_actionSystemTrayPlaymode, &QAction::triggered, this, &KMusicPlayer::onBtnPlayModeClicked);
	QAction *actionSystemTrayQuit = new QAction(QIcon(":/images/quit.png"), QStringLiteral("退出应用"));
	(void)connect(actionSystemTrayQuit, &QAction::triggered, this, &KMusicPlayer::quitMusicPlayer);

	QMenu *pContextMenu = new QMenu(this);
	pContextMenu->addAction(actionSystemTrayPre);
	pContextMenu->addAction(m_actionSystemTrayPlay);
	pContextMenu->addAction(actionSystemTrayNext);
	pContextMenu->addAction(m_actionSystemTrayPlaymode);
	pContextMenu->addAction(actionSystemTrayQuit);
	m_systemTray->setContextMenu(pContextMenu);
	m_systemTray->show();
}

void KMusicPlayer::initDownloader()
{
	m_musicBuffer = new QBuffer(this);
	m_jsonBuffer = new QBuffer(this);
	m_jsonDownloader = new KDownloader(this);
	m_musicDownloader = new KDownloader(this);
	m_lyricDownloader = new KDownloader(this);
	m_imgDownloader = new KDownloader(this);
	m_musicPlayDownload = new KDownloader(this);
	m_jsonDownloader->moveToThread(&m_jsonWorkThread);
	m_musicDownloader->moveToThread(&m_musicWorkThread);
	m_lyricDownloader->moveToThread(&m_lyricWorkThread);
	m_imgDownloader->moveToThread(&m_imgWorkThread);
	m_musicPlayDownload->moveToThread(&m_musicPlayThread);
	// 下载JSON
	(void)connect(&m_jsonWorkThread, &QThread::finished, m_jsonDownloader, &QObject::deleteLater);
	(void)connect(ui.btnOnlineSearch, &QPushButton::clicked, this, &KMusicPlayer::onOnlineSearchClicked);
	(void)connect(m_jsonDownloader, &KDownloader::dataReady, this, &KMusicPlayer::onJsonDownloadDataReady, Qt::QueuedConnection);
	(void)connect(m_jsonDownloader, &KDownloader::downloadFinished, this, &KMusicPlayer::onJsonDownloadFinished, Qt::QueuedConnection);
	// 下载歌曲
	(void)connect(&m_musicWorkThread, &QThread::finished, m_musicDownloader, &QObject::deleteLater);
	(void)connect(m_musicDownloader, &KDownloader::downloadFinished, this, &KMusicPlayer::onMusicDownloadFinished, Qt::QueuedConnection);
	(void)connect(m_musicDownloader, &KDownloader::downloadProgress, this, &KMusicPlayer::onMusicDownloadProgress, Qt::QueuedConnection);
	// 下载歌词和封面（不需要信号反馈）
	(void)connect(&m_imgWorkThread, &QThread::finished, m_imgDownloader, &QObject::deleteLater);
	(void)connect(&m_lyricWorkThread, &QThread::finished, m_lyricDownloader, &QObject::deleteLater);
	// 边下载边播放
	(void)connect(&m_musicPlayThread, &QThread::finished, m_musicPlayDownload, &QObject::deleteLater);
	(void)connect(m_musicPlayDownload, &KDownloader::dataReady, this, &KMusicPlayer::onMusicDownloadDataReady, Qt::QueuedConnection);
	(void)connect(m_musicPlayDownload, &KDownloader::downloadFinished, this, &KMusicPlayer::onMusicDownloadFinished, Qt::QueuedConnection);
	(void)connect(m_musicPlayDownload, &KDownloader::downloadProgress, this, &KMusicPlayer::onMusicDownloadProgress, Qt::QueuedConnection);
	m_jsonWorkThread.start();
	m_musicWorkThread.start();
	m_lyricWorkThread.start();
	m_imgWorkThread.start();
	m_musicPlayThread.start();
}

void KMusicPlayer::handleAllMusic(QVector<KMusic> music)
{
	ui.localMusicWidget->m_musicList.setMusicArray(music);
	ui.localMusicWidget->refresh();
}

void KMusicPlayer::setPosition(int position)
{
	if (qAbs(m_player->position() - position) > 99)
		m_player->setPosition(position);
}

void KMusicPlayer::updatePosition(qint64 position)
{
	ui.positionSlider->setValue(static_cast<int>(position));
	ui.positionLabel->setText(formatTime(position) + "/" + formatTime(m_player->duration()));
	if (m_playlist->currentIndex() >= 0)
		ui.lyricWidget->show(position);
}

void KMusicPlayer::updateDuration(qint64 duration)
{
	ui.positionSlider->setRange(0, static_cast<int>(duration));
	ui.positionSlider->setEnabled(static_cast<int>(duration) > 0);
	if (!(static_cast<int>(duration) > 0)) 
	{
		// 无音乐播放时，界面元素
		ui.infoLabel->setText("KEEP CALM AND CARRY ON ...");
		m_systemTray->setToolTip(QStringLiteral("音乐播放器"));
		QImage image(":/images/non-music.png");
		ui.coverLabel->setPixmap(QPixmap::fromImage(image));
		ui.musicTitleLabel->setText("");
		ui.musicAlbumLabel->setText("");
		ui.musicAuthorLabel->setText("");
		ui.lyricWidget->clearLyric();
	}
	ui.positionSlider->setPageStep(static_cast<int>(duration) / 10);
}

void KMusicPlayer::updateInfo()
{
	if (m_player->isMetaDataAvailable())
	{
		// 返回可用MP3元数据列表（调试时可以查看）
		QStringList listInfo_debug = m_player->availableMetaData();
		// 歌曲信息
		QString info = "";
		QString author = m_player->metaData(QStringLiteral("Author")).toStringList().join(",");
		info.append(author);
		QString title = m_player->metaData(QStringLiteral("Title")).toString();
		QString albumTitle = m_player->metaData(QStringLiteral("AlbumTitle")).toString();
		info.append(" - " + title);
		info.append(" [" + formatTime(m_player->duration()) + "]");
		ui.infoLabel->setText(info);
		m_systemTray->setToolTip("正在播放：" + info);
		// 获取封面图片
		QImage picImage;
		picImage = m_player->metaData(QStringLiteral("ThumbnailImage")).value<QImage>();
		if (picImage.isNull()) 
			picImage.load(m_saveImgPath);
		if (picImage.isNull())
			picImage = QImage(":/images/non-music.png");
		ui.coverLabel->setPixmap(QPixmap::fromImage(picImage));
		ui.coverLabel->setScaledContents(true);
		// 改变正在播放歌曲的图标
		for (int i = 0; i < m_playlist->mediaCount(); i++)
		{
			QListWidgetItem *p = ui.playListWidget->item(i);
			p->setIcon(ui.playListWidget->getIcon());
		}
		int index = m_playlist->currentIndex();
		QListWidgetItem *p = ui.playListWidget->item(index);
		p->setIcon(QIcon(":/images/music-playing.png"));

		// 歌词界面显示的信息
		ui.musicTitleLabel->setText(title);
		ui.musicAlbumLabel->setText(QStringLiteral("专辑：") + albumTitle);
		ui.musicAuthorLabel->setText(QStringLiteral("歌手：") + author);
		// 解析歌词
		ui.lyricWidget->process(ui.playListWidget->m_musicList.getMusicArray()[index].getLyricFile());
	}
}

void KMusicPlayer::updatePlayBtn()
{
	if (m_player->state() == QMediaPlayer::PlayingState)
	{
		ui.btnPlay->setStyleSheet(PlayStyle());
		m_actionSystemTrayPlay->setIcon(QIcon(":/images/pause2.png"));
		m_actionSystemTrayPlay->setText(QStringLiteral("暂停"));
	}
	else {
		ui.btnPlay->setStyleSheet(PaseStyle());
		m_actionSystemTrayPlay->setIcon(QIcon(":/images/play2.png"));
		m_actionSystemTrayPlay->setText(QStringLiteral("播放"));
	}
}

void KMusicPlayer::playlistRemoveMusic()
{
	int pos = ui.playListWidget->currentRow();
	int playing_pos = m_playlist->currentIndex();
	ui.playListWidget->removeMusic();
	if (pos < playing_pos) 
	{
		// 移除前备份
		QMediaPlayer::State state =m_player->state();// 记录下删除前的播放状态
		qint64 position_backup = m_player->position();
		playing_pos = playing_pos - 1;
		m_playlist->removeMedia(pos);
		// 移除后恢复
		m_playlist->setCurrentIndex(playing_pos);
		m_player->setPosition(position_backup);
		ui.positionSlider->setValue(position_backup);
		if (state == QMediaPlayer::PlayingState)
			m_player->play();
		else if (state == QMediaPlayer::PlayingState)
			m_player->pause();
	}
	else if (pos < playing_pos)
	{
		m_playlist->removeMedia(pos);
		m_playlist->setCurrentIndex(pos);
	}
	else
	{
		m_playlist->removeMedia(pos);
	}
}

void KMusicPlayer::localToPlaylist()
{
	int pos = ui.localMusicWidget->currentRow();
	KMusic tempMusic = ui.localMusicWidget->m_musicList.getMusic(pos);
	ui.playListWidget->m_musicList.addMusic(tempMusic);
	ui.playListWidget->refresh();
	// 添加到播放器
	m_playlist->addMedia(tempMusic.getUrl());
}

void KMusicPlayer::systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::DoubleClick:
		if (isHidden())
			show();
		else
			hide();
		break;
	default:
		break;
	}
}

void KMusicPlayer::onBtnPreClicked()
{
	m_playlist->previous();
}

void KMusicPlayer::onBtnPlayClicked()
{
	if (m_player->state() == QMediaPlayer::PlayingState)
	{
		m_player->pause();
	}
	else if (m_player->state() == QMediaPlayer::PausedState)
	{
		m_player->play();
	}
	else if (!m_playlist->isEmpty() && (m_player->state() == QMediaPlayer::StoppedState))
	{
		m_playlist->setCurrentIndex(0);
		m_player->play();
	}
}

void KMusicPlayer::onBtnNextClicked()
{
	m_playlist->next();
}

void KMusicPlayer::onBtnPlayModeClicked()
{
	if (m_playlist->playbackMode() == QMediaPlaylist::Loop) 
	{
		ui.btnPlayMode->setStyleSheet(RandomStyle());
		ui.btnPlayMode->setToolTip(QStringLiteral("随机播放"));
		m_actionSystemTrayPlaymode->setIcon(QIcon(":/images/random2.png"));
		m_actionSystemTrayPlaymode->setText(QStringLiteral("随机播放"));
		m_playlist->setPlaybackMode(QMediaPlaylist::Random);
	}
	else if (m_playlist->playbackMode() == QMediaPlaylist::Random) 
	{
		ui.btnPlayMode->setStyleSheet(LoopOneStyle());
		ui.btnPlayMode->setToolTip(QStringLiteral("单曲循环"));
		m_actionSystemTrayPlaymode->setIcon(QIcon(":/images/loop-one2.png"));
		m_actionSystemTrayPlaymode->setText(QStringLiteral("单曲循环"));
		m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	}
	else if (m_playlist->playbackMode() == QMediaPlaylist::CurrentItemInLoop) 
	{
		ui.btnPlayMode->setStyleSheet(LoopStyle());
		ui.btnPlayMode->setToolTip(QStringLiteral("循环播放"));
		m_actionSystemTrayPlaymode->setIcon(QIcon(":/images/loop2.png"));
		m_actionSystemTrayPlaymode->setText(QStringLiteral("循环播放"));
		m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
	}
}

void KMusicPlayer::quitMusicPlayer()
{
	QApplication::quit();
}

void KMusicPlayer::onBtnCurMusicClicked()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void KMusicPlayer::onBtnLocalMusicClicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void KMusicPlayer::onBtnOnlineMusicClicked()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void KMusicPlayer::onBtnQuitClicked()
{
	if (ui.checkBox->isChecked())
		close();
	else
		QApplication::quit();
}

void KMusicPlayer::onBtnMinClicked()
{
	showMinimized();
}

void KMusicPlayer::onBtnAddClicked()
{
	QFileDialog fileDialog(this);
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setFileMode(QFileDialog::ExistingFiles);
	fileDialog.setWindowTitle(QStringLiteral("添加本地音乐，可多选"));
	// 设置文件后缀过滤器
	QStringList nameFilters;
	nameFilters << QStringLiteral("音频文件 (*.mp3 *.flac *.mpga)")
		<< QStringLiteral("MP3 文件 (*.mp3)")
		<< QStringLiteral("FLAC 文件 (*.flac)")
		<< QStringLiteral("MPGA 文件 (*.mpga)");
	fileDialog.setNameFilters(nameFilters);
	fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
	if (fileDialog.exec() == QDialog::Accepted)
	{
		QList<QUrl> urls = fileDialog.selectedUrls();
		ui.localMusicWidget->m_musicList.addMusic(urls);
		ui.localMusicWidget->refresh();
		ui.stackedWidget->setCurrentIndex(1);
	}
}

void KMusicPlayer::onBtnVolumeClicked()
{
	if (ui.volumeSlider->isHidden())
		ui.volumeSlider->show();
	else
		ui.volumeSlider->hide();
}

void KMusicPlayer::onVolumeSliderValueChanged(int value)
{
	m_player->setVolume(value);
}

void KMusicPlayer::onPlayListWidgetDoubleClicked(const QModelIndex& index)
{
	int i = index.row();
	m_playlist->setCurrentIndex(i);
	m_player->play();
}

void KMusicPlayer::onLocalMusicWidgetDoubleClicked(const QModelIndex& index)
{
	m_playlist->clear();
	ui.localMusicWidget->m_musicList.addToPlayList(m_playlist);
	ui.playListWidget->setMusicList_playing(ui.localMusicWidget->m_musicList);
	int i = index.row();
	m_playlist->setCurrentIndex(i);
	m_player->play();
	ui.stackedWidget->setCurrentIndex(0);
}

void KMusicPlayer::onPlayListWidgetCustomContextMenuRequested(const QPoint& pos)
{
	if (ui.playListWidget->itemAt(pos) == Q_NULLPTR)
		return;
	m_menuPlaylist->exec(QCursor::pos());
}

void KMusicPlayer::onLocalMusicWidgetCustomContextMenuRequested(const QPoint& pos)
{
	if (ui.localMusicWidget->itemAt(pos) == Q_NULLPTR)
		return;
	m_menuLocallist->exec(QCursor::pos());
}

void KMusicPlayer::onOnlineListWidgetCustomContextMenuRequested(const QPoint& pos)
{
	if (ui.onlineListWidget->itemAt(pos) == Q_NULLPTR)
		return;
	m_menuOnlineSearch->exec(QCursor::pos());
}

void KMusicPlayer::onBtnLyricClicked()
{
	if (ui.stackedWidget->currentIndex() == 3)
		ui.stackedWidget->setCurrentIndex(0);
	else
		ui.stackedWidget->setCurrentIndex(3);
}

void KMusicPlayer::onBtnClearPlayClicked()
{
	QMessageBox::StandardButton btn;
	btn = QMessageBox::question(this
		, QStringLiteral("提示")
		, QStringLiteral("此操作不可逆！\n确实要清空吗?")
		, QMessageBox::Yes | QMessageBox::No);
	if (btn == QMessageBox::Yes)
	{
		ui.playListWidget->m_musicList.clear();
		ui.playListWidget->refresh();
		m_playlist->clear();
	}
}

void KMusicPlayer::onBtnClearLocalClicked()
{
	QMessageBox::StandardButton btn;
	btn = QMessageBox::question(this
		, QStringLiteral("提示")
		, QStringLiteral("此操作不可逆！\n确实要清空吗?")
		, QMessageBox::Yes | QMessageBox::No);
	if (btn == QMessageBox::Yes)
	{
		ui.localMusicWidget->m_musicList.clear();
		ui.localMusicWidget->refresh();
	}
}

void KMusicPlayer::onComboBoxLinkSetIndex(int index)
{
	// 0为《孤勇者》，1为《风雨无阻》
	if (index == 0)
		m_url.setUrl("https://gitee.com/MarkYangUp/music/raw/master/%E5%AD%A4%E5%8B%87%E8%80%85/music.json");
	else
		m_url.setUrl("https://gitee.com/MarkYangUp/music/raw/master/%E9%A3%8E%E9%9B%A8%E6%97%A0%E9%98%BB/music.json");
	ui.lineEditLink->setText(m_url.toString());
}

void KMusicPlayer::onOnlineSearchClicked()
{
	m_jsonDownloader->startDownload(m_url);
}

void KMusicPlayer::onJsonDownloadDataReady(const QByteArray& data)
{
	m_jsonBuffer->open(QIODevice::Append);
	m_jsonBuffer->write(data);
	m_jsonBuffer->close();
}

void KMusicPlayer::onJsonDownloadFinished()
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(m_jsonBuffer->data());
	QJsonObject jsonObject = jsonDoc.object();
	QJsonArray musicList = jsonObject["list"].toArray();
	ui.onlineListWidget->clear();
	m_onlineUrl.clear();
	// 遍历音乐列表
	for (const QJsonValue &value : musicList)
	{
		QJsonObject musicObject = value.toObject();
		QString musicName = musicObject["musicName"].toString();
		QString albumName = musicObject["albumName"].toString();
		qint64 duration = musicObject["duration"].toInt() * qint64(1000);
		QString path = musicObject["path"].toString();
		QString mp3 = musicObject["mp3"].toString();
		QString lyric = musicObject["lyric"].toString();
		QString img = musicObject["img"].toString();
		QListWidgetItem *item = new QListWidgetItem(ui.onlineListWidget);
		item->setIcon(ui.onlineListWidget->getIcon());
		QString info = musicName + " [" + formatTime(duration) + "]";
		item->setText(info);
		ui.onlineListWidget->addItem(item);
		QUrl musicUrl(m_baseUrl + path + "/" + mp3);
		QUrl lyricUrl(m_baseUrl + path + "/" + lyric);
		QUrl imgUrl(m_baseUrl + path + "/" + img);
		KMusic tmpMusic;
		tmpMusic.setOnlineMusicUrl(musicUrl);
		tmpMusic.setOnlineLyricUrl(lyricUrl);
		tmpMusic.setOnlineImgUrl(imgUrl);
		tmpMusic.setMusicName(musicName.replace(" ", ""));
		m_onlineUrl.append(tmpMusic);
	}
	// 清空
	m_jsonBuffer->setData(QByteArray());
}

void KMusicPlayer::onDownloadMusic()
{
	if (m_isDownloading)
	{
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请等待当前歌曲下载完毕！"));;
		return;
	}
	if (m_savePath.isEmpty())
	{
		QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("选择歌曲下载保存路径"), "");
		if (!path.isEmpty())
			m_savePath = path;
		else
			return;
	}
	// 获取点击的位置
	int pos = ui.onlineListWidget->currentRow();
	QUrl musicUrl = m_onlineUrl[pos].getOnlineMusicUrl();
	QUrl lyricUrl = m_onlineUrl[pos].getOnlineLyricUrl();
	QUrl imgUrl = m_onlineUrl[pos].getOnlineImgUrl();
	m_saveMusicPath = m_savePath + '/' + m_onlineUrl[pos].getMusicName() + ".mp3";
	m_saveLyricPath = m_savePath + '/' + m_onlineUrl[pos].getMusicName() + ".txt";
	m_saveImgPath = m_savePath + '/' + m_onlineUrl[pos].getMusicName() + ".jpg";
	// 检查是否已经存在歌曲
	if (m_database.existMusic(m_saveMusicPath))
	{
		QMessageBox::warning(this, QStringLiteral("提示"), m_saveMusicPath + QStringLiteral("已经存在"));
	}
	else
	{
		m_musicDownloader->startDownload(musicUrl, m_saveMusicPath);
		m_lyricDownloader->startDownload(lyricUrl, m_saveLyricPath);
		m_imgDownloader->startDownload(imgUrl, m_saveImgPath);
		m_isDownloading = true;
	}
}

void KMusicPlayer::onDownloadPlayMusic()
{
	QMessageBox::warning(this, QStringLiteral("提示"), m_saveMusicPath + QStringLiteral("尚未开发完成"));
}

void KMusicPlayer::onMusicDownloadDataReady(const QByteArray& data)
{
	m_musicBuffer->write(data);
}

void KMusicPlayer::onMusicDownloadFinished()
{
	QMessageBox::information(this, QStringLiteral("提示"), m_saveMusicPath + QStringLiteral(" 下载完成"));
	m_isDownloading = false;
	QList<QUrl> urls;
	urls.append(QUrl::fromLocalFile(m_saveMusicPath));
	ui.localMusicWidget->m_musicList.addMusic(urls);
	ui.localMusicWidget->refresh();
	ui.stackedWidget->setCurrentIndex(1);
	if (m_musicBuffer->isOpen())
	{
		m_musicBuffer->close();
		m_musicBuffer->setData(QByteArray());
	}
}

void KMusicPlayer::onMusicDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if (bytesTotal > 0) 
	{
		int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
		// 更新进度条（在此项目无效，因为Gitee中获取不了总大小）
		ui.downloadProgressBar->setValue(progress);
	}
}

void KMusicPlayer::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	// 将当前的KMusicPlayer部件作为参数传递给init方法，opt对象就包含了当前部件的样式选项
	opt.init(this);
	QPainter p(this);
	// 使用当前部件的样式（由style()返回）来绘制一个基本的部件图元
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void KMusicPlayer::mousePressEvent(QMouseEvent* event)
{
	// 实现点击界面中某点，音量条隐藏
	int x = event->pos().x();
	int y = event->pos().y();
	int x_widget = ui.volumeSlider->geometry().x();
	int y_widget = ui.volumeSlider->geometry().y();
	int w = ui.volumeSlider->geometry().width();
	int h = ui.volumeSlider->geometry().height();
	if (!(x >= x_widget && x <= x_widget + w && y >= y_widget && y <= y_widget + h))
		ui.volumeSlider->hide();
	// 记录窗口移动的初始位置
	m_offset = event->globalPos() - pos();
	event->accept();
}

void KMusicPlayer::mouseMoveEvent(QMouseEvent* event)
{
	int x = event->pos().x();
	int y = event->pos().y();
	if ((y < ui.titleLabel->geometry().height()) && (x < ui.layoutWidget1->geometry().x())) 
	{
		move(event->globalPos() - m_offset);
		event->accept();
		setCursor(Qt::ClosedHandCursor);
	}
}

void KMusicPlayer::mouseReleaseEvent(QMouseEvent* event)
{
	m_offset = QPoint();
	event->accept();
	setCursor(Qt::ArrowCursor);
}

void KMusicPlayer::closeEvent(QCloseEvent* event)
{
	// 最小化到托盘
	if (!m_systemTray->isVisible())
		m_systemTray->show();
	hide();
	event->ignore();
}

void KMusicPlayer::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void KMusicPlayer::dropEvent(QDropEvent* event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	ui.localMusicWidget->m_musicList.addMusic(urls);
	ui.localMusicWidget->refresh();
	ui.stackedWidget->setCurrentIndex(1);// 切换到本地音乐
}

QString KMusicPlayer::formatTime(qint64 timeMilliSeconds)
{
	qint64 seconds = timeMilliSeconds / 1000;
	const qint64 minutes = seconds / 60;
	seconds -= minutes * 60;
	return QStringLiteral("%1:%2")
		.arg(minutes, 2, 10, QLatin1Char('0'))
		.arg(seconds, 2, 10, QLatin1Char('0'));
}
