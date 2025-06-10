#pragma once

#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#include <QPushButton>
#include <QVector>

#include "config_file_handler.hpp"
#include "logs.hpp"

class Song_folder_manager
{
	private:
		Song_folder_manager();
		~Song_folder_manager() = default;

		QString m_song_dir_path;
		QVector < QPushButton* > m_songs_buttons;

		void add_pix_map( QPushButton &button );
		void create_button( const QString &name );

	public:
		static Song_folder_manager &get_Instance()
		{
			static Song_folder_manager instance;
			return instance;
		}

		QVector< QPushButton* > &refresh_list();
};
