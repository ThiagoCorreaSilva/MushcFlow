#include "../headers/theme.hpp"
#include "uis/ui_theme.h"

Theme::Theme( QWidget *parent, QApplication *app ) :
	QDialog(parent),
	ui(new Ui::Theme),
	app( app )
{
	ui->setupUi(this);
}

Theme::~Theme()
{
	delete ui;
}

void Theme::on_change_theme_button_clicked()
{
	QFileInfo file ( QFileDialog::getOpenFileName( this, tr( "Select a theme" ), m_themes_path, tr( "Theme files (*.qss)" )) );
	ui->current_theme_label->setText( file.fileName().remove( ".qss" ) );

	// TODO
	// SAVES THE CURRENT THEME IN CONFIG FILE!

	QFile file_content( file.absoluteFilePath() );
	if (!file_content.open( QFile::ReadOnly ))
	{
		Logs::get_Instance().create_log( { "ERROR WHILE OPENING FILE TO CHANGE THE THEME!" }, this, ERROR_TYPE::FATAL );
		return;
	}

	QString buffer = file_content.readAll();
	app->setStyleSheet( buffer );
}
