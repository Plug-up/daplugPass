#include "DaplugPassGui.h"
#include "DaplugPass.h"

DaplugPassGui::DaplugPassGui(){

    //=== Create and initialize widgets =========================================================================================

    //general
    main_widget = new QWidget(this);
    config_widget = new QWidget(main_widget);
    passGen_widget = new QWidget(main_widget);
    auth_widget = new QWidget(main_widget);
    errors_widget = new QWidget(main_widget);
    infos_widget = new QWidget(main_widget);

    //main
    logo_main_label = new QLabel(main_widget);
    logo_main_label->setPixmap(QPixmap(":/images/logo.png"));
    wait_main_label = new QLabel(main_widget);
    wait_main_label->setFixedSize(40,40);
    wait_main_movie = new QMovie(main_widget);

    //config view
    enterPass_config_label = new QLabel(main_widget);
    confirmPass_config_label = new QLabel("Confirm password:", main_widget);
    pass_config_lineedit = new QLineEdit(main_widget);
    pass_config_lineedit->setEchoMode(QLineEdit::Password);
    confirmPass_config_lineedit = new QLineEdit(main_widget);
    confirmPass_config_lineedit->setEchoMode(QLineEdit::Password);
    recoveryKey_config_label = new QLabel("Enter the recovery key:", main_widget);
    recoveryKey_config_lineedit = new QLineEdit(main_widget);
    ok_config_button = new QPushButton("Ok", main_widget);
    ok_config_button->setFocusPolicy(Qt::NoFocus);

    //pass gen view
    enterServiceName_passGen_label = new QLabel("Enter service name:\n(gmail, facebook, windows, ...)", main_widget);
    enterServiceName_passGen_lineedit = new QLineEdit(main_widget);
    enterUserId_passGen_label = new QLabel("Enter user Id :\n(Recommended in case of multiple accounts of the same service)", main_widget);
    enterUserId_passGen_lineedit = new QLineEdit(main_widget);
    computerLogin_passGen_checkbox = new QCheckBox("Use it for computer login.", main_widget);
    go_passGen_button = new QPushButton("Go", main_widget);
    go_passGen_button->setIcon(QIcon(":/images/genPass.png"));
    go_passGen_button->setFocusPolicy(Qt::NoFocus);

    //auth view
    enterPass_auth_label = new QLabel("Enter your dongle password:", main_widget);
    pass_auth_lineedit = new QLineEdit(main_widget);
    pass_auth_lineedit->setEchoMode(QLineEdit::Password);
    ok_auth_button = new QPushButton("Ok", main_widget);
    ok_auth_button->setFocusPolicy(Qt::NoFocus);

    //errors view
    error_errors_label = new QLabel(main_widget);
    ok_errors_button = new QPushButton("Ok", main_widget);
    ok_errors_button->setFocusPolicy(Qt::NoFocus);

    //infos view
    info_infos_textedit = new QTextEdit(main_widget);
    info_infos_textedit->setReadOnly(true);
    ok_infos_button = new QPushButton("Ok", main_widget);
    ok_infos_button->setFocusPolicy(Qt::NoFocus);

    //Menu
    QMenu *daplugPass_menu = menuBar()->addMenu("&DaplugPass");
    QAction *genPassAction = new QAction("&Generate password",main_widget);
    genPassAction->setIcon(QIcon(":/images/genPass.png"));
    daplugPass_menu->addAction(genPassAction);
    daplugPass_menu->addSeparator();
    QAction *newConfigAction = new QAction("&New Configuration",main_widget);
    newConfigAction->setIcon(QIcon(":/images/newConfig.png"));
    daplugPass_menu->addAction(newConfigAction);
    QAction *recoveryAction = new QAction("&Recovery",main_widget);
    recoveryAction->setIcon(QIcon(":/images/recovery.png"));
    daplugPass_menu->addAction(recoveryAction);
    daplugPass_menu->addSeparator();
    QAction *exitAction = new QAction("&Exit", main_widget);
    exitAction->setIcon(QIcon(":/images/exit.png"));
    daplugPass_menu->addAction(exitAction);

//    QMenu *dongle_menu = menuBar()->addMenu("&Dongle");
//    QAction *redetectAction = new QAction("&Re-detect",main_widget);
//    redetectAction->setIcon(QIcon(":/images/redetect.png"));
//    dongle_menu->addAction(redetectAction);

    QMenu *about_menu = menuBar()->addMenu("&About");
    QAction *aboutAction = new QAction("&About DaplugPass", main_widget);
    aboutAction->setIcon(QIcon(":/images/about.png"));
    about_menu->addAction(aboutAction);

    //Toolbar
    QToolBar *toolBar = addToolBar("ToolBar");
    toolBar->addAction(genPassAction);
    toolBar->addSeparator();
    toolBar->addAction(newConfigAction);
    toolBar->addAction(recoveryAction);
    toolBar->addSeparator();
//    toolBar->addAction(redetectAction);
//    toolBar->addSeparator();
    toolBar->addAction(exitAction);

    //=== create, initialize and set layouts ====================================================================================

    main_vlayout = new QVBoxLayout();
    config_vlayout = new QVBoxLayout();
    passGen_vlayout = new QVBoxLayout();
    auth_vlayout = new QVBoxLayout();
    errors_vlayout = new QVBoxLayout();
    infos_vlayout = new QVBoxLayout();

    //config layout
    config_vlayout->addWidget(enterPass_config_label);
    config_vlayout->addWidget(pass_config_lineedit);
    config_vlayout->addWidget(confirmPass_config_label);
    config_vlayout->addWidget(confirmPass_config_lineedit);
    config_vlayout->addWidget(recoveryKey_config_label);
    config_vlayout->addWidget(recoveryKey_config_lineedit);
    config_vlayout->addWidget(ok_config_button);
    config_widget->setLayout(config_vlayout);

    //pass gen layout
    passGen_vlayout->addWidget(enterServiceName_passGen_label);
    passGen_vlayout->addWidget(enterServiceName_passGen_lineedit);
    passGen_vlayout->addWidget(enterUserId_passGen_label);
    passGen_vlayout->addWidget(enterUserId_passGen_lineedit);
    passGen_vlayout->addWidget(computerLogin_passGen_checkbox);
    passGen_vlayout->addWidget(go_passGen_button);
    passGen_widget->setLayout(passGen_vlayout);

    //auth layout
    auth_vlayout->addWidget(enterPass_auth_label);
    auth_vlayout->addWidget(pass_auth_lineedit);
    auth_vlayout->addWidget(ok_auth_button);
    auth_widget->setLayout(auth_vlayout);

    //errors layout
    errors_vlayout->addWidget(error_errors_label);
    errors_vlayout->addWidget(ok_errors_button);
    errors_widget->setLayout(errors_vlayout);

    //infos layout
    infos_vlayout->addWidget(info_infos_textedit);
    infos_vlayout->addWidget(ok_infos_button);
    infos_widget->setLayout(infos_vlayout);

    //main
    main_vlayout->addWidget(logo_main_label,0,Qt::AlignCenter);
    main_vlayout->addWidget(config_widget);
    main_vlayout->addWidget(passGen_widget);
    main_vlayout->addWidget(auth_widget);
    main_vlayout->addWidget(errors_widget);
    main_vlayout->addWidget(infos_widget);
    main_vlayout->addWidget(wait_main_label,0,Qt::AlignCenter);
    main_widget->setLayout(main_vlayout);

    // === style ================================================================================================================

    //main
    main_widget->setStyleSheet("background: rgb(44,44,44);");
    //logo_main_label->setStyleSheet("height: 50px, width: 50px; border: red;");

    //config view
    enterPass_config_label->setStyleSheet("color: white;");
    enterPass_config_label->setWordWrap(true);
    pass_config_lineedit->setStyleSheet("background: white; border:2px solid rgb(145,230,64); border-radius: 5px; height: 30px;");
    confirmPass_config_label->setStyleSheet("color: white;");
    confirmPass_config_label->setWordWrap(true);
    confirmPass_config_lineedit->setStyleSheet("background: white; border:2px solid rgb(145,230,64); border-radius: 5px; height: 30px;");
    recoveryKey_config_label->setStyleSheet("color: white;");
    recoveryKey_config_label->setWordWrap(true);
    recoveryKey_config_lineedit->setStyleSheet("background: white; border:2px solid rgb(145,230,64); border-radius: 5px; height: 30px;");
    ok_config_button->setStyleSheet("background: rgb(145,230,64); border:2px solid white; border-radius: 5px; height: 30px; margin-top: 20px;");

    //pass gen view
    enterServiceName_passGen_label->setStyleSheet("color: white;");
    enterServiceName_passGen_label->setWordWrap(true);
    enterServiceName_passGen_lineedit->setStyleSheet("background: white; border:2px solid rgb(145,230,64); border-radius: 5px; height: 30px;");
    enterUserId_passGen_label->setStyleSheet("color: white;");
    enterUserId_passGen_label->setWordWrap(true);
    enterUserId_passGen_lineedit->setStyleSheet("background: white; border:2px solid rgb(145,230,64); border-radius: 5px; height: 30px;");
    computerLogin_passGen_checkbox->setStyleSheet("color: white; margin-top: 20px;");
    go_passGen_button->setStyleSheet("background: rgb(145,230,64); border:2px solid white; border-radius: 5px; height: 30px; margin-top: 10px;");

    //auth view
    enterPass_auth_label->setStyleSheet("color: white;");
    enterPass_auth_label->setWordWrap(true);
    pass_auth_lineedit->setStyleSheet("background: white; border:2px solid rgb(145,230,64); border-radius: 5px; height: 30px;");
    ok_auth_button->setStyleSheet("background: rgb(145,230,64); border:2px solid white; border-radius: 5px; height: 30px; margin-top: 20px;");

    //errors view
    error_errors_label->setText("The two passwords do not match !");
    error_errors_label->setStyleSheet("color: orange; font-weight:bold;");
    error_errors_label->setWordWrap(true);
    ok_errors_button->setStyleSheet("background: rgb(145,230,64); border:2px solid white; border-radius: 5px; height: 30px;");

    //infos view
    info_infos_textedit->setStyleSheet("color: white; text-align: justify; border:0px;");
    ok_infos_button->setStyleSheet("background: rgb(145,230,64); border:2px solid white; border-radius: 5px; height: 30px; margin-top: 20px;");

    //toolbar
    toolBar->setStyleSheet("background: rgb(255,255,255); height: 20px;");

    //=== Signals ===============================================================================================================

    //Buttons
    QObject::connect(ok_config_button, SIGNAL(clicked()), this, SLOT(configure_slot()));
    QObject::connect(ok_auth_button, SIGNAL(clicked()), this, SLOT(authentication_slot()));
    QObject::connect(ok_infos_button, SIGNAL(clicked()), this, SLOT(displayInfo_slot()));
    QObject::connect(go_passGen_button, SIGNAL(clicked()), this, SLOT(genPassword_slot()));

    //Menu
    //QObject::connect(redetectAction, SIGNAL(triggered()), this, SLOT(redetectDongle_slot()));
    QObject::connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(genPassAction, SIGNAL(triggered()), this, SLOT(genPass_slot()));
    QObject::connect(newConfigAction, SIGNAL(triggered()), this, SLOT(newConfig_slot()));
    QObject::connect(recoveryAction, SIGNAL(triggered()), this, SLOT(recovery_slot()));
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about_slot()));


    //=== main ==================================================================================================================

    //init view
    setCentralWidget(main_widget);
    setWindowTitle("DaplugPASS");
    setWindowIcon(QIcon(":/images/logoBlack.png"));
    authPass = "";
    dongleSelected = false;
    startDaplugPass();

//    //=== Apdu log ===
//    extern FILE *flog_apdu;
//    flog_apdu = fopen("apdu_log.txt","w");
//    //===

}

DaplugPassGui::~DaplugPassGui(){

    Daplug_terminate();
//    extern FILE *flog_apdu;
//    fclose(flog_apdu);
    delete main_widget;
}

//=== SLOTS =====================================================================================================================

void DaplugPassGui::configure_slot(){

    //waitForConfig(true);

    //=== Check dongle ===

    if(dongleSelected){

        //To free dongles list if previously used
        Daplug_terminate();
    }

    //get first dongle
    int boxRet = 0;
    while(!Daplug_getFirstDongle()){

        boxRet = QMessageBox::information(this,"Dongle detection","Insert your Daplug dongle and click the Ok button to continue.\nIf your Daplug dongle is already inserted, reinsert it and try again."
                                              "\n\nClick the Cancel button to cancel.",
                                     QMessageBox::Ok, QMessageBox::Cancel);

        if(boxRet == QMessageBox::Cancel){
            return;
        }

    }

    dongleSelected = true;
    //===


    //=== Check configuration ===
    if(isDefinedKeyset(AUTH_KEYSET) && isDefinedKeyset(GEN_KEYSET)){
        QMessageBox::information(this,"Warning","Your Daplug dongle is already configured !",QMessageBox::Ok);
        return;
    }
    //===

    //=== Configuration ===

    bool recovery = false;

    QString pass12 = pass_config_lineedit->text();
    QString pass22 = confirmPass_config_lineedit->text();
    QString recovKey;

    //Dongle recovery case
    if(!recoveryKey_config_lineedit->isHidden()){
        recovery = true;
        recovKey = recoveryKey_config_lineedit->text();
    }


    //Password check
    if(pass12.length() < 4){
        QMessageBox::warning(this,"Warning","The minimum password length must be 4 characters !",QMessageBox::Ok);
        //waitForConfig(false);
        return;
    }
    if(pass12.compare(pass22) != 0){
        QMessageBox::warning(this,"Warning","The two passwords do not match !",QMessageBox::Ok);
        //waitForConfig(false);
        return;
    }

    authPass = pass12;

    //Create AUTH keyset
    if(!createAuthKeyset(authPass.toStdString().c_str())){
        QMessageBox::warning(this,"Warning"," Configuration failed !",QMessageBox::Ok);
        //waitForConfig(false);
        return;
    }

    //create GEN KEYSET (HMAC-SHA1)
    char recoveryKey_out[16*2+1] = "";
    if(recovery){
        if(!createGeneratorKeyset(authPass.toStdString().c_str(), recovKey.toStdString().c_str(), recoveryKey_out)){
            QMessageBox::warning(this,"Warning"," Configuration failed !",QMessageBox::Ok);
            //waitForConfig(false);
            return;
        }
    }else{
        if(!createGeneratorKeyset(authPass.toStdString().c_str(), NULL, recoveryKey_out)){
            QMessageBox::warning(this,"Warning"," Configuration failed !",QMessageBox::Ok);
            //waitForConfig(false);
            return;
        }
    }

    if(recovery){
        info_infos_textedit->setText("Your new Daplug dongle is successfuly configured and is ready to use with the DaplugPass application.\n\n"
                                  "Dongle recovery key is the same as the old one. You must keep it in a safe place.\n\n");
    }else{
        info_infos_textedit->setText("Your Daplug dongle is successfuly configured and is ready to use with the DaplugPass application.\n\n"
                                  "As a safety measure, keep the recovery key displayed below in a safe place. This key will be required for your DaplugPass dongle recovery in case of loss.\n\n"
                                  "It's a good idea to print it out and keep it safely.\n\n"
                                  "Recovery key:\n" +
                                  QString(recoveryKey_out));
    }

    //=== End of configuration ===

    //waitForConfig(false);

    showView(INFOS_VIEW);

}

void DaplugPassGui::authentication_slot(){

    //=== Check dongle ===

    if(dongleSelected){

        //To free dongles list if previously used
        Daplug_terminate();
    }

    //get first dongle
    int boxRet = 0;
    while(!Daplug_getFirstDongle()){

        boxRet = QMessageBox::information(this,"Dongle detection","Insert your Daplug dongle and click the Ok button to continue.\nIf your Daplug dongle is already inserted, reinsert it and try again."
                                              "\n\nClick the Cancel button to cancel.",
                                     QMessageBox::Ok, QMessageBox::Cancel);

        if(boxRet == QMessageBox::Cancel){
            return;
        }

    }

    dongleSelected = true;
    //===


    //=== Check configuration ===
    if(!isDefinedKeyset(AUTH_KEYSET) && !isDefinedKeyset(GEN_KEYSET)){
        QMessageBox::information(this,"Warning","Your Daplug dongle is not configured yet.\nPlease, configure your Daplug dongle or do a recovery of an old one."
                             "\n\nDaplugPass > New configuration or DaplugPass > Recovery",QMessageBox::Ok);
        return;
    }
    //===

    QString pass = pass_auth_lineedit->text();

    if(!daplugPassAuth(pass.toStdString().c_str())){
        QMessageBox::warning(this,"Warning","Make sure you have entered the right password !\n\nReinsert your Daplug dongle,  and retry...",QMessageBox::Ok);
        pass_auth_lineedit->setText("");
        pass_auth_lineedit->setFocus();
        return;
    }

    authPass = pass;

    showView(PASSGEN_VIEW);
}

void DaplugPassGui::displayInfo_slot(){

    showView(PASSGEN_VIEW);

}

//void DaplugPassGui::redetectDongle_slot(){

//    if(dongleSelected){

//        //To free dongles list if previously used
//        Daplug_terminate();
//    }

//    //get first dongle
//    if(!Daplug_getFirstDongle()){
//        QMessageBox::warning(this,"Warning","Cannot detect Daplug dongle !\nPlease, reinsert your dongle and try again.",QMessageBox::Ok);
//    }

//    dongleSelected = true;

//}

void DaplugPassGui::genPassword_slot(){

    //=== Check dongle ===

    if(dongleSelected){

        //To free dongles list if previously used
        Daplug_terminate();
    }

    //get first dongle
    int boxRet = 0;
    while(!Daplug_getFirstDongle()){

        boxRet = QMessageBox::information(this,"Dongle detection","Insert your Daplug dongle and click the Ok button to continue.\nIf your Daplug dongle is already inserted, reinsert it and try again."
                                              "\n\nClick the Cancel button to cancel.",
                                     QMessageBox::Ok, QMessageBox::Cancel);

        if(boxRet == QMessageBox::Cancel){
            return;
        }

    }

    dongleSelected = true;
    //===


    //=== Check configuration ===
    if(!isDefinedKeyset(AUTH_KEYSET) && !isDefinedKeyset(GEN_KEYSET)){
        QMessageBox::information(this,"Warning","Your Daplug dongle is not configured yet.\nPlease, configure your Daplug dongle or do a recovery of an old one."
                             "\n\nDaplugPass > New configuration or DaplugPass > Recovery",QMessageBox::Ok);
        return;
    }
    //===


    //=== Password generation ===

    QString serviceName = enterServiceName_passGen_lineedit->text();
    QString userId = enterUserId_passGen_lineedit->text();

    if(serviceName.isEmpty()){
        QMessageBox::warning(this,"Warning","You must enter a service name !",QMessageBox::Ok);
        return;
    }


    if(!computerLogin_passGen_checkbox->isChecked()){

        QMessageBox::information(this,"Information","A password will be generated by the dongle.\n"
                                 "Please, make the cursor on a password text field after clicking the Ok button.",QMessageBox::Ok);

        if(!makePasssword(authPass.toStdString().c_str(), serviceName.toStdString().c_str(), userId.toStdString().c_str(), 0)){
            QMessageBox::warning(this,"Warning","Cannot generate password !",QMessageBox::Ok);
            return;
        }
    }
    else{

        int ret = 0;
        int retBox = 0;

        ret = isDefinedForcomputerLogin(authPass.toStdString().c_str());
        if(ret != -1){
            if(ret == 1){
                retBox = QMessageBox::warning(this,"Warning","This card is already configured for computer login !"
                                     "\nAre you sure you want to modify the login password outputed by this dongle?",QMessageBox::Yes, QMessageBox::No);
            }
            if(retBox == QMessageBox::No){
                return;
            }
        }else{
            QMessageBox::warning(this,"Warning","Cannot generate password !",QMessageBox::Ok);
            return;
        }

        if(!makePasssword(authPass.toStdString().c_str(), serviceName.toStdString().c_str(), userId.toStdString().c_str(), 1)){
            QMessageBox::warning(this,"Warning","Cannot generate password !",QMessageBox::Ok);
            return;
        }

        QMessageBox::information(this,"Information","A password followed by a carriage return will be generated by the dongle at insertion.\n",QMessageBox::Ok);

        computerLogin_passGen_checkbox->setChecked(false);

    }

    //===

}

void DaplugPassGui::newConfig_slot(){

    showView(CONFIG_VIEW);

}

void DaplugPassGui::recovery_slot(){

    showView(RECOV_VIEW);
}

void DaplugPassGui::about_slot(){

    QMessageBox::information(this,"About DaplugPass","Manage your passwords simply with your Daplug dongle.\n\n"
                             "Developper : s.benamar@plug-up.com\n"
                             "Icons and colors designed by : f.aumont@plug-up.com\n\n"
                             "Plug-up International - 2014",QMessageBox::Ok);

}

void DaplugPassGui::genPass_slot(){

    if(authPass.isEmpty()){ //First auth
        showView(AUTH_VIEW);
    }else{
        showView(PASSGEN_VIEW);
    }

}

//===============================================================================================================================

void DaplugPassGui::startDaplugPass(){

    if(dongleSelected){

        //To free dongles list if previously used
        Daplug_terminate();
    }

    //get first dongle
    int boxRet = 0;
    while(!Daplug_getFirstDongle()){

        boxRet = QMessageBox::information(this,"Dongle detection","Insert your Daplug dongle and click the Ok button to continue.\nIf your Daplug dongle is already inserted, reinsert it and try again."
                                              "\n\nClick the Close button to close the application.",
                                     QMessageBox::Ok, QMessageBox::Close);

        if(boxRet == QMessageBox::Close){
            exit(EXIT_SUCCESS);
        }

    }

    dongleSelected = true;

    //Check if the admin keyset exists : personalized card
    if(!isDefinedKeyset(ADMIN_KEYSET)){
        QMessageBox::warning(this,"Warning","Your dongle is not personalized.\nMake sure you have a dongle with firmware version 1.1.9 or greather.\n",
                             QMessageBox::Close);
        exit(EXIT_SUCCESS);
    }

    //Check if auth keyset already exists : first use or not
    if(!isDefinedKeyset(AUTH_KEYSET) && !isDefinedKeyset(GEN_KEYSET)){
        QMessageBox::information(this,"Warning","Your Daplug dongle is not configured yet.\nPlease, configure your Daplug dongle or do a recovery of an old one."
                             "\n\nDaplugPass > New configuration or DaplugPass > Recovery",QMessageBox::Ok);

        showView(MAIN_VIEW);
    }else{
        showView(AUTH_VIEW);
    }

}

//=== GUI =======================================================================================================================

void DaplugPassGui::showView(int view){

    switch(view){
        case MAIN_VIEW:
            this->config_widget->setHidden(true);
            this->passGen_widget->setHidden(true);
            this->auth_widget->setHidden(true);
            this->errors_widget->setHidden(true);
            this->infos_widget->setHidden(true);
            this->main_widget->updateGeometry();
            this->setFixedSize(this->sizeHint());
            this->setMinimumWidth(GUI_MIN_WIDTH);
            this->setMinimumHeight(GUI_MIN_HEIGHT);
            break;
        case CONFIG_VIEW:
            this->config_widget->setHidden(false);
            this->passGen_widget->setHidden(true);
            this->auth_widget->setHidden(true);
            this->errors_widget->setHidden(true);
            this->infos_widget->setHidden(true);
            this->enterPass_config_label->setText("Enter a password for your dongle:");
            this->recoveryKey_config_label->setHidden(true);
            this->recoveryKey_config_lineedit->setHidden(true);
            this->wait_main_label->setHidden(true);
            //reinit editable widget
            this->pass_config_lineedit->setText("");
            this->confirmPass_config_lineedit->setText("");
            this->pass_config_lineedit->setFocus();
            //update gui
            this->config_widget->updateGeometry();
            this->main_widget->updateGeometry();
            this->setFixedSize(this->sizeHint());
            this->setMinimumWidth(GUI_MIN_WIDTH);
            break;
        case RECOV_VIEW:
            this->config_widget->setHidden(false);
            this->passGen_widget->setHidden(true);
            this->auth_widget->setHidden(true);
            this->errors_widget->setHidden(true);
            this->infos_widget->setHidden(true);
            this->enterPass_config_label->setText("Enter your old dongle password:");
            this->recoveryKey_config_label->setHidden(false);
            this->recoveryKey_config_lineedit->setHidden(false);
            this->wait_main_label->setHidden(true);
            //reinit editable widget
            this->pass_config_lineedit->setText("");
            this->confirmPass_config_lineedit->setText("");
            this->recoveryKey_config_lineedit->setText("");
            this->pass_config_lineedit->setFocus();
            //update gui
            this->config_widget->updateGeometry();
            this->main_widget->updateGeometry();
            this->setFixedSize(this->sizeHint());
            this->setMinimumWidth(GUI_MIN_WIDTH);
            break;
        case PASSGEN_VIEW:
            this->config_widget->setHidden(true);
            this->passGen_widget->setHidden(false);
            this->auth_widget->setHidden(true);
            this->errors_widget->setHidden(true);
            this->infos_widget->setHidden(true);
            this->enterServiceName_passGen_label->setFocus();
            this->wait_main_label->setHidden(true);
            //reinit editable widget
            this->enterServiceName_passGen_lineedit->setText("");
            this->enterUserId_passGen_lineedit->setText("");
            this->computerLogin_passGen_checkbox->setChecked(false);
            this->enterServiceName_passGen_lineedit->setFocus();
            //update gui
            this->main_widget->updateGeometry();
            this->setFixedSize(this->sizeHint());
            this->setMinimumWidth(GUI_MIN_WIDTH);
            break;
        case AUTH_VIEW:
            this->config_widget->setHidden(true);
            this->passGen_widget->setHidden(true);
            this->auth_widget->setHidden(false);
            this->errors_widget->setHidden(true);
            this->infos_widget->setHidden(true);
            this->pass_auth_lineedit->setFocus();
            this->wait_main_label->setHidden(true);
            //reinit editable widget
            this->pass_auth_lineedit->setText("");
            this->pass_auth_lineedit->setFocus();
            //update gui
            this->main_widget->updateGeometry();
            this->setFixedSize(this->sizeHint());
            this->setMinimumWidth(GUI_MIN_WIDTH);
            break;
        case ERRORS_VIEW:
            this->config_widget->setHidden(true);
            this->passGen_widget->setHidden(true);
            this->auth_widget->setHidden(true);
            this->errors_widget->setHidden(false);
            this->infos_widget->setHidden(true);
            this->wait_main_label->setHidden(true);
            this->main_widget->updateGeometry();
            this->setFixedSize(this->sizeHint());
            this->setMinimumWidth(GUI_MIN_WIDTH);
            break;
        case INFOS_VIEW:
            this->config_widget->setHidden(true);
            this->passGen_widget->setHidden(true);
            this->auth_widget->setHidden(true);
            this->errors_widget->setHidden(true);
            this->infos_widget->setHidden(false);
            this->wait_main_label->setHidden(true);
            this->main_widget->updateGeometry();
            this->setFixedSize(this->sizeHint());
            this->setMinimumWidth(GUI_MIN_WIDTH);
            this->setMaximumWidth(GUI_MIN_HEIGHT);
            break;
    }

}

void DaplugPassGui::showAWait(QMovie *movie, QLabel *label, QString imagePath, bool wait){

    movie->setFileName(imagePath);

    if(!movie->isValid()){
        qDebug() << "wait movie error !" << endl;
    }

    if(wait){
        movie->start();
        label->setMovie(movie);
        label->setHidden(false);
    }else{
        movie->stop();
        label->setMovie(NULL);
        label->setHidden(true);
    }

}

void DaplugPassGui::waitForConfig(bool wait){

    //wait for configuration
    ok_config_button->setHidden(wait);
    config_widget->updateGeometry();
    showAWait(wait_main_movie, wait_main_label, ":/images/wait.gif", wait);
    main_widget->updateGeometry();
}
