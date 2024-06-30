// -----------------------------------------------------------
// guiStyle.h
// 创建者： 李康
// 创建时间： 2024/05/31
// 功能描述： 设置gui样式相关的qss
// -----------------------------------------------------------

#ifndef _KMUSICPLAYER_GUISTYLE_H_
#define _KMUSICPLAYER_GUISTYLE_H_

#include<QString>

// 播放按钮样式
inline QString PlayStyle()
{
	return "QPushButton"
		" {"
		"background-image:url(:/images/play.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/images/play-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/images/play.png);"

		"}";
}

// 暂停按钮样式
inline QString PaseStyle()
{
	return "QPushButton"
		" {"
		"background-image:url(:/images/pase.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/images/pase-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/images/pase.png);"

		"}";
}
inline QString RandomStyle()
{
	return "QPushButton"
		" {"
		"background-image:url(:/images/play-random.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/images/play-random-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/images/play-random.png);"

		"}";

}
inline QString LoopStyle()
{
	return "QPushButton"
		" {"
		"background-image:url(:/images/loop.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/images/loop-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/images/loop.png);"

		"}";

}
inline QString LoopOneStyle()
{
	return "QPushButton"
		" {"
		"background-image:url(:/images/loop-one.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/images/loop-one-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/images/loop-one.png);"

		"}";

}

inline QString ListWidgetStyle() {
	return "QScrollBar:vertical"
		"{"
		"width:8px;"
		"background-color:transparent;"
		"margin:0px,0px,0px,0px;"
		" padding-top:12px;   /*上预留位置*/"
		" padding-bottom:12px;    /*下预留位置*/"
		"}"


		"  QScrollBar::handle:vertical"
		" {"
		"width:8px;"
		" background-color:rgba(255,255,255,0.2);"
		" border-radius:4px;"
		" min-height:20px;"
		" }"


		"    QScrollBar::handle:vertical:hover"
		"   {"
		"      width:9px;"
		" background-color:rgba(255,255,255,0.5);"
		"   border-radius:4px;"
		"     min-height:20;"
		"  }";
}

#endif // _KMUSICPLAYER_GUISTYLE_H_
