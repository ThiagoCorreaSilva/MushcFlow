#include "first_open_config.hpp"
#include "ui_first_open_config.h"

First_Open_Config::First_Open_Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::First_Open_Config)
{
    ui->setupUi(this);

    // Dont allow user close the dialog
    //setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint );
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

    auto result = QMessageBox::question( this, tr("Confirm"), tr("You want to use: \"") + m_app_dir_path + tr("\" for MushcFlow folder?"),
                  QMessageBox::Yes | QMessageBox::No );

    if (result == QMessageBox::Yes)
    {
        m_app_language = ui->language_box->currentText();
        create_config_file();
        return;
    }

    m_app_dir_path.clear();
    m_songs_dir_path.clear();
    ui->folder_label->clear();
}

void First_Open_Config::create_config_file()
{
    QFile config_file( "configs.json" );
    if (!config_file.open( QFile::WriteOnly ))
    {
        config_file.remove();

        QStringList error_list;
        QString error_1 = "ERROR WHILE WRITING CONFIG_FILE.TXT!";
        QString error_2 = "PLEASE CLOSE THE PROGRAM AND TRY AGAIN!";

        error_list.push_back( error_1 );
        error_list.push_back( error_2 );

        log.create_log( error_list );
        exit( EXIT_FAILURE );
    }

    QJsonDocument document;
    QJsonObject object;

    object["app_dir"] = m_app_dir_path;
    object["songs_dir"] = m_songs_dir_path;
    object["language"] = m_app_language;

    document.setObject( object );

    QByteArray data_json = document.toJson();

    config_file.write( data_json );
    config_file.close();

    create_app_directories();
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
