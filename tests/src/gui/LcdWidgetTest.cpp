/*
 * LcdWidgetTest.cpp - LCD label layout regression tests
 *
 * This file is part of LMMS - https://lmms.io
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QFontMetrics>
#include <QImage>
#include <QPainter>
#include <QPixmapCache>
#include <QtTest>

#include "LcdWidget.h"

using lmms::gui::LcdWidget;

class LcdWidgetTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase()
	{
		// Provide a deterministic atlas without relying on an installed theme.
		QPixmap atlas(120, 20);
		atlas.fill(Qt::black);
		QPainter painter(&atlas);
		painter.fillRect(10, 0, 10, 10, Qt::white);
		painter.end();
		QPixmapCache::insert(QStringLiteral("lcd_19green"), atlas);
	}

	void labelFollowsFontChanges()
	{
		LcdWidget lcd(3, nullptr);
		QVERIFY(lcd.cellHeight() > 0);
		lcd.setLabel(QStringLiteral("Long label gy"));
		QFont small = lcd.font();
		small.setPointSize(8);
		lcd.setFont(small);
		const QSize smallSize = lcd.size();

		QFont large = small;
		large.setPointSize(24);
		lcd.setFont(large);
		QVERIFY(lcd.width() > smallSize.width());
		QVERIFY(lcd.height() > smallSize.height());
		QVERIFY(lcd.width() >= QFontMetrics(large, &lcd).horizontalAdvance(QStringLiteral("Long label gy")));
		QVERIFY(lcd.height() >= lcd.cellHeight() + QFontMetrics(large, &lcd).height());

		lcd.setLabel(QString());
		QVERIFY(lcd.height() < smallSize.height());
	}

	void valuesChangeTheRenderedDigits()
	{
		LcdWidget lcd(3, nullptr);
		QVERIFY(lcd.cellHeight() > 0);
		lcd.setSeamless(true, false);
		lcd.setValue(0);
		const QImage zero = lcd.grab().toImage();
		lcd.setValue(1);
		const QImage one = lcd.grab().toImage();
		QVERIFY(zero != one);
	}

};

QTEST_MAIN(LcdWidgetTest)
#include "LcdWidgetTest.moc"
