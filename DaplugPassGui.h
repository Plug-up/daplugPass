#ifndef DAPLUGPASSGUI_H
#define DAPLUGPASSGUI_H

#include <QtWidgets>
#include <QMainWindow>

class DaplugPassGui : public QMainWindow{

    static const int    MAIN_VIEW = 0,
                        CONFIG_VIEW = 1,
                        RECOV_VIEW = 2,
                        PASSGEN_VIEW = 3,
                        AUTH_VIEW = 4,
                        ERRORS_VIEW = 5,
                        INFOS_VIEW = 6;

    static const int    GUI_MIN_WIDTH = 330,
                        GUI_MIN_HEIGHT = 330;

    Q_OBJECT

    public:
    DaplugPassGui();
    ~DaplugPassGui();

    public slots:
    void configure_slot();
    void newConfig_slot();
    void recovery_slot();
    void genPass_slot();
    void authentication_slot();
//    void redetectDongle_slot();
    void displayInfo_slot();
    void genPassword_slot();
    void about_slot();

    private:

    //=== GUI ===================================================================================================================

    QWidget *main_widget, //Main
            *config_widget, //Configuration
            *passGen_widget, //Password generation
            *auth_widget, //Authentication
            *errors_widget, //Error messages
            *infos_widget; //Information messages

    QVBoxLayout *main_vlayout,
                *config_vlayout,
                *passGen_vlayout,
                *auth_vlayout,
                *errors_vlayout,
                *infos_vlayout;

    QLabel  *logo_main_label,
            *wait_main_label,
            *enterPass_config_label,
            *confirmPass_config_label,
            *recoveryKey_config_label,
            *enterServiceName_passGen_label,
            *enterUserId_passGen_label,
            *enterPass_auth_label,
            *error_errors_label;

    QLineEdit   *pass_config_lineedit,
                *confirmPass_config_lineedit,
                *recoveryKey_config_lineedit,
                *enterServiceName_passGen_lineedit,
                *enterUserId_passGen_lineedit,
                *pass_auth_lineedit;

    QPushButton *ok_config_button,
                *go_passGen_button,
                *ok_auth_button,
                *ok_errors_button,
                *ok_infos_button;

    QCheckBox   *computerLogin_passGen_checkbox;

    QTextEdit   *info_infos_textedit;

    QMovie  *wait_main_movie;

    //=== DATA ==================================================================================================================

    QString authPass;
    bool dongleSelected;

    void showView(int view);
    void showAWait(QMovie *movie, QLabel *label, QString imagePath, bool wait);
    void waitForConfig(bool wait);
    void startDaplugPass();


};


#endif // DAPLUGPASSGUI_H
