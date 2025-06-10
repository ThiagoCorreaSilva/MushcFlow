#pragma once

#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include <QMessageBox>
#include <QTranslator>

#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>

#include "config_file_handler.hpp"
#include "song_handler.hpp";
#include "logs.hpp"

class Song_folder_manager
{
	private:
		Song_folder_manager();
		~Song_folder_manager() = default;

		QString m_song_dir_path;
		QVector < QPushButton* > m_songs_buttons;

		QVBoxLayout *m_layout;

		void add_pix_map( QPushButton &button );
		void create_button( const QString &name );
		void button_pressed( const QString &name );

	public:
		static Song_folder_manager &get_Instance()
		{
			static Song_folder_manager instance;
			return instance;
		}

		void set_layout( QVBoxLayout *layout );
		QVector< QPushButton* > &refresh_list();
};
