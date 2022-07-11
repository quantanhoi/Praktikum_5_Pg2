#include<DungeonCrawler.h>
#include<QMessageBox>
#include"mainwindow.h"
Level *DungeonCrawler::getCurrentLevel() const
{
    return currentLevel;
}

myList *DungeonCrawler::getLevelList() const
{
    return levelList;
}

GraphicalUI *DungeonCrawler::getPGraphicalUI() const
{
    return pGraphicalUI;
}

void DungeonCrawler::setPGraphicalUI(GraphicalUI *newPGraphicalUI)
{
    pGraphicalUI = newPGraphicalUI;
}

void DungeonCrawler::setCurrentLevel(Level *newCurrentLevel)
{
    currentLevel = newCurrentLevel;
}

void DungeonCrawler::setGameEnd(bool newGameEnd)
{
    gameEnd = newGameEnd;
}

void DungeonCrawler::initialisieren() {
    //level 1
    Level* level1 = new Level;
    levelList->push_back(level1);
    for(int i = 0; i < numRows; i ++ ) {
        for(int z = 0; z < numColumns; z++){
           level1->stage[i][z] = new Floor(level1, i, z);
        }
    }

    //test Portal
    delete level1->stage[3][1];
    delete level1->stage[6][2];
    Portal* newPort = new Portal(level1, 3, 1, nullptr);
    level1->stage[3][1]= newPort;

    Portal* newPort2 = new Portal(level1,6, 2, nullptr);
    newPort2->connectPortal(newPort);
    level1->stage[6][2] = newPort2;


    //test door and switch
    delete level1->stage[6][4];
    delete level1->stage[3][4];
    Door* door1 = new Door(level1, 6, 4);
    level1->stage[6][4] = door1;
    Switch* switch1 = new Switch(level1, 3, 4);
    level1->stage[3][4] = switch1;
    switch1->attach(door1);


    //test Pit and Ramp
    for(int i = 0; i < numRows; i ++ ) {
        for(int z = 0; z < numColumns; z++){
           if(i == 2 && z < 5 ) {
               level1->stage[i][z] = new Pit(level1, i, z);
           }
        }
    }
    level1->stage[2][5] = new Ramp(level1, 2, 5);

    //test Wall
    for(int i = 0; i < numRows; i ++ ) {
        for(int z = 0; z < numColumns; z++){
           if((i== 5 || i == 7) && z < 5 ) {
               level1->stage[i][z] = new Wall(level1, i, z);
           }
        }
    }
    //testing level changer
    delete level1->stage[3][2];
    levelChanger* changer1 = new levelChanger(level1, 3, 2, this);
    level1->stage[3][2] = changer1;

    //testing loot chest
    delete level1->stage[4][5];
    lootChest* loot1 = new lootChest(level1, 4, 5, this);
    level1->stage[4][5] = loot1;


    //level2
    Level* level2 = new Level;
    levelList->push_back(level2);
    for(int i = 0; i < numRows; i ++ ) {
        for(int z = 0; z < numColumns; z++){
           level2->stage[i][z] = new Floor(level2, i, z);
        }
    }
    iterator = levelList->begin();
    //++iterator;
    currentLevel = *iterator;
    this->currentLevel->placePlayer(3,3);
    this->currentLevel->placeNPC(4, 4);
    pGraphicalUI->chooseCharacter(this->currentLevel);    //choose the first character placed in the current level

    //level changer 2
    delete level2->stage[0][0];
    levelChanger* changer2 = new levelChanger(level2, 0, 0, this);
    level2->stage[0][0] = changer2;
    changer2->connectLevelChanger(changer1);

//3, 2
}
void DungeonCrawler::play() {
    //int i{0};
    initialisieren();
    MainWindow* mainWindow = new MainWindow(this);
    mainWindow->show();
    GraphicalUI::draw(this->currentLevel, mainWindow);







    /*this->currentLevel->placeCharacterTest(1, 3);
    this->currentLevel->placeCharacterTest(2, 6);
    Level new_level = *this->currentLevel;    //test copy constructor
    this->allLevel[1] = &new_level;
    this->pTerminalUI->draw(this->currentLevel);
    this->pTerminalUI->chooseCharacter(this->currentLevel);
    while(true) {
        system("cls");      //clear console
        this->pTerminalUI->draw(this->currentLevel);
        this->pTerminalUI->move();
        i++;
        log(i);
        sif(i == 5) {
            this->pTerminalUI->draw(this->allLevel[1]);
            Sleep(10000);
        }

    }*/
}
bool DungeonCrawler::checkEndGame() {
    if(this->gameEnd) {
        return true;
    }
    else {
        for(int i = 0; i < this->currentLevel->characterVector.size(); i++) {
            if(this->currentLevel->characterVector.at(i)->getStatus() == alive) {
                this->gameEnd = false;
                return false;
            }
            else {
                this->gameEnd = true;
                return true;
            }
        }
    }
    return false;
}
