#include "../headers/first_open_config.hpp"
#include "uis/ui_first_open_config.h"

First_Open_Config::First_Open_Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::First_Open_Config)
{
    ui->setupUi(this);

    // Dont allow user close the dialog
    setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );

    QString documents_path = QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).at(0);
    ui->folder_label->setText( documents_path );
}

First_Open_Config::~First_Open_Config()
{
    delete ui;
}

void First_Open_Config::on_select_folder_button_clicked()
{
    QString home_path = QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).at(0);
    QString selected_path = QFileDialog::getExistingDirectory( this, tr("Select Folder"), home_path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    ui->folder_label->setText( selected_path );
}

void First_Open_Config::on_confirm_button_clicked()
{
    if (ui->folder_label->text().isEmpty())
    {
        QMessageBox::warning( this, tr("Empty folder!"), tr("Please select a folder!") );
        return;
    }

    m_app_dir_path = ui->folder_label->text() + "/" + m_app_dir_name;
    m_songs_dir_path = m_app_dir_path + "/" + m_songs_dir_name;

    int result = QMessageBox::question( this, tr("Confirm"), tr("You want to use: \"") + ui->folder_label->text() + tr("\" for MushcFlow folder?"),
                  QMessageBox::Yes | QMessageBox::No );

    if (result == QMessageBox::Yes)
    {
        m_app_language = ui->language_box->currentText();
        create_config_file();
        create_app_directories();

        return;
    }

    m_app_dir_path.clear();
    m_songs_dir_path.clear();
    ui->folder_label->clear();
}

void First_Open_Config::create_config_file()
{
    QMap < QString, QString > values_to_write;
    values_to_write["app_dir"] = m_app_dir_path;
    values_to_write["songs_dir"] = m_songs_dir_path;
    values_to_write["use_thumbnail"] = "1";

    Config_file_handler::get_Instance().write_values( values_to_write );
}

void First_Open_Config::create_app_directories()
{
    QDir app_dir( m_app_dir_path );
    QDir songs_dir( m_app_dir_path + "/" + m_songs_dir_name );

    if (app_dir.exists() && songs_dir.exists())
    {
        close();
    }

    app_dir.mkdir( m_app_dir_path );
    songs_dir.mkdir( m_app_dir_path + "/" + m_songs_dir_name );

    close();
}
