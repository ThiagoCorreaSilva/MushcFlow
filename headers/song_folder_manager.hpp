#pragma once

#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include <QMessageBox>
#include <QTranslator>
#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>

#include <source_location>

#include "config_file_handler.hpp"
#include "song_handler.hpp";
#include "logs.hpp"

class Song_folder_manager
{
	private:
		Song_folder_manager();
		~Song_folder_manager() = default;

		QString m_song_dir_path;
		QString m_thumbnail_dir_path;
		QVector < QPushButton* > m_songs_buttons;

		QWidget *m_container;
		QVBoxLayout *m_layout;
		QLabel *m_song_count_label;

		void move_thumbnails();
		void create_button( const QString &name );
		void add_pix_map( QPushButton &button );
		void button_pressed( const QString &name );
		void delete_song( const QFileInfo &file );

	public:
		static Song_folder_manager &get_Instance()
		{
			static Song_folder_manager instance;
			return instance;
		}

		void set_layout( QVBoxLayout *layout );
		void set_container( QWidget *container );
		void set_label( QLabel *label );
		QVector< QPushButton* > &refresh_list();
};
