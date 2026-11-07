/*
 * TextFloat.cpp - class textFloat, a floating text-label
 *
 * Copyright (c) 2005-2010 Tobias Doerffel <tobydox/at/users.sourceforge.net>
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#include "TextFloat.h"

#include <QEvent>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>

#include "GuiApplication.h"
#include "MainWindow.h"

namespace lmms::gui
{


TextFloat::TextFloat() :
	TextFloat("", "", QPixmap())
{
}

TextFloat::TextFloat(const QString & title, const QString & text, const QPixmap & pixmap) :
	QWidget(getGUI()->mainWindow(), Qt::ToolTip)
{
	QHBoxLayout * mainLayout = new QHBoxLayout();
	setLayout(mainLayout);

	// Create the label that displays the pixmap
	m_pixmapLabel = new QLabel(this);
	mainLayout->addWidget(m_pixmapLabel);

	// Create the widget that displays the title and the text
	QWidget * titleAndTextWidget = new QWidget(this);
	QVBoxLayout * titleAndTextLayout = new QVBoxLayout();
	titleAndTextWidget->setLayout(titleAndTextLayout);

	m_titleLabel = new QLabel(titleAndTextWidget);
	m_titleLabel->setStyleSheet("font-weight: bold;");
	titleAndTextLayout->addWidget(m_titleLabel);

	m_textLabel = new QLabel(titleAndTextWidget);
	m_titleLabel->setWordWrap(true);
	m_textLabel->setWordWrap(true);
	titleAndTextLayout->addWidget(m_textLabel);

	mainLayout->addWidget(titleAndTextWidget);

	// Call the setters so that the hidden state is updated
	setTitle(title);
	setText(text);
	setPixmap(pixmap);
}

void TextFloat::setTitle(const QString & title)
{
	m_titleLabel->setText(title);
	m_titleLabel->setHidden(title.isEmpty());
}

void TextFloat::setText(const QString & text)
{
	m_textLabel->setText(text);
	m_textLabel->setHidden(text.isEmpty());
}

void TextFloat::setPixmap(const QPixmap & pixmap)
{
	m_pixmapLabel->setPixmap(pixmap);
	m_pixmapLabel->setHidden(pixmap.isNull());
}

void TextFloat::moveGlobal(QWidget* w, const QPoint& offset)
{
	if (m_positioning) { return; }
	m_positioning = true;
	m_anchor = w;
	m_anchorOffset = offset;
	QPoint position = w->mapToGlobal(QPoint(0, 0)) + offset;
	QScreen* screen = QGuiApplication::screenAt(position);
	if (!screen) { screen = w->screen(); }
	if (screen)
	{
		const QRect available = screen->availableGeometry();
		// Reserve the space occupied by the icon and layout margins.
		m_titleLabel->setMaximumWidth(QWIDGETSIZE_MAX);
		m_textLabel->setMaximumWidth(QWIDGETSIZE_MAX);
		const int overhead = layout()->contentsMargins().left() + layout()->contentsMargins().right()
			+ m_titleLabel->parentWidget()->layout()->contentsMargins().left()
			+ m_titleLabel->parentWidget()->layout()->contentsMargins().right()
			+ (m_pixmapLabel->isHidden() ? 0 : m_pixmapLabel->sizeHint().width() + layout()->spacing());
		const int textWidth = std::max(1, available.width() - overhead);
		m_titleLabel->setMaximumWidth(textWidth);
		m_textLabel->setMaximumWidth(textWidth);
		adjustSize();
		position.setX(
			std::clamp(position.x(), available.left(), std::max(available.left(), available.right() - width() + 1)));
		position.setY(
			std::clamp(position.y(), available.top(), std::max(available.top(), available.bottom() - height() + 1)));
	}
	else
	{
		adjustSize();
	}
	move(position);
	m_positioning = false;
}

bool TextFloat::event(QEvent* event)
{
	const auto type = event->type();
	const bool result = QWidget::event(event);
	if ((type == QEvent::FontChange || type == QEvent::ApplicationFontChange || type == QEvent::ScreenChangeInternal
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
			|| type == QEvent::DevicePixelRatioChange
#endif
			)
		&& m_anchor && !m_positioning)
	{
		// Defer until the inherited font and child layouts have received the event.
		QTimer::singleShot(0, this, [this] {
			if (m_anchor) { moveGlobal(m_anchor, m_anchorOffset); }
		});
	}
	return result;
}

void TextFloat::setVisibilityTimeOut(int msecs)
{
	QTimer::singleShot(msecs, this, SLOT(hide()));
	show();
}

TextFloat * TextFloat::displayMessage(const QString & title,
					const QString & msg,
					const QPixmap & pixmap,
					int timeout, QWidget * parent)
{
	auto tf = new TextFloat(title, msg, pixmap);

	// Show the widget so that the correct height is calculated in the code that follows
	tf->show();

	if(parent != nullptr)
	{
		tf->moveGlobal(parent, QPoint(parent->width() + 2, 0));
	}
	else
	{
		// If no parent is given move the window to the lower left area of the main window
		QWidget * mw = getGUI()->mainWindow();
		tf->moveGlobal(mw, QPoint(32, mw->height() - tf->height() - 8));
	}

	if (timeout > 0)
	{
		tf->setAttribute(Qt::WA_DeleteOnClose, true);
		QTimer::singleShot(timeout, tf, SLOT(close()));
	}

	return tf;
}

void TextFloat::mousePressEvent(QMouseEvent *)
{
	close();
}

} // namespace lmms::gui
